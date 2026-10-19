# Milestone 1: starea mașinii și încărcarea ROM-ului

## Ce construiești

În milestone-ul ăsta emulatorul nu execută încă nimic. Construiești „hardware-ul”: memoria, registrele, stiva. Apoi pui în memorie fontul și programul (ROM-ul), exact unde le caută CHIP-8.

Ai patru TODO-uri:

| TODO | Fișier | Ce faci |
|---|---|---|
| 1 | `src/chip8.h` | Definești structura `Chip8` și alegi tipurile |
| 2 | `src/chip8.c` | `chip8_init`: starea de pornire |
| 3 | `src/chip8.c` | `chip8_load_rom`: citești fișierul în memorie |
| 4 | `src/main.c` | Verifici argumentele din linia de comandă |

**Milestone-ul e gata când** `test_m1` afișează `37/37 teste trecute`, iar `chip8 roms/timendus/2-ibm-logo.ch8` afișează memoria ca în secțiunea 10.

Ordinea recomandată: TODO 1 → compilezi → TODO 2 → rulezi testele → TODO 3 → TODO 4.

---

## 1. Harta memoriei

CHIP-8 are 4096 bytes de memorie, cu adrese de la `0x000` la `0xFFF`.

```
0x000 ┌──────────────────────────┐
      │ (liber, rezervat)        │  Pe COSMAC VIP aici stătea interpretorul.
0x050 ├──────────────────────────┤
      │ Font 0-F (80 bytes)      │  0x050 - 0x09F
0x0A0 ├──────────────────────────┤
      │ (liber)                  │
0x200 ├──────────────────────────┤
      │ ROM-ul (programul)       │  Aici începe execuția.
      │          ...             │  Maxim 4096 - 512 = 3584 bytes.
0xFFF └──────────────────────────┘
```

**De ce 0x200?** Pe calculatoarele originale, primii 512 bytes erau ocupați de interpretorul CHIP-8. Programele au fost scrise să înceapă după el, așa că toate ROM-urile se așteaptă să fie încărcate la `0x200`. Toate salturile din ele sunt calculate pe baza asta.

**De ce 0x050 pentru font?** E o convenție, nu o regulă. Orice adresă sub `0x200` ar merge, pentru că programele găsesc fontul printr-o instrucțiune (în milestone 3), nu printr-o adresă fixă. `0x050` e alegerea cea mai răspândită.

---

## 2. Registrele și tipurile lor

| Registru | Ce ține | Interval de valori |
|---|---|---|
| `V0`–`VF` | Date de uz general. `VF` e și flag (carry, coliziune). | 0–255 |
| `I` | O adresă de memorie (de obicei a unui sprite) | 0x000–0xFFF |
| `PC` | Adresa instrucțiunii curente | 0x000–0xFFF |
| `delay_timer` | Contor care scade de 60 ori/s | 0–255 |
| `sound_timer` | Contor care scade de 60 ori/s; cât e > 0, se aude un beep | 0–255 |

### Tipuri cu dimensiune fixă: `<stdint.h>`

În C, `int` poate avea 2, 4 sau 8 bytes, în funcție de platformă. Pentru emulare ai nevoie de dimensiuni exacte, așa că folosești tipurile din `<stdint.h>`:

| Tip | Bytes | Interval |
|---|---|---|
| `uint8_t` | 1 | 0 … 255 |
| `uint16_t` | 2 | 0 … 65 535 |
| `uint32_t` | 4 | 0 … ~4,3 miliarde |
| `int8_t`, `int16_t`, … | | variantele **cu semn** (pot fi negative) |

**Adrese pe 12 biți.** `0xFFF` are nevoie de 12 biți. Nu există `uint12_t`, deci iei cel mai mic tip care îi cuprinde. Biții în plus rămân nefolosiți. În milestone 2 vei vedea cum tai valorile la 12 biți cu o mască.

### De ce contează să fie fără semn

Pentru tipurile **fără semn**, standardul C garantează *wrap-around*: `255 + 1` devine `0`, iar `0 - 1` devine `255`. Exact asta face și un registru hardware de 8 biți.

Pentru tipurile **cu semn**, overflow-ul e *comportament nedefinit*: compilatorul poate face orice. Un emulator construit pe `int8_t` ar avea bug-uri greu de găsit.

Testele verifică ambele proprietăți: dimensiunea (`sizeof`) și faptul că tipul e fără semn (`0 - 1` dă o valoare pozitivă).

---

## 3. Stiva și stack pointer-ul

Stiva ține **adresele de întoarcere** de la subrutine. Când programul apelează o subrutină, adresa curentă se pune pe stivă. La `return`, se scoate de acolo. Sunt 16 niveluri, deci maxim 16 apeluri imbricate.

- Fiecare element al stivei e o **adresă**. Gândește-te ce tip are.
- `sp` (stack pointer) reține **câte adrese sunt pe stivă**. Altfel spus, e indexul primului loc liber. La pornire stiva e goală, deci `sp = 0`.

```
după 2 apeluri:   stack: [0x204][0x31A][  ][  ] ...
                                         ↑
                                       sp = 2
```

Instrucțiunile care folosesc stiva vin în milestone 3. Acum doar o declari.

---

## 4. Ecranul și tastatura

- **Ecranul:** 64×32 pixeli, fiecare aprins sau stins. Îl declari ca matrice `[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH]`, adică întâi rândul (y), apoi coloana (x). Accesul la pixelul de la (x, y) arată deci așa: `display[y][x]`.
- **Tastatura:** 16 taste, fiecare apăsată (1) sau eliberată (0).

Pentru amândouă ar fi ajuns un singur bit pe element. Un byte pe element e mai risipitor, dar mult mai simplu de folosit. 2 KB în plus nu contează pe un PC.

---

## 5. Fontul

Programele CHIP-8 pot afișa cifrele hex 0–F folosind un font din memorie. Fiecare caracter are 4 pixeli lățime și 5 înălțime, stocat ca 5 bytes. Doar primii 4 biți din fiecare byte contează:

```
cifra "1":   0x20 = 0010 0000    ..#.
             0x60 = 0110 0000    .##.
             0x20 = 0010 0000    ..#.
             0x20 = 0010 0000    ..#.
             0x70 = 0111 0000    .###
```

Tabelul complet, `CHIP8_FONTSET`, e deja în `chip8.c`. Tu doar îl copiezi la adresa potrivită.

---

## 6. Structuri în C

### Definiție cu `typedef`

```c
typedef struct {
    float   temperatura;
    uint8_t zile[7];
} Senzor;

Senzor s;               // o variabilă de tip Senzor
s.temperatura = 21.5f;  // acces direct: punct
s.zile[0] = 3;
```

### Transmiterea prin pointer

Structura `Chip8` are peste 6 KB. Dacă ai scrie `void chip8_init(Chip8 chip)`, funcția ar primi o **copie**, iar modificările s-ar pierde la ieșire. De aceea funcțiile primesc un pointer: `Chip8 *chip`.

```c
void incalzeste(Senzor *s) {
    s->temperatura += 1.0f;   // prin pointer: săgeată
}                             // s->x e prescurtarea pentru (*s).x

incalzeste(&s);               // & = adresa variabilei
```

### `sizeof`

`sizeof(x)` dă dimensiunea în bytes a unei variabile sau a unui tip:
- `sizeof(s.zile)` = 7, adică tot array-ul.
- `sizeof(s)` = dimensiunea structurii întregi.
- Dacă `p` e un pointer la structură, `sizeof(*p)` = dimensiunea structurii, iar `sizeof(p)` = dimensiunea pointerului (8 bytes pe un PC pe 64 de biți). **Confuzia asta e o sursă clasică de bug-uri.**

### De ce `static Chip8 chip;` în `main`

Variabilele locale normale stau pe stiva programului, care pe unele sisteme e mică (pe un ESP32, câțiva KB). `static` mută variabila într-o zonă de memorie permanentă și o inițializează cu 0. Pe PC ar merge și fără, dar e un obicei bun pentru structuri mari.

---

## 7. `memset` și `memcpy` (`<string.h>`)

```c
void *memset(void *dest, int valoare, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
```

- `memset` umple `n` bytes, începând de la `dest`, cu `valoare`.
- `memcpy` copiază `n` bytes de la `src` la `dest`. Zonele nu au voie să se suprapună.

```c
uint8_t buffer[100];
memset(buffer, 0, sizeof(buffer));        // tot buffer-ul pe 0

uint8_t salut[3] = { 'H', 'i', '!' };
memcpy(&buffer[10], salut, sizeof(salut)); // buffer[10..12] = H, i, !
```

`&buffer[10]` înseamnă „adresa elementului 10”, adică punctul de unde începe copierea. Se poate scrie și `buffer + 10`.

**Atenție:** `memset` lucrează pe **bytes**. `memset(x, 0, …)` pune 0 corect în orice tip, dar `memset(x, 1, …)` pe un `uint16_t[]` dă `0x0101 = 257` în fiecare element, nu `1`.

---

## 8. Lucrul cu fișiere (`<stdio.h>`)

### Deschidere: `fopen`

```c
FILE *f = fopen("date.bin", "rb");
if (f == NULL) {
    // fișierul nu există, nu ai drepturi etc.
}
```

| Mod | Înseamnă |
|---|---|
| `"r"` | citire, **mod text** |
| `"rb"` | citire, **mod binar** |
| `"w"` / `"wb"` | scriere (șterge conținutul existent) |

### Capcana de pe Windows: text vs. binar

Pe Linux și macOS, `"r"` și `"rb"` se comportă identic. **Pe Windows nu:** în mod text, biblioteca C „repară” fișierul în timp ce îl citește:
- perechea `0x0D 0x0A` (`\r\n`) devine un singur byte `0x0A`;
- byte-ul `0x1A` e interpretat ca sfârșit de fișier, iar citirea se oprește acolo.

Pentru un fișier text e util. Pentru un ROM e un dezastru: instrucțiunile care conțin acești bytes se corup, iar jocul crapă fără vreo eroare evidentă. Unul dintre teste verifică exact asta, cu un ROM care conține `0x0D 0x0A 0x1A`.

### Dimensiunea fișierului: `fseek`, `ftell`, `rewind`

```c
fseek(f, 0, SEEK_END);   // mută cursorul la final; returnează 0 la succes
long dim = ftell(f);     // poziția curentă = numărul de bytes; -1 la eroare
rewind(f);               // înapoi la început
```

### Citire: `fread`

```c
size_t fread(void *dest, size_t dim_element, size_t nr_elemente, FILE *f);
```

Citește până la `nr_elemente` elemente de câte `dim_element` bytes și le pune la `dest`. **Returnează numărul de elemente citite complet**, nu numărul de bytes (decât dacă `dim_element` e 1). Dacă rezultatul e mai mic decât ai cerut, citirea a eșuat sau s-a terminat fișierul.

```c
uint8_t buf[512];
size_t n = fread(buf, 1, sizeof(buf), f);   // n = câți bytes s-au citit
```

### Închidere: `fclose`

Orice `fopen` reușit trebuie urmat de un `fclose`, **pe toate drumurile prin funcție**, inclusiv la `return false` din mijloc. Fiecare fișier rămas deschis e o resursă pierdută. Când scrii funcția, urmărește fiecare `return` și întreabă-te: „aici fișierul e închis?”

### Mesaje de eroare

```c
fprintf(stderr, "Eroare: nu pot deschide %s\n", path);
```

`stderr` e canalul de erori, separat de `stdout` (unde scrie `printf`). Nu e bufferizat, deci mesajul apare imediat. Iar dacă cineva redirecționează ieșirea normală într-un fișier (`chip8 rom.ch8 > log.txt`), erorile se văd în continuare în consolă.

---

## 9. Argumentele din linia de comandă

```c
int main(int argc, char *argv[])
```

- `argc` = numărul de argumente, **inclusiv numele programului**.
- `argv[0]` = numele sau calea programului, `argv[1]` = primul argument propriu-zis etc.

Pentru `chip8 roms/pong.ch8`: `argc = 2`, `argv[0] = "chip8"` (pe Windows, de obicei calea completă spre `.exe`), `argv[1] = "roms/pong.ch8"`.

Dacă rulezi `chip8` fără argumente, `argv[1]` e `NULL`, iar `fopen(NULL, …)` poate crăpa programul. De aceea TODO 4 verifică argumentele înainte de orice altceva.

---

## 10. Cum rulezi

### Testele
1. Sus-dreapta în CLion, alege configurația **test_m1** și apasă ▶.
2. Fiecare verificare afișează `[OK]` sau `[FAIL]`, iar la final apare `X/37 teste trecute`.
3. Mesajele de eroare de tipul `Dimensiune invalida...` sunt normale: testele verifică intenționat și cazuri greșite. Sunt mesajele *tale*, din `chip8_load_rom`.

### Programul
1. Alege configurația **chip8** → **Edit Configurations…**
2. **Program arguments:** `roms/timendus/2-ibm-logo.ch8`
3. **Working directory:** `$ProjectFileDir$`. Altfel calea relativă se caută în `cmake-build-debug`.
4. ▶

**Rezultat corect:** fontul începe cu `F0 90 90 90 F0 20 60 20 ...`, la `0x200` primul rând e
```
0x200: 00 E0 A2 2A 60 0C 61 08 D0 1F 70 09 A2 39 D0 1F
```
iar ultima linie e `PC = 0x200`.

`00E0` e prima instrucțiune din ROM: „șterge ecranul”. În milestone 2 o vei executa.

---

## 11. Specificația exactă (ce verifică testele)

**Structura (TODO 1):** pentru fiecare câmp, cel mai mic tip fără semn care cuprinde toate valorile lui. Numele câmpurilor sunt cele din comentariul din `chip8.h`.

**`chip8_init` (TODO 2):**
- funcționează și pe o structură care conține deja date („gunoi”);
- după apel: memoria e 0 peste tot, cu excepția fontului de la `0x050`; registrele, stiva, `sp`, timerele, ecranul și tastele sunt 0; `pc = 0x200`.

**`chip8_load_rom` (TODO 3):**
- ROM valid (1–3584 bytes): conținutul ajunge byte cu byte la `0x200`; restul memoriei, fontul și `pc` rămân neschimbate; returnează `true`;
- fișier inexistent, fișier gol sau mai mare de 3584 bytes: returnează `false` și afișează un mesaj pe `stderr`;
- la un ROM prea mare, **memoria nu se modifică deloc**;
- citirea se face exact, inclusiv pentru bytes `0x0D`, `0x0A`, `0x1A`.

**`main` (TODO 4):** cu alt număr de argumente decât unul, afișează pe `stderr` `Utilizare: <argv[0]> <cale_rom>` și iese cu `EXIT_FAILURE`. Testele nu verifică asta; o verifici tu, rulând programul fără argumente.

---

## 12. Întrebări de lămurit înainte să scrii cod

Dacă nu știi să răspunzi la vreuna, recitește secțiunea indicată sau întreabă-mă.

1. Ce valoare maximă trebuie să țină `sp`? Ce tip îi trebuie? (§3)
2. De ce `chip8_init` primește `Chip8 *` și nu `Chip8`? (§6)
3. În `chip8_init`, care e diferența dintre `sizeof(chip)` și `sizeof(*chip)`? Care e cel corect? (§6)
4. Ce se întâmplă cu un ROM care conține `0x1A` dacă îl deschizi cu `"r"` pe Windows? (§8)
5. De ce trebuie verificată dimensiunea **înainte** de `fread`, nu după? (§11)
6. Ce returnează `fread(dest, 1, 300, f)` dacă fișierul are doar 200 de bytes? Dar `fread(dest, 300, 1, f)`? (§8)

---

## Când termini

```
git add .
git commit -m "Milestone 1: starea masinii si incarcarea ROM-ului"
git push
```

Apoi scrie-mi „verifică milestone 1”. Clonez repo-ul, rulez testele și primești review linie cu linie.

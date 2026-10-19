# Emulator CHIP-8 (C + SDL2)

Proiect hobby, în stil CS50: scheletul vine gata, logica emulatorului o scriu eu.

## Milestone 0: setup

### 1. Deschide proiectul în CLion
1. Dezarhivează folderul `chip8-emulator` unde vrei (de exemplu în `Documents\Proiecte`).
2. CLion → **File → Open** → alege folderul `chip8-emulator` (cel care conține `CMakeLists.txt`).
3. Dacă CLion întreabă de toolchain, lasă **MinGW (bundled)**, cel inclus în CLion. Nu trebuie instalat nimic separat.

### 2. Prima configurare
La deschidere, CMake descarcă și compilează automat SDL2. Durează 1–3 minute, o singură dată.
Progresul se vede în fereastra **CMake** de jos. La final trebuie să apară `Build files have been written to...`.

> SDL2 nu se instalează manual: `CMakeLists.txt` îl aduce singur prin `FetchContent`.
> Avantaj: același proiect se compilează identic pe orice calculator.

### 3. Rulează
Selectează configurația **chip8** sus-dreapta și apasă ▶ (Run).

**Rezultat corect:**
- se deschide o fereastră 640×320 cu o tablă de șah alb-negru care se mișcă;
- în consola CLion apare `SDL 2.32.10 functioneaza. Apasa Esc ca sa inchizi.`;
- **Esc** sau butonul X închide fereastra.

Dacă vezi toate astea, setup-ul e gata.

### 4. GitHub
```
git init
git add .
git commit -m "Milestone 0: setup CLion + SDL2"
```
Apoi creează repo-ul `chip8-emulator` pe GitHub și urmează instrucțiunile de „push an existing repository”.

## Probleme frecvente

| Simptom | Cauză probabilă | Soluție |
|---|---|---|
| Configurarea CMake eșuează la descărcare | Fără internet sau firewall | Verifică conexiunea, apoi **File → Reload CMake Project** |
| `SDL.h: No such file or directory` | Configurarea CMake n-a terminat | Așteaptă sau dă **Reload CMake Project** |
| Erori ciudate după ce ai schimbat ceva în CMake | Cache vechi | **Tools → CMake → Reset Cache and Reload Project** |

## Structura
```
chip8-emulator/
├── CMakeLists.txt   configurarea build-ului (SDL2 inclus automat)
├── src/
│   └── main.c       milestone 0: test de fereastră
└── roms/            aici vor sta jocurile (.ch8)
```

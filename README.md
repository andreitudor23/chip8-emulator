# Emulator CHIP-8 (C + SDL2)

Proiect hobby, în stil CS50: scheletul vine gata, logica emulatorului o scriu eu.

## Milestone-uri

| # | Conținut | Documentație | Teste |
|---|---|---|---|
| 0 | Setup CLion + SDL2 | secțiunea de mai jos | — |
| 1 | Starea mașinii, fontul, încărcarea ROM-ului | [`docs/milestone1.md`](docs/milestone1.md) | `test_m1` |

## Structura
```
chip8-emulator/
├── CMakeLists.txt     build: nucleul (fără SDL), programul, testele
├── src/
│   ├── chip8.h        starea mașinii + interfața nucleului
│   ├── chip8.c        nucleul emulatorului (C pur, fără SDL)
│   └── main.c         punctul de intrare
├── tests/
│   ├── test.h         mini-framework de testare
│   └── test_m1.c      testele milestone 1
├── docs/              documentația fiecărui milestone
└── roms/timendus/     ROM-uri de test (GPL-3.0, vezi README din folder)
```

## Setup (milestone 0)

### Deschiderea proiectului în CLion
1. CLion → **File → Open** → folderul care conține `CMakeLists.txt`.
2. În fereastra **Open Project Wizard**, lasă setările implicite (Debug, MinGW, Ninja) → **OK**.
3. La prima configurare, CMake descarcă și compilează automat SDL2 (1–3 minute, o singură dată).

### Configurațiile de rulare
- **test_m1**: rulează testele. Nu are nevoie de argumente.
- **chip8**: programul. În **Edit Configurations…** setează:
  - **Program arguments:** calea ROM-ului, de exemplu `roms/timendus/2-ibm-logo.ch8`
  - **Working directory:** `$ProjectFileDir$`

### Git
```
git add .
git commit -m "descriere scurta"
git push
```

## Probleme frecvente

| Simptom | Cauză probabilă | Soluție |
|---|---|---|
| Configurarea CMake eșuează la descărcare | Fără internet sau firewall | Verifică conexiunea, apoi **File → Reload CMake Project** |
| `SDL.h: No such file or directory` | Configurarea CMake n-a terminat | Așteaptă sau dă **Reload CMake Project** |
| `Nu pot deschide roms/...` la rulare | Working directory greșit | Setează `$ProjectFileDir$` în configurația **chip8** |
| Erori ciudate după modificări în CMake | Cache vechi | **Tools → CMake → Reset Cache and Reload Project** |

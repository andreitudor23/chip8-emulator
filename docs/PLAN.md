# Proiect: Emulator CHIP-8 (C + SDL2)

Fișier de context pentru Project-ul din Claude. Se actualizează la fiecare milestone.

## Scop
Proiect hobby în stil CS50: Claude livrează scheletul de cod cu TODO-uri, documentația de teorie și testele. Andrei scrie logica emulatorului. **Nu vibe coding.**

Produsul final: un executabil (`chip8.exe`) care primește un fișier ROM (`./chip8 roms/pong.ch8`), deschide o fereastră 64×32 scalată ×10 și rulează jocuri CHIP-8 clasice (Pong, Tetris, Space Invaders, Breakout).

## Decizii luate
| Subiect | Decizie |
|---|---|
| Limbaj | C11 |
| Grafică / input / sunet | SDL2 2.32.10, adus automat prin CMake `FetchContent`, legat static |
| IDE | CLion (licență facultate), toolchain MinGW inclus în CLion |
| Sistem de operare | Windows 11 |
| Versionare | Git + GitHub, commit după fiecare milestone |
| Arhitectură | Nucleul (`chip8.c/.h`) nu include SDL; e compilat ca bibliotecă separată `chip8_core`, folosită de program și de teste. Partea de platformă stă separat. |
| Portare ESP32 | Amânată, se face după ce emulatorul merge pe PC |

## Reguli de lucru
- Claude este **tutor, nu programator**: nu dă soluții complete la TODO-uri decât la cererea explicită „dă-mi soluția”.
- Când Andrei se blochează, hint-urile vin pe niveluri: întrebare → concept → pseudocod.
- Review-ul de cod se face **linie cu linie**, pe fișierele reale din repo (clonate, compilate, testate), nu din descrieri.
- Partea de SDL (fereastră, taste, sunet) o livrează Claude gata scrisă, iar Andrei scrie CPU-ul.
- Fiecare milestone vine cu: document de teorie, schelet cu TODO-uri, teste automate.
- Comunicare în română, concisă.

## Milestone-uri
| # | Conținut | Criteriu de „gata” | Status |
|---|---|---|---|
| 0 | Setup: CLion + CMake + SDL2, fereastră de test | Apare fereastra cu tabla de șah, Esc o închide | Gata |
| 1 | Structura CPU: memorie 4 KB, registre V0–VF, I, PC, stivă; fontul 0–F; încărcarea ROM-ului de la 0x200 | `test_m1`: 37/37; dump-ul IBM Logo corect | Livrat, în lucru |
| 2 | Ciclul fetch–decode–execute + primele instrucțiuni (00E0, 1NNN, 6XNN, 7XNN, ANNN, DXYN) | Rulează ROM-ul **IBM Logo** | — |
| 3 | Restul instrucțiunilor: salturi condiționale, subrutine, aritmetică 8XY*, FX*, BCD | Trec ROM-urile de test Timendus (opcode, flags) | — |
| 4 | Tastatură, timere delay/sound, beep, sincronizare 60 Hz | Pong și Breakout jucabile | — |
| 5 | Quirks și timing (8XY6/8XYE, FX55/FX65, BNNN, clipping) | Tetris și Space Invaders rulează corect | — |
| 6 | Bonus: debugger (registre, instrucțiunea curentă, pas cu pas) | La alegere | — |

**După proiect (opțional):** portare pe ESP32 (OLED SSD1306 128×64, keypad 4×4, buzzer), un joc propriu scris în Octo, extensiile SUPER-CHIP/XO-CHIP.

## Structura repo-ului
```
chip8-emulator/
├── CMakeLists.txt     build: chip8_core (fără SDL), chip8, teste
├── src/
│   ├── chip8.h        structura Chip8 + interfața nucleului
│   ├── chip8.c        nucleul emulatorului
│   └── main.c         punctul de intrare
├── tests/             test.h (mini-framework) + test_mN.c
├── docs/              milestoneN.md: teoria fiecărui milestone
└── roms/timendus/     ROM-urile de test Timendus (GPL-3.0)
```
Din milestone 2 apare `platform.c/.h` (partea de SDL).

## Linkuri
- Repo GitHub: https://github.com/andreitudor23/chip8-emulator
- Ghid Tobias V. Langhoff, „Guide to making a CHIP-8 emulator”: tobiasvl.github.io/blog/write-a-chip-8-emulator
- Cowgod's CHIP-8 Technical Reference: devernay.free.fr/hacks/chip8/C8TECH10.HTM
- ROM-uri de test: github.com/Timendus/chip8-test-suite

## Jurnal
- **2026-10-19:** ales proiectul (CHIP-8 în loc de aplicația de flashcards), limbajul (C + SDL2) și mediul (CLion pe Windows 11). Livrat milestone 0.
- **2026-10-19:** milestone 0 verificat (repo publicat pe GitHub). Livrat milestone 1: structura, init, încărcarea ROM-ului, 37 de teste, `docs/milestone1.md`.

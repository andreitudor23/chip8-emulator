/*
 * main.c - punctul de intrare
 *
 * Milestone 1: initializeaza masina, incarca ROM-ul primit ca argument
 * si afiseaza memoria, ca sa vezi ca totul a ajuns la locul potrivit.
 * Fereastra SDL revine in milestone 2, cand avem ce desena.
 *
 * Rulare: chip8 <cale_rom>
 *   ex.:  chip8 roms/timendus/2-ibm-logo.ch8
 */

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

/* Afiseaza `count` bytes din memorie, de la adresa `start`, 16 pe rand (dat). */
static void dump_memory(const Chip8 *chip, uint16_t start, uint16_t count)
{
    for (uint16_t row = 0; row < count; row += 16) {
        printf("0x%03X:", start + row);
        for (uint16_t col = 0; col < 16 && row + col < count; col++) {
            printf(" %02X", chip->memory[start + row + col]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    /* ------------------------------------------------------------------
     * TODO 4: verifica argumentele
     *
     * Daca programul nu primeste exact un argument (calea ROM-ului),
     * afiseaza pe stderr un mesaj de utilizare, de forma:
     *     Utilizare: chip8 <cale_rom>
     * si iesi cu EXIT_FAILURE.
     *
     * Intrebare: ce contine argv[0]? Foloseste-l in mesaj.
     * ------------------------------------------------------------------ */

    static Chip8 chip;   /* static: vezi docs/milestone1.md, sectiunea 6 */
    chip8_init(&chip);

    if (!chip8_load_rom(&chip, argv[1])) {
        return EXIT_FAILURE;
    }

    printf("Font (0x%03X):\n", CHIP8_FONT_ADDRESS);
    dump_memory(&chip, CHIP8_FONT_ADDRESS, CHIP8_FONT_SIZE);

    printf("\nInceputul ROM-ului (0x%03X):\n", CHIP8_START_ADDRESS);
    dump_memory(&chip, CHIP8_START_ADDRESS, 64);

    printf("\nPC = 0x%03X\n", chip.pc);
    return EXIT_SUCCESS;
}

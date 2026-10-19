/*
 * chip8.c - nucleul emulatorului CHIP-8
 *
 * Milestone 1: initializarea masinii si incarcarea ROM-ului.
 * Specificatia completa: docs/milestone1.md
 */

#include "chip8.h"

#include <stdio.h>
#include <string.h>

/* ---------------------------------------------------------------------------
 * Fontul CHIP-8 (dat). Fiecare caracter are 5 bytes = 5 randuri de 8 pixeli,
 * din care se folosesc doar primii 4 biti (cei mai semnificativi).
 *
 * Exemplu, cifra "0":     0xF0 = 1111 0000   ####
 *                         0x90 = 1001 0000   #  #
 *                         0x90 = 1001 0000   #  #
 *                         0x90 = 1001 0000   #  #
 *                         0xF0 = 1111 0000   ####
 * ------------------------------------------------------------------------- */
const uint8_t CHIP8_FONTSET[CHIP8_FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

/* ---------------------------------------------------------------------------
 * TODO 2: chip8_init
 *
 * Starea de pornire:
 *   - toata memoria, registrele, stiva, timerele, ecranul si tastele pe 0
 *   - fontul copiat in memorie incepand cu CHIP8_FONT_ADDRESS
 *   - pc pe CHIP8_START_ADDRESS
 *
 * Indiciu: exista o functie standard care umple o zona de memorie cu
 * o valoare, si una care copiaza o zona de memorie in alta. Ambele sunt
 * in <string.h>. Gandeste-te ce zona de memorie ocupa structura intreaga.
 * ------------------------------------------------------------------------- */
void chip8_init(Chip8 *chip)
{
    /* TODO */
    (void)chip;   /* sterge linia asta cand implementezi functia */
}

/* ---------------------------------------------------------------------------
 * TODO 3: chip8_load_rom
 *
 * Pasi, in mare:
 *   1. deschide fisierul (atentie la MODUL de deschidere, vezi documentatia)
 *   2. afla cati bytes are
 *   3. verifica dimensiunea INAINTE sa scrii ceva in memorie
 *   4. citeste continutul la adresa corecta
 *   5. verifica daca s-a citit tot
 *   6. inchide fisierul pe TOATE drumurile, inclusiv la erori
 *
 * La fiecare eroare: mesaj pe stderr (fprintf(stderr, ...)) si return false.
 * ------------------------------------------------------------------------- */
bool chip8_load_rom(Chip8 *chip, const char *path)
{
    /* TODO */
    (void)chip;   /* sterge liniile astea cand implementezi functia */
    (void)path;
    return false;
}

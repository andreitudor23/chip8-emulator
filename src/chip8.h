/*
 * chip8.h - starea masinii CHIP-8 si interfata nucleului emulatorului
 *
 * REGULA DE ARHITECTURA: acest fisier si chip8.c NU includ SDL.
 * Nucleul e C pur, ca sa poata fi testat separat si portat mai tarziu (ESP32).
 */

#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------------
 * Constantele masinii (date, nu le modifica)
 * ------------------------------------------------------------------------- */
#define CHIP8_MEMORY_SIZE    4096    /* 4 KB de memorie                       */
#define CHIP8_START_ADDRESS  0x200   /* aici incepe programul (ROM-ul)        */
#define CHIP8_FONT_ADDRESS   0x050   /* aici se pune fontul 0-F               */
#define CHIP8_FONT_SIZE      80      /* 16 caractere x 5 bytes                */
#define CHIP8_NUM_REGISTERS  16      /* V0 ... VF                             */
#define CHIP8_STACK_SIZE     16      /* 16 niveluri de subrutine              */
#define CHIP8_NUM_KEYS       16      /* tastele 0 ... F                       */
#define CHIP8_DISPLAY_WIDTH  64
#define CHIP8_DISPLAY_HEIGHT 32

/* Dimensiunea maxima a unui ROM: tot ce incape de la 0x200 pana la final */
#define CHIP8_MAX_ROM_SIZE   (CHIP8_MEMORY_SIZE - CHIP8_START_ADDRESS)

/* Fontul 0-F, definit in chip8.c. Il folosesc si testele. */
extern const uint8_t CHIP8_FONTSET[CHIP8_FONT_SIZE];

/* ---------------------------------------------------------------------------
 * TODO 1: Starea masinii
 *
 * Defineste campurile structurii. Numele campurilor sunt fixe (le folosesc
 * testele si restul codului). TIPURILE le alegi tu: pentru fiecare camp,
 * cel mai mic tip din <stdint.h> care poate tine toate valorile posibile.
 *
 *   memory       memoria de 4 KB
 *   V            cele 16 registre de uz general
 *   I            registrul de adresa
 *   pc           program counter
 *   stack        stiva de adrese de intoarcere
 *   sp           stack pointer (cate adrese sunt pe stiva)
 *   delay_timer  timer-ul de intarziere
 *   sound_timer  timer-ul de sunet
 *   display      ecranul, ca matrice [CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH]
 *                (un element = un pixel, 0 = stins, 1 = aprins)
 *   keypad       starea celor 16 taste (0 = eliberata, 1 = apasata)
 *
 * Detaliile (cat de mare e fiecare valoare) sunt in docs/milestone1.md.
 * Pana nu completezi structura, proiectul nu compileaza. E normal.
 * ------------------------------------------------------------------------- */
typedef struct {

    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t V[CHIP8_NUM_REGISTERS];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t display[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
    uint8_t keypad[CHIP8_NUM_KEYS];

} Chip8;

/* ---------------------------------------------------------------------------
 * Interfata nucleului
 * ------------------------------------------------------------------------- */

/*
 * Aduce masina in starea de pornire. Vezi specificatia din docs/milestone1.md.
 * Trebuie sa functioneze corect si pe o structura care contine deja date.
 */
void chip8_init(Chip8 *chip);

/*
 * Incarca fisierul ROM de la `path` in memorie, incepand cu adresa 0x200.
 * Returneaza true la succes, false la orice eroare (fisier inexistent,
 * gol, prea mare, eroare de citire). La eroare afiseaza un mesaj pe stderr.
 */
bool chip8_load_rom(Chip8 *chip, const char *path);

#endif /* CHIP8_H */

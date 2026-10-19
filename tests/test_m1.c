/*
 * test_m1.c - testele pentru milestone 1 (date, nu le modifica)
 *
 * Ruleaza configuratia "test_m1" din CLion.
 * Milestone-ul e gata cand toate testele afiseaza [OK].
 */

#include "../src/chip8.h"
#include "test.h"

#include <stdio.h>
#include <string.h>

#define TMP_ROM "test_m1_tmp.ch8"

static Chip8 chip;

/* Scrie `size` bytes intr-un fisier temporar. Returneaza 1 la succes. */
static int write_file(const char *path, const uint8_t *data, size_t size)
{
    FILE *f = fopen(path, "wb");
    if (f == NULL) return 0;
    size_t written = size ? fwrite(data, 1, size, f) : 0;
    fclose(f);
    return written == size;
}

/* Verifica daca `size` bytes de la `p` sunt toti egali cu `value`. */
static int all_equal(const uint8_t *p, size_t size, uint8_t value)
{
    for (size_t i = 0; i < size; i++) {
        if (p[i] != value) return 0;
    }
    return 1;
}

#define COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

static void test_struct(void)
{
    SECTION("TODO 1: structura Chip8");

    CHECK(sizeof(chip.memory) == CHIP8_MEMORY_SIZE && sizeof(chip.memory[0]) == 1,
          "memory: 4096 elemente de cate 1 byte");

    CHECK(COUNT(chip.V) == CHIP8_NUM_REGISTERS && sizeof(chip.V[0]) == 1,
          "V: 16 registre de cate 1 byte");
    chip.V[0] = 0; chip.V[0]--;
    CHECK(chip.V[0] == 255, "V: fara semn (0 - 1 devine 255)");

    CHECK(sizeof(chip.I) == 2, "I: 2 bytes");
    chip.I = 0; chip.I--;
    CHECK(chip.I > 0, "I: fara semn");

    CHECK(sizeof(chip.pc) == 2, "pc: 2 bytes");
    chip.pc = 0; chip.pc--;
    CHECK(chip.pc > 0, "pc: fara semn");

    CHECK(COUNT(chip.stack) == CHIP8_STACK_SIZE && sizeof(chip.stack[0]) == 2,
          "stack: 16 elemente de cate 2 bytes");
    chip.stack[0] = 0; chip.stack[0]--;
    CHECK(chip.stack[0] > 0, "stack: fara semn");

    CHECK(sizeof(chip.sp) == 1, "sp: 1 byte");
    chip.sp = 0; chip.sp--;
    CHECK(chip.sp > 0, "sp: fara semn");

    CHECK(sizeof(chip.delay_timer) == 1 && sizeof(chip.sound_timer) == 1,
          "delay_timer si sound_timer: cate 1 byte");
    chip.delay_timer = 0; chip.delay_timer--;
    chip.sound_timer = 0; chip.sound_timer--;
    CHECK(chip.delay_timer == 255 && chip.sound_timer == 255,
          "delay_timer si sound_timer: fara semn");

    CHECK(COUNT(chip.display) == CHIP8_DISPLAY_HEIGHT &&
          COUNT(chip.display[0]) == CHIP8_DISPLAY_WIDTH &&
          sizeof(chip.display[0][0]) == 1,
          "display: matrice [32][64] de cate 1 byte");

    CHECK(COUNT(chip.keypad) == CHIP8_NUM_KEYS && sizeof(chip.keypad[0]) == 1,
          "keypad: 16 elemente de cate 1 byte");
}

static void test_init(void)
{
    SECTION("TODO 2: chip8_init");

    /* Umplem structura cu "gunoi", ca sa vedem daca init chiar curata tot. */
    memset(&chip, 0xAB, sizeof(chip));
    chip8_init(&chip);

    CHECK(memcmp(&chip.memory[CHIP8_FONT_ADDRESS], CHIP8_FONTSET, CHIP8_FONT_SIZE) == 0,
          "fontul e copiat la 0x050");
    CHECK(all_equal(chip.memory, CHIP8_FONT_ADDRESS, 0),
          "memoria de sub font (0x000-0x04F) e 0");
    CHECK(all_equal(&chip.memory[CHIP8_FONT_ADDRESS + CHIP8_FONT_SIZE],
                    CHIP8_MEMORY_SIZE - CHIP8_FONT_ADDRESS - CHIP8_FONT_SIZE, 0),
          "memoria de deasupra fontului (0x0A0-0xFFF) e 0");

    CHECK(chip.pc == CHIP8_START_ADDRESS, "pc = 0x200");
    CHECK(chip.I == 0, "I = 0");
    CHECK(all_equal(chip.V, sizeof(chip.V), 0), "V0-VF = 0");
    CHECK(chip.sp == 0, "sp = 0");
    CHECK(all_equal((const uint8_t *)chip.stack, sizeof(chip.stack), 0), "stiva = 0");
    CHECK(chip.delay_timer == 0 && chip.sound_timer == 0, "timerele = 0");
    CHECK(all_equal(&chip.display[0][0], sizeof(chip.display), 0), "ecranul e stins");
    CHECK(all_equal(chip.keypad, sizeof(chip.keypad), 0), "nicio tasta apasata");
}

static void test_load_rom(void)
{
    static uint8_t rom[CHIP8_MAX_ROM_SIZE + 1];
    bool ok;

    SECTION("TODO 3: chip8_load_rom");

    /* ROM normal, 300 de bytes */
    for (size_t i = 0; i < 300; i++) rom[i] = (uint8_t)(i * 7 + 3);
    write_file(TMP_ROM, rom, 300);
    chip8_init(&chip);
    ok = chip8_load_rom(&chip, TMP_ROM);
    CHECK(ok, "ROM de 300 bytes: returneaza true");
    CHECK(memcmp(&chip.memory[CHIP8_START_ADDRESS], rom, 300) == 0,
          "ROM de 300 bytes: continutul e la 0x200");
    CHECK(all_equal(&chip.memory[CHIP8_START_ADDRESS + 300],
                    CHIP8_MAX_ROM_SIZE - 300, 0),
          "ROM de 300 bytes: dupa ROM memoria ramane 0");
    CHECK(memcmp(&chip.memory[CHIP8_FONT_ADDRESS], CHIP8_FONTSET, CHIP8_FONT_SIZE) == 0,
          "ROM de 300 bytes: fontul ramane intact");
    CHECK(chip.pc == CHIP8_START_ADDRESS, "ROM de 300 bytes: pc ramane 0x200");

    /* Bytes care se strica daca fisierul e deschis in mod text pe Windows */
    const uint8_t special[] = { 0x12, 0x0D, 0x0A, 0x1A, 0x00, 0x0A, 0x0D, 0xFF };
    write_file(TMP_ROM, special, sizeof(special));
    chip8_init(&chip);
    ok = chip8_load_rom(&chip, TMP_ROM);
    CHECK(ok && memcmp(&chip.memory[CHIP8_START_ADDRESS], special, sizeof(special)) == 0,
          "ROM cu bytes 0x0D 0x0A 0x1A: citit exact (mod binar)");

    /* Exact dimensiunea maxima */
    for (size_t i = 0; i < CHIP8_MAX_ROM_SIZE; i++) rom[i] = (uint8_t)(i ^ 0x5A);
    write_file(TMP_ROM, rom, CHIP8_MAX_ROM_SIZE);
    chip8_init(&chip);
    ok = chip8_load_rom(&chip, TMP_ROM);
    CHECK(ok && memcmp(&chip.memory[CHIP8_START_ADDRESS], rom, CHIP8_MAX_ROM_SIZE) == 0,
          "ROM de 3584 bytes (maxim): incarcat complet, pana la 0xFFF");

    /* Cu un byte prea mare */
    write_file(TMP_ROM, rom, CHIP8_MAX_ROM_SIZE + 1);
    chip8_init(&chip);
    ok = chip8_load_rom(&chip, TMP_ROM);
    CHECK(!ok, "ROM de 3585 bytes (prea mare): returneaza false");
    CHECK(all_equal(&chip.memory[CHIP8_START_ADDRESS], CHIP8_MAX_ROM_SIZE, 0),
          "ROM prea mare: memoria NU e modificata");

    /* Fisier gol */
    write_file(TMP_ROM, rom, 0);
    chip8_init(&chip);
    CHECK(!chip8_load_rom(&chip, TMP_ROM), "fisier gol: returneaza false");

    /* Fisier inexistent */
    remove(TMP_ROM);
    chip8_init(&chip);
    CHECK(!chip8_load_rom(&chip, "nu_exista_acest_fisier.ch8"),
          "fisier inexistent: returneaza false (fara crash)");
}

int main(void)
{
    printf("=== Teste milestone 1 ===\n");
    printf("(mesajele de eroare de mai jos sunt asteptate: testele verifica si cazurile gresite)\n");

    test_struct();
    test_init();
    test_load_rom();

    remove(TMP_ROM);
    TEST_SUMMARY();
}

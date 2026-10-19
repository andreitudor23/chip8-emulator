/*
 * test.h - mini-framework de testare (dat, nu trebuie modificat)
 *
 * CHECK(conditie, "descriere") afiseaza [OK] sau [FAIL] si numara rezultatele.
 * TEST_SUMMARY() afiseaza totalul si da codul de iesire al programului.
 */

#ifndef TEST_H
#define TEST_H

#include <stdio.h>

static int tests_run    = 0;
static int tests_failed = 0;

#define CHECK(cond, desc)                                              \
    do {                                                               \
        tests_run++;                                                   \
        if (cond) {                                                    \
            printf("  [OK]   %s\n", desc);                             \
        } else {                                                       \
            tests_failed++;                                            \
            printf("  [FAIL] %s\n         (%s:%d)\n",                  \
                   desc, __FILE__, __LINE__);                          \
        }                                                              \
    } while (0)

#define SECTION(name) printf("\n%s\n", name)

#define TEST_SUMMARY()                                                 \
    do {                                                               \
        printf("\n%d/%d teste trecute\n",                              \
               tests_run - tests_failed, tests_run);                   \
        return tests_failed == 0 ? 0 : 1;                              \
    } while (0)

#endif /* TEST_H */

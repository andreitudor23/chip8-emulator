/*
 * Milestone 0 - test de setup
 *
 * Scopul acestui fisier: sa verifici ca CLion + CMake + SDL2 functioneaza.
 * Daca vezi o fereastra cu un model de pixeli si o poti inchide cu Esc,
 * setup-ul e gata si putem trece la milestone 1.
 *
 * Nu e nevoie sa intelegi tot codul acum. In milestone 1 primesti
 * documentatia pentru partea de SDL.
 */

#define SDL_MAIN_HANDLED   // folosim propriul main(), nu pe cel al SDL
#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH  64   // rezolutia CHIP-8
#define DISPLAY_HEIGHT 32
#define SCALE          10   // fiecare pixel CHIP-8 devine un patrat de 10x10

int main(void)
{
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Eroare SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "CHIP-8 - test setup",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE,
        0
    );
    if (window == NULL) {
        fprintf(stderr, "Eroare SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Eroare SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Ecranul CHIP-8: 1 = pixel aprins, 0 = stins.
    // Exact asa va arata si framebuffer-ul din emulator.
    uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {0};

    printf("SDL %d.%d.%d functioneaza. Apasa Esc ca sa inchizi.\n",
           SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

    bool running = true;
    uint32_t frame = 0;

    while (running) {
        // 1. Evenimente: inchiderea ferestrei si tastatura
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // 2. Model de test: o tabla de sah care se deplaseaza la fiecare 15 cadre
        uint32_t offset = frame / 15;
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                display[y][x] = ((x / 4 + y / 4 + offset) % 2 == 0);
            }
        }

        // 3. Desenare: fundal negru, pixelii aprinsi albi
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        for (int y = 0; y < DISPLAY_HEIGHT; y++) {
            for (int x = 0; x < DISPLAY_WIDTH; x++) {
                if (display[y][x]) {
                    SDL_Rect pixel = { x * SCALE, y * SCALE, SCALE, SCALE };
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }

        SDL_RenderPresent(renderer);

        // 4. ~60 de cadre pe secunda (aceeasi frecventa ca timerele CHIP-8)
        SDL_Delay(16);
        frame++;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

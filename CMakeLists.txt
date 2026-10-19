cmake_minimum_required(VERSION 3.16)
project(chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# SDL2: CMake il descarca si il compileaza automat la prima configurare.
# Nu trebuie instalat nimic manual. Prima compilare dureaza 1-3 minute.
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-2.32.10.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)   # legam SDL static: un singur .exe, fara DLL-uri
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

add_executable(chip8
    src/main.c
)

target_link_libraries(chip8 PRIVATE SDL2::SDL2-static)

# Avertismente stricte: compilatorul te ajuta sa prinzi bug-uri devreme
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(chip8 PRIVATE -Wall -Wextra)
endif()

# Pe Windows (MinGW) legam si runtime-ul static, ca .exe-ul sa mearga oriunde
if(MINGW)
    target_link_options(chip8 PRIVATE -static -static-libgcc)
endif()

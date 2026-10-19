cmake_minimum_required(VERSION 3.16)
project(chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# SDL2: CMake il descarca si il compileaza automat la prima configurare.
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-2.32.10.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

# Avertismente stricte, aplicate doar codului nostru (nu si lui SDL)
function(chip8_warnings target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
    if(MINGW)
        target_link_options(${target} PRIVATE -static -static-libgcc)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# Nucleul emulatorului: C pur, FARA SDL.
# Il folosesc atat programul, cat si testele.
# ---------------------------------------------------------------------------
add_library(chip8_core STATIC
    src/chip8.c
)
target_include_directories(chip8_core PUBLIC src)
chip8_warnings(chip8_core)

# ---------------------------------------------------------------------------
# Programul principal
# ---------------------------------------------------------------------------
add_executable(chip8
    src/main.c
)
target_link_libraries(chip8 PRIVATE chip8_core SDL2::SDL2-static)
chip8_warnings(chip8)

# ---------------------------------------------------------------------------
# Teste: nu depind de SDL, deci se compileaza si ruleaza rapid
# ---------------------------------------------------------------------------
enable_testing()

add_executable(test_m1 tests/test_m1.c)
target_link_libraries(test_m1 PRIVATE chip8_core)
chip8_warnings(test_m1)
add_test(NAME milestone1 COMMAND test_m1)

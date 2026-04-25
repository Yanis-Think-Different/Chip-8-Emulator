#ifndef CHIP8
#define CHIP8

#include <stdint.h>

#define MEMORY_SIZE 4096
#define LARGEUR 64
#define HAUTEUR 32

typedef struct STACK{
    uint16_t tab[16];
    uint8_t stack_pointer;
}stack;

typedef struct Chip8{
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[LARGEUR*HAUTEUR];
    uint16_t program_counter;
    uint16_t index_register;
    stack pile;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t variable[16];
}chip8_t;

typedef struct {
    uint16_t opcode; // L'instruction complète (ex: 0x8AB1)
    uint8_t  type;   // Le premier grignotage (0x8)
    uint8_t  x;      // Le deuxième (0xA)
    uint8_t  y;      // Le troisième (0xB)
    uint8_t  n;      // Le quatrième (0x1)
    uint8_t  nn;     // Les deux derniers (0xB1)
    uint16_t nnn;    // Les trois derniers (0xAB1)
} instruction_t;

void init_emulator(chip8_t *console);
uint16_t fetch(chip8_t *console);
instruction_t decode(uint16_t opcode);

#endif

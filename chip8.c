#include "chip8.h"
#include <assert.h>
#include <memory.h>
#include <stdint.h>

void init_emulator(chip8_t *console){
    console->delay_timer = 0;
    console->sound_timer = 0;
    console->index_register = 0;
    console->program_counter = 0x0200;
    console->pile.stack_pointer = 0;
    memset(console->memory, 0, MEMORY_SIZE);
    memset(console->display, 0, LARGEUR * HAUTEUR);
    memset(console->pile.tab, 0, 16);
    memset(console->variable, 0, 16);

    // Chargement de la FONT
    uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };
    memcpy(&console->memory[0x050], font, sizeof(font));
}

uint16_t fetch(chip8_t *console){
    assert(console->program_counter <= MEMORY_SIZE - 1);
    assert(console->program_counter % 2 == 0);

    uint16_t instruction_final = 0;

    uint16_t instruction1 = console->memory[console->program_counter];
    uint16_t instruction2 = console->memory[console->program_counter+1];
    console->program_counter += 2;

    instruction_final = (instruction1 << 8) | instruction2;
    return instruction_final;
}

instruction_t decode(uint16_t opcode){
    instruction_t instruction;

    instruction.opcode = opcode;
    instruction.type = (opcode >> 12);
    instruction.x = (opcode & 0x0F00) >> 8;
    instruction.y = (opcode & 0x00F0) >> 4;
    instruction.n = (opcode & 0x000F);
    instruction.nn = (opcode & 0x00FF);
    instruction.nnn = (opcode & 0x0FFF);

    return instruction;
}

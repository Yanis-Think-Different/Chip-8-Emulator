#include "chip8.h"
#include <assert.h>
#include <stdint.h>

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

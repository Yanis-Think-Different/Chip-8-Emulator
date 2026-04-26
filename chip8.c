#include "chip8.h"
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void init_emulator(chip8_t *console){
    console->delay_timer = 0x0;
    console->sound_timer = 0x0;
    console->index_register = 0x0;
    console->program_counter = 0x0200;
    console->pile.stack_pointer = 0x0;
    memset(console->memory, 0x0, MEMORY_SIZE);
    memset(console->display, 0x0, LARGEUR * HAUTEUR);
    memset(console->pile.tab, 0x0, 0x10);
    memset(console->V, 0x0, 0x10);
    memset(console->keypad, 0x0, 0x10);
    console->V[0xF] = 0x0;

    // Chargement de la FONT
    uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    memcpy(&console->memory[0x050], font, sizeof(font));
}

uint16_t fetch(chip8_t *console){
    assert(console->program_counter <= MEMORY_SIZE - 0x1);
    assert(console->program_counter % 0x2 == 0x0);

    uint16_t instruction_final = 0x0;

    uint16_t instruction1 = console->memory[console->program_counter];
    uint16_t instruction2 = console->memory[console->program_counter+1];
    console->program_counter += 0x2;

    instruction_final = (instruction1 << 0x8) | instruction2;
    return instruction_final;
}

instruction_t decode(uint16_t opcode){
    instruction_t instruction;

    instruction.opcode = opcode;
    instruction.type = (opcode >> 0xC);
    instruction.x = (opcode & 0x0F00) >> 0x8;
    instruction.y = (opcode & 0x00F0) >> 0x4;
    instruction.n = (opcode & 0x000F);
    instruction.nn = (opcode & 0x00FF);
    instruction.nnn = (opcode & 0x0FFF);

    return instruction;
}

void execute(instruction_t *instruction, chip8_t *console) {
    switch(instruction->type) {
        case 0x0:
            switch(instruction->nnn) {
                case 0x0E0:
                    memset(console->display, 0x0, LARGEUR*HAUTEUR);
                    break;
                case 0x0EE:
                    console->program_counter = console->pile.tab[console->pile.stack_pointer];
                    console->pile.stack_pointer--;
                    break;
                default:
                    break;
            }
            break;
        case 0x1:
            console->program_counter = instruction->nnn;
            break;
        case 0x2:
            console->pile.stack_pointer++;
            console->pile.tab[console->pile.stack_pointer] = console->program_counter;
            console->program_counter = instruction->nnn;
            break;
        case 0x3:
            if (console->V[instruction->x] == instruction->nn)
                console->program_counter += 0x2;
            break;
        case 0x4:
            if (console->V[instruction->x] != instruction->nn)
                console->program_counter += 0x2;
            break;
        case 0x5:
            if ((console->V[instruction->x] == console->V[instruction->y]) && (instruction->n == 0))
                console->program_counter += 0x2;
            break;
        case 0x6:
            console->V[instruction->x] = instruction->nn;
            break;
        case 0x7:
            console->V[instruction->x] += instruction->nn;
            break;
        case 0x8:
            switch (instruction->n) {
                case 0x0:
                    console->V[instruction->x] = console->V[instruction->y];
                    break;
                case 0x1:
                    console->V[instruction->x] |= console->V[instruction->y];
                    break;
                case 0x2:
                    console->V[instruction->x] &= console->V[instruction->y];
                    break;
                case 0x3:
                    console->V[instruction->x] ^= console->V[instruction->y];
                    break;
                case 0x4: {
                    uint16_t add = (uint16_t)console->V[instruction->x] + (uint16_t)console->V[instruction->y];
                    console->V[0xF] = (add > 0xFF) ? 0x1 : 0x0;
                    console->V[instruction->x] = (uint8_t)(add & 0xFF);
                    break;
                }
                case 0x5:
                    console->V[0xF] = (console->V[instruction->x] > console->V[instruction->y]) ? 0x1 : 0x0;
                    console->V[instruction->x] -= console->V[instruction->y];
                    break;
                case 0x6:
                    console->V[0xF] = console->V[instruction->x] & 0x1;
                    console->V[instruction->x] >>= 0x1;
                    break;
                case 0x7:
                    console->V[0xF] = (console->V[instruction->y] > console->V[instruction->x]) ? 0x1 : 0x0;
                    console->V[instruction->x] = console->V[instruction->y] - console->V[instruction->x];
                    break;
                case 0xE:
                    console->V[0xF] = (console->V[instruction->x] >> 0x7) & 0x1;
                    console->V[instruction->x] <<= 0x1;
                    break;
            }
            break;
        case 0x9:
            if ((console->V[instruction->x] != console->V[instruction->y]) && (instruction->n == 0x0))
                console->program_counter += 0x2;
            break;
        case 0xA:
            console->index_register = instruction->nnn;
            break;
        case 0xB:
            console->program_counter = instruction->nnn + console->V[0x0];
            break;
        case 0xC:
            console->V[instruction->x] = (rand() % 0x100) & instruction->nn;
            break;
        case 0xD: {
            uint8_t x_pos = console->V[instruction->x] % LARGEUR;
            uint8_t y_pos = console->V[instruction->y] % HAUTEUR;

            console->V[0xF] = 0x0;

            for (uint8_t i = 0x0; i < instruction->n; i++){
                uint8_t sprite = console->memory[console->index_register + i];
                for (uint8_t j = 0x0; j < 0x8; j++) {
                    uint8_t index = ((y_pos + i) * LARGEUR) + (x_pos + j);
                    uint8_t tmp = console->display[index];
                    console->display[index] ^= ((sprite >> (0x7-j)) & 0x1U);
                    if (tmp){
                        if (!console->display[index])
                            console->V[0xF] = 0x1;
                    }
                }
            }
            break;
        }
        case 0xE:
            switch(instruction->nn) {
                case 0x9E:
                    if (console->keypad[console->V[instruction->x]])
                        console->program_counter += 2;
                    break;
                case 0xA1:
                    if (!console->keypad[console->V[instruction->x]])
                        console->program_counter += 2;
                    break;
            }
            break;
        case 0xF:
            switch(instruction->nn) {
                case 0x07:
                    console->V[instruction->x] = console->delay_timer;
                    break;
                case 0x0A: {
                    uint8_t flag = 0x0;
                    for (uint8_t i = 0x0 ; i < 0x10; i++)
                        if (console->keypad[i]){
                            console->V[instruction->x] = i;
                            flag = 0x1;
                            break;
                        }
                    if (!flag)
                        console->program_counter -= 2;
                    break;
                }
                case 0x15:
                    console->delay_timer = console->V[instruction->x];
                    break;
                case 0x18:
                    console->sound_timer = console->V[instruction->x];
                    break;
                case 0x1E:
                    console->index_register += console->V[instruction->x];
                    break;
                case 0x29:
                    console->index_register = 0x050 + console->V[instruction->x] * 5;
                    break;
                case 0x33: {
                    uint8_t val = console->V[instruction->x];
                    console->memory[console->index_register] = val / 100;
                    console->memory[console->index_register + 1] = (val / 10) % 10;
                    console->memory[console->index_register + 2] = val % 10;
                    break;
                }
                case 0x55:
                    for (uint8_t i = 0x0; i <= instruction->x; i++)
                        console->memory[console->index_register + i] = console->V[i];
                    break;
                case 0x65:
                    for (uint8_t i = 0x0; i <= instruction->x; i++)
                        console->V[i] = console->memory[console->index_register + i];
                    break;
            }
            break;
        default:
            break;
    }
}

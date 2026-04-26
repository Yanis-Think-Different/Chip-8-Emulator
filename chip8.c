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
    memset(console->V, 0, 16);

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

void execute(instruction_t instruction){
    switch(instruction.type){
        case 0x0:
            switch(instruction.nnn){
                case 0x0E0:
                    //Clear the display
                    break;
                case 0x0EE:
                    //Return from a subroutine. The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
                    break;
                default:
                    //Jump to a machine code routine at nnn
                    break;
            }
            break;
        case 0x1:
            // The interpreter sets the program counter to nnn.
            break;
        case 0x2:
            // Call subroutine at nnn. The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            break;
        case 0x3:
            // Skip next instruction if Vx = kk. The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            break;
        case 0x4:
            // Skip next instruction if Vx != kk. The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            break;
        case 0x5:
            // Skip next instruction if Vx = Vy.The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            break;
        case 0x6:
            // Set Vx = kk. The interpreter puts the value kk into register Vx.
            break;
        case 0x7:
            // Set Vx = Vx + kk. Adds the value kk to the value of register Vx, then stores the result in Vx.
            break;
        case 0x8:
            switch (instruction.n) {
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
                case 0xE:
                    break;
            }
            break;
        case 0x9:
            if (instruction.n == 0)
            break;
        case 0xA:
            // Set I = nnn. The value of register I is set to nnn.
            break;
        case 0xB:
            // Jump to location nnn + V0. The program counter is set to nnn plus the value of V0.
            break;
        case 0xC:
            // Set Vx = random byte AND kk. The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx.
            break;
        case 0xD:
            // lire la docu
            break;
        case 0xE:
            switch(instruction.nn){
                case 0x9E:
                    // kip next instruction if key with the value of Vx is pressed.Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
                    break;
                case 0xA1:
                    // Skip next instruction if key with the value of Vx is not pressed.Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
                    break;
                default :
                    break;
            }
            break;
        case 0xF:
            switch(instruction.nn){
                case 0x07:
                    // Set Vx = delay timer value. The value of DT is placed into Vx.
                    break;
                case 0x0A:
                    // Wait for a key press, store the value of the key in Vx.  execution stops until a key is pressed, then the value of that key is stored in Vx.
                    break;
                case 0x15:
                    // Set delay timer = Vx. DT is set equal to the value of Vx.
                    break;
                case 0x18:
                    // Set sound timer = Vx. ST is set equal to the value of Vx.
                    break;
                case 0x1E:
                    // Set I = I + Vx. The values of I and Vx are added, and the results are stored in I.
                    break;
                case 0x29:
                    // Set I = location of sprite for digit Vx. The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
                    break;
                case 0x33:
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2. The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
                    break;
                case 0x55:
                    // Store registers V0 through Vx in memory starting at location I. The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
                    break;
                case 0x65:
                    // Read registers V0 through Vx from memory starting at location I. The interpreter reads values from memory starting at location I into registers V0 through Vx.
                    break;
            }
            break;
        default :
            break;
    }
}

#include "chip8.h"
#include <assert.h>

void test_fetch() {
    chip8_t console;

    console.program_counter = 0x200;
    console.memory[0x200] = 0xA2;
    console.memory[0x201] = 0xF0;

    uint16_t result = fetch(&console);
    assert(result == 0xA2F0);
    assert(console.program_counter == 0x202);
    console.program_counter = 0x2095;
    console.program_counter = 0x0201;
    fetch(&console);
}

int main(void) {
    test_fetch();
    return 0;
}

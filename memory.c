#include <stdint.h>
#include "inc/memory.h"

uint8_t read_memory(uint16_t address) {
    return memory[address];
}

void write_memory(uint16_t address, uint8_t value) {
    memory[address] = value;
}

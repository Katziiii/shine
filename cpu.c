#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/bus.h"

uint8_t A,B,C,D,E,H,L,F ;
uint16_t SP ;
uint16_t PC = 0x100;

uint8_t opcode ;

uint8_t get_opcode() {
    return read_memory (PC++);
}

void cpu_init() {
}

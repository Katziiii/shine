#pragma once

#include <stdint.h>

#define BUFFER 2*1024*1024

extern uint8_t memory[BUFFER];


uint8_t read_memory(uint16_t address) ;
void write_memory(uint16_t address, uint8_t value) ;

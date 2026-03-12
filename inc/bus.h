#pragma once

#include <stdint.h>

uint8_t bus_read(uint16_t address);
void bus_write(uint16_t address, uint8_t value);
uint8_t read_memory(uint16_t address);
void write_memory(uint16_t address, uint8_t value);

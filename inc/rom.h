#pragma once

#include <stdint.h>

#define BUFFER 2*1024*1024

extern uint8_t rom_buffer[BUFFER];

extern char title[10];
extern char region[10];
extern char rom_type[40];
extern char publisher[40];

extern uint8_t version;

void load_rom(char *path);
void get_rom_info();

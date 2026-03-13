#include <stdint.h>
#include "inc/memory.h"

uint8_t vram[0x2000];
uint8_t eram[0x2000];
uint8_t wram[0x2000];
uint8_t oam[0xA0];
uint8_t io[0x80];
uint8_t hram[0x7F];
uint8_t ie;

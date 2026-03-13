#pragma once

#include <stdint.h>

extern uint8_t vram[0x2000];   /* 0x8000–0x9FFF */
extern uint8_t eram[0x2000];   /* 0xA000–0xBFFF */
extern uint8_t wram[0x2000];   /* 0xC000–0xDFF */
extern uint8_t oam[0xA0];      /* 0xFE00–0xFE9F */
extern uint8_t io[0x80];       /* 0xFF00–0xFF7F */
extern uint8_t hram[0x7F];     /* 0xFF80–0xFFFE */
extern uint8_t ie;             /* 0xFFFF  */

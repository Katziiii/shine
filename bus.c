#include <stdint.h>
#include "inc/bus.h"
#include "inc/rom.h"
#include "inc/memory.h"

uint8_t bus_read(uint16_t addr) {
    if (addr < 0x8000) {
        /* 0x0000–0x7FFF: ROM (cartridge) */
        return rom_buffer[addr];
    }
    if (addr < 0xA000) {
        /* 0x8000–0x9FFF: VRAM */
        return vram[addr - 0x8000];
    }
    if (addr < 0xC000) {
        /* 0xA000–0xBFFF: External/cartridge RAM */
        return eram[addr - 0xA000];
    }
    if (addr < 0xE000) {
        /* 0xC000–0xDFFF: Work RAM */
        return wram[addr - 0xC000];
    }
    if (addr < 0xFE00) {
        /* 0xE000–0xFDFF: Echo RAM — mirrors wram */
        return wram[addr - 0xE000];
    }
    if (addr < 0xFEA0) {
        /* 0xFE00–0xFE9F: OAM */
        return oam[addr - 0xFE00];
    }
    if (addr < 0xFF00) {
        /* 0xFEA0–0xFEFF: Prohibited — returns 0xFF on real hardware */
        return 0xFF;
    }
    if (addr < 0xFF80) {
        /* 0xFF00–0xFF7F: I/O registers */
        return io[addr - 0xFF00];
    }
    if (addr < 0xFFFF) {
        /* 0xFF80–0xFFFE: High RAM */
        return hram[addr - 0xFF80];
    }
    /* 0xFFFF: Interrupt Enable register */
    return ie;
}

void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        /* 0x0000–0x7FFF: ROM — writes ignored (no MBC yet) */
        return;
    }
    if (addr < 0xA000) {
        vram[addr - 0x8000] = value;
        return;
    }
    if (addr < 0xC000) {
        eram[addr - 0xA000] = value;
        return;
    }
    if (addr < 0xE000) {
        wram[addr - 0xC000] = value;
        return;
    }
    if (addr < 0xFE00) {
        /* Echo RAM — mirror write to wram */
        wram[addr - 0xE000] = value;
        return;
    }
    if (addr < 0xFEA0) {
        oam[addr - 0xFE00] = value;
        return;
    }
    if (addr < 0xFF00) {
        /* Prohibited region — writes ignored */
        return;
    }
    if (addr < 0xFF80) {
        io[addr - 0xFF00] = value;
        return;
    }
    if (addr < 0xFFFF) {
        hram[addr - 0xFF80] = value;
        return;
    }
    ie = value;
}

#include "inc/bus.h"
#include <stdio.h>
#include <stdio.h>
#include "inc/cpu.h"


uint8_t  A, B, C, D, E, H, L, F;
uint16_t SP ;
uint8_t opcode ;

uint16_t PC = 0x100;

uint8_t get_opcode() {
    return read_memory (PC++);
}

uint8_t LD8_RR (uint8_t *reg1 ,uint8_t reg2) {
    /*
     *     0 x40  LD B,B      0x41  LD B,C      0x42  LD B,D      0x43  LD B,E
     *     0x44  LD B,H      0x45  LD B,L      0x47  LD B,A
     *
     *     0x48  LD C,B      0x49  LD C,C      0x4A  LD C,D      0x4B  LD C,E
     *     0x4C  LD C,H      0x4D  LD C,L      0x4F  LD C,A
     *
     *     0x50  LD D,B      0x51  LD D,C      0x52  LD D,D      0x53  LD D,E
     *     0x54  LD D,H      0x55  LD D,L      0x57  LD D,A
     *
     *     0x58  LD E,B      0x59  LD E,C      0x5A  LD E,D      0x5B  LD E,E
     *     0x5C  LD E,H      0x5D  LD E,L      0x5F  LD E,A
     *
     *     0x60  LD H,B      0x61  LD H,C      0x62  LD H,D      0x63  LD H,E
     *     0x64  LD H,H      0x65  LD H,L      0x67  LD H,A
     *
     *     0x68  LD L,B      0x69  LD L,C      0x6A  LD L,D      0x6B  LD L,E
     *     0x6C  LD L,H      0x6D  LD L,L      0x6F  LD L,A
     *
     *     0x78  LD A,B      0x79  LD A,C      0x7A  LD A,D      0x7B  LD A,E
     *     0x7C  LD A,H      0x7D  LD A,L      0x7F  LD A,A
     */
    return *reg1 = reg2 ;
}

uint8_t LD8_RN (uint8_t *reg) {
    /*
     *     0 x06  LD B,d8
     *     0x0E  LD C,d8
     *     0x16  LD D,d8
     *     0x1E  LD E,d8
     *     0x26  LD H,d8
     *     0x2E  LD L,d8
     *     0x3E  LD A,d8
     */
    return *reg = read_memory(PC++);
}


uint16_t LD16_RN(uint8_t *hi, uint8_t *lo) {
    /*
     *     0 x01 LD BC,n16
     *     0x11  LD DE,n16
     *     0x21  LD HL,n16
     *     0x31  LD SP,n16
     */
    uint8_t lo_val = read_memory(PC++);
    uint8_t hi_val = read_memory(PC++);
    *hi = hi_val;
    *lo = lo_val;
    return (*hi << 8) | *lo;
}

void LD_HL_R(uint8_t *reg) {
    /*
     *    0x70  LD (HL),B
     *    0x71  LD (HL),C
     *    0x72  LD (HL),D
     *    0x73  LD (HL),E
     *    0x74  LD (HL),H
     *    0x75  LD (HL),L
     *    0x77  LD (HL),A
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, *reg);
}

void LD_HL_N() {
    /*
     *     0 x36  LD (HL),d8*
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, read_memory(PC++));
}

uint8_t LD_R_HL() {
    /*
     *   0x46  LD B,(HL)
     *   0x4E  LD C,(HL)
     *   0x56  LD D,(HL)
     *   0x5E  LD E,(HL)
     *   0x66  LD H,(HL)
     *   0x6E  LD L,(HL)
     *   0x7E  LD A,(HL)
     */
    uint16_t addr = (H << 8) | L;
    return read_memory(addr);
}

void LD_R16_A(uint8_t hi, uint8_t lo) {
    /*
     *   0x02  LD (BC),A
     *   0x12  LD (DE),A
     */
    uint16_t addr = (hi << 8) | lo;
    write_memory(addr, A);
}

void LD_N16_A() {
    /*
     *   0xEA  LD (a16),A
     */
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    uint16_t addr = (hi << 8) | lo;
    write_memory(addr, A);
}

void LDH_N16_A() {
    /*
     *   0xE0  LDH (a8),A
     */
    uint16_t addr = 0xFF00 | read_memory(PC++);
    write_memory(addr, A);
}

void LDH_C_A() {
    /*
     *   0xE2  LD (C),A
     */
    uint16_t addr = 0xFF00 | C;
    write_memory(addr, A);
}

uint8_t LD_A_R16(uint8_t hi, uint8_t lo) {
    /*
     *   0x0A  LD A,(BC)
     *   0x1A  LD A,(DE)
     */
    uint16_t addr = (hi << 8) | lo;
    return read_memory(addr);
}

uint8_t LD_A_N16() {
    /*
     *   0xFA  LD A,(a16)
     */
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    uint16_t addr = (hi << 8) | lo;
    return read_memory(addr);
}

uint8_t LDH_A_N16() {
    /*
     *   0xF0  LDH A,(a8)
     */
    uint16_t addr = 0xFF00 | read_memory(PC++);
    return read_memory(addr);
}

uint8_t LDH_A_C() {
    /*
     *   0xF2  LD A,(C)
     */
    uint16_t addr = 0xFF00 | C;
    return read_memory(addr);
}

void LD_HLI_A() {
    /*
     *   0x22  LD (HL+),A
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, A);
    uint16_t hl = addr + 1;
    H = hl >> 8;
    L = hl & 0xFF;
}

void LD_HLD_A() {
    /*
     *   0x32  LD (HL-),A
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, A);
    uint16_t hl = addr - 1;
    H = hl >> 8;
    L = hl & 0xFF;
}

uint8_t LD_A_HLD() {
    /*
     *   0x3A  LD A,(HL-)
     */
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    uint16_t hl = addr - 1;
    H = hl >> 8;
    L = hl & 0xFF;
    return val;
}

uint8_t LD_A_HLI() {
    /*
     *   0x2A  LD A,(HL+)
     */
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    uint16_t hl = addr + 1;
    H = hl >> 8;
    L = hl & 0xFF;
    return val;
}

void LD_N16_SP() {
    /*
     *   0x08  LD (a16),SP
     */
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    uint16_t addr = (hi << 8) | lo;
    write_memory(addr,     SP & 0xFF);
    write_memory(addr + 1, SP >> 8);
}

void LD_HL_SP_E8() {
    /*
     *   0xF8  LD HL,SP+r8
     */
    int8_t e8 = (int8_t)read_memory(PC++);
    uint16_t result = SP + e8;
    F = 0;
    if (((SP ^ e8 ^ result) & 0x10))  F |= FLAG_H;
    if (((SP ^ e8 ^ result) & 0x100)) F |= FLAG_C;
    H = result >> 8;
    L = result & 0xFF;
}

void LD_SP_HL() {
    /*
     *   0xF9  LD SP,HL
     */
    SP = (H << 8) | L;
}

void cpu_exec () {
    while (1) {
        opcode = get_opcode();
        switch (opcode) {

            case 0x06: LD8_RN(&B); break;
            case 0x0E: LD8_RN(&C); break;
            case 0x16: LD8_RN(&D); break;
            case 0x1E: LD8_RN(&E); break;
            case 0x26: LD8_RN(&H); break;
            case 0x2E: LD8_RN(&L); break;
            case 0x3E: LD8_RN(&A); break;

            case 0x01: LD16_RN(&B, &C); break;
            case 0x11: LD16_RN(&D, &E); break;
            case 0x21: LD16_RN(&H, &L); break;
            case 0x31: LD16_RN((uint8_t*)&SP + 1, (uint8_t*)&SP); break;

            case 0x36: LD_HL_N(); break;

            case 0x70: LD_HL_R(&B); break;
            case 0x71: LD_HL_R(&C); break;
            case 0x72: LD_HL_R(&D); break;
            case 0x73: LD_HL_R(&E); break;
            case 0x74: LD_HL_R(&H); break;
            case 0x75: LD_HL_R(&L); break;
            case 0x77: LD_HL_R(&A); break;

            case 0x46: B = LD_R_HL(); break;
            case 0x4E: C = LD_R_HL(); break;
            case 0x56: D = LD_R_HL(); break;
            case 0x5E: E = LD_R_HL(); break;
            case 0x66: H = LD_R_HL(); break;
            case 0x6E: L = LD_R_HL(); break;
            case 0x7E: A = LD_R_HL(); break;

            case 0x40: LD8_RR(&B, B); break;
            case 0x41: LD8_RR(&B, C); break;
            case 0x42: LD8_RR(&B, D); break;
            case 0x43: LD8_RR(&B, E); break;
            case 0x44: LD8_RR(&B, H); break;
            case 0x45: LD8_RR(&B, L); break;
            case 0x47: LD8_RR(&B, A); break;

            case 0x48: LD8_RR(&C, B); break;
            case 0x49: LD8_RR(&C, C); break;
            case 0x4A: LD8_RR(&C, D); break;
            case 0x4B: LD8_RR(&C, E); break;
            case 0x4C: LD8_RR(&C, H); break;
            case 0x4D: LD8_RR(&C, L); break;
            case 0x4F: LD8_RR(&C, A); break;

            case 0x50: LD8_RR(&D, B); break;
            case 0x51: LD8_RR(&D, C); break;
            case 0x52: LD8_RR(&D, D); break;
            case 0x53: LD8_RR(&D, E); break;
            case 0x54: LD8_RR(&D, H); break;
            case 0x55: LD8_RR(&D, L); break;
            case 0x57: LD8_RR(&D, A); break;

            case 0x58: LD8_RR(&E, B); break;
            case 0x59: LD8_RR(&E, C); break;
            case 0x5A: LD8_RR(&E, D); break;
            case 0x5B: LD8_RR(&E, E); break;
            case 0x5C: LD8_RR(&E, H); break;
            case 0x5D: LD8_RR(&E, L); break;
            case 0x5F: LD8_RR(&E, A); break;

            case 0x60: LD8_RR(&H, B); break;
            case 0x61: LD8_RR(&H, C); break;
            case 0x62: LD8_RR(&H, D); break;
            case 0x63: LD8_RR(&H, E); break;
            case 0x64: LD8_RR(&H, H); break;
            case 0x65: LD8_RR(&H, L); break;
            case 0x67: LD8_RR(&H, A); break;

            case 0x68: LD8_RR(&L, B); break;
            case 0x69: LD8_RR(&L, C); break;
            case 0x6A: LD8_RR(&L, D); break;
            case 0x6B: LD8_RR(&L, E); break;
            case 0x6C: LD8_RR(&L, H); break;
            case 0x6D: LD8_RR(&L, L); break;
            case 0x6F: LD8_RR(&L, A); break;

            case 0x78: LD8_RR(&A, B); break;
            case 0x79: LD8_RR(&A, C); break;
            case 0x7A: LD8_RR(&A, D); break;
            case 0x7B: LD8_RR(&A, E); break;
            case 0x7C: LD8_RR(&A, H); break;
            case 0x7D: LD8_RR(&A, L); break;
            case 0x7F: LD8_RR(&A, A); break;

            case 0x02: LD_R16_A(B, C); break;
            case 0x12: LD_R16_A(D, E); break;

            case 0xEA: LD_N16_A(); break;

            case 0xE0: LDH_N16_A(); break;

            case 0xE2: LDH_C_A(); break;

            case 0x0A: A = LD_A_R16(B, C); break;
            case 0x1A: A = LD_A_R16(D, E); break;

            case 0xFA: A = LD_A_N16(); break;

            case 0xF0: A = LDH_A_N16(); break;

            case 0xF2: A = LDH_A_C(); break;

            case 0x22: LD_HLI_A(); break;

            case 0x32: LD_HLD_A(); break;

            case 0x3A: A = LD_A_HLD(); break;

            case 0x2A: A = LD_A_HLI(); break;

            case 0x08: LD_N16_SP(); break;

            case 0xF8: LD_HL_SP_E8(); break;

            case 0xF9: LD_SP_HL(); break;

            default:
                printf("Unknown opcode: 0x%02X @ PC=0x%04X\n", opcode, PC - 1);
                break;
        }
    }
}

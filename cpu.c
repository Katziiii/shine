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

void ADD_R(uint8_t reg) {
    /*
     *   0x80  ADD A,B
     *   0x81  ADD A,C
     *   0x82  ADD A,D
     *   0x83  ADD A,E
     *   0x84  ADD A,H
     *   0x85  ADD A,L
     *   0x87  ADD A,A
     */
    uint16_t result = A + reg;
    F = 0;
    if ((uint8_t)result == 0)        F |= FLAG_Z;
    if ((A & 0xF) + (reg & 0xF) > 0xF) F |= FLAG_H;
    if (result > 0xFF)               F |= FLAG_C;
    A = (uint8_t)result;
}

// ADD A,(HL)
void ADD_HL() {
    /*
     *   0x86  ADD A,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    uint16_t result = A + val;
    F = 0;
    if ((uint8_t)result == 0)          F |= FLAG_Z;
    if ((A & 0xF) + (val & 0xF) > 0xF) F |= FLAG_H;
    if (result > 0xFF)                 F |= FLAG_C;
    A = (uint8_t)result;
}

void ADD_N() {
    /*
     *   0xC6  ADD A,d8
     */
    uint8_t val = read_memory(PC++);
    uint16_t result = A + val;
    F = 0;
    if ((uint8_t)result == 0)          F |= FLAG_Z;
    if ((A & 0xF) + (val & 0xF) > 0xF) F |= FLAG_H;
    if (result > 0xFF)                 F |= FLAG_C;
    A = (uint8_t)result;
}

void ADC_R(uint8_t reg) {
    /*
     *   0x88  ADC A,B
     *   0x89  ADC A,C
     *   0x8A  ADC A,D
     *   0x8B  ADC A,E
     *   0x8C  ADC A,H
     *   0x8D  ADC A,L
     *   0x8F  ADC A,A
     */
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A + reg + carry;
    F = 0;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) + (reg & 0xF) + carry > 0xF)  F |= FLAG_H;
    if (result > 0xFF)                           F |= FLAG_C;
    A = (uint8_t)result;
}

void ADC_HL() {
    /*
     *   0x8E  ADC A,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A + val + carry;
    F = 0;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) + (val & 0xF) + carry > 0xF)  F |= FLAG_H;
    if (result > 0xFF)                           F |= FLAG_C;
    A = (uint8_t)result;
}

void ADC_N() {
    /*
     *   0xCE  ADC A,d8
     */
    uint8_t val = read_memory(PC++);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A + val + carry;
    F = 0;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) + (val & 0xF) + carry > 0xF)  F |= FLAG_H;
    if (result > 0xFF)                           F |= FLAG_C;
    A = (uint8_t)result;
}

void SUB_R(uint8_t reg) {
    /*
     *   0x90  SUB A,B
     *   0x91  SUB A,C
     *   0x92  SUB A,D
     *   0x93  SUB A,E
     *   0x94  SUB A,H
     *   0x95  SUB A,L
     *   0x97  SUB A,A
     */
    uint16_t result = A - reg;
    F = FLAG_N;
    if ((uint8_t)result == 0)        F |= FLAG_Z;
    if ((A & 0xF) < (reg & 0xF))    F |= FLAG_H;
    if (A < reg)                     F |= FLAG_C;
    A = (uint8_t)result;
}

void SUB_HL() {
    /*
     *   0x96  SUB A,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    uint16_t result = A - val;
    F = FLAG_N;
    if ((uint8_t)result == 0)        F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF))    F |= FLAG_H;
    if (A < val)                     F |= FLAG_C;
    A = (uint8_t)result;
}

void SUB_N() {
    /*
     *   0xD6  SUB A,d8
     */
    uint8_t val = read_memory(PC++);
    uint16_t result = A - val;
    F = FLAG_N;
    if ((uint8_t)result == 0)        F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF))    F |= FLAG_H;
    if (A < val)                     F |= FLAG_C;
    A = (uint8_t)result;
}

void SBC_R(uint8_t reg) {
    /*
     *   0x98  SBC A,B
     *   0x99  SBC A,C
     *   0x9A  SBC A,D
     *   0x9B  SBC A,E
     *   0x9C  SBC A,H
     *   0x9D  SBC A,L
     *   0x9F  SBC A,A
     */
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A - reg - carry;
    F = FLAG_N;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) < (reg & 0xF) + carry)        F |= FLAG_H;
    if (A < reg + carry)                         F |= FLAG_C;
    A = (uint8_t)result;
}

void SBC_HL() {
    /*
     *   0x9E  SBC A,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A - val - carry;
    F = FLAG_N;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF) + carry)        F |= FLAG_H;
    if (A < val + carry)                         F |= FLAG_C;
    A = (uint8_t)result;
}

void SBC_N() {
    /*
     *   0xDE  SBC A,d8
     */
    uint8_t val = read_memory(PC++);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    uint16_t result = A - val - carry;
    F = FLAG_N;
    if ((uint8_t)result == 0)                    F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF) + carry)        F |= FLAG_H;
    if (A < val + carry)                         F |= FLAG_C;
    A = (uint8_t)result;
}

void CP_R(uint8_t reg) {
    /*
     *   0xB8  CP A,B
     *   0xB9  CP A,C
     *   0xBA  CP A,D
     *   0xBB  CP A,E
     *   0xBC  CP A,H
     *   0xBD  CP A,L
     *   0xBF  CP A,A
     */
    F = FLAG_N;
    if (A == reg)             F |= FLAG_Z;
    if ((A & 0xF) < (reg & 0xF)) F |= FLAG_H;
    if (A < reg)              F |= FLAG_C;
}

void CP_HL() {
    /*
     *   0xBE  CP A,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    F = FLAG_N;
    if (A == val)             F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF)) F |= FLAG_H;
    if (A < val)              F |= FLAG_C;
}

void CP_N() {
    /*
     *   0xFE  CP A,d8
     */
    uint8_t val = read_memory(PC++);
    F = FLAG_N;
    if (A == val)             F |= FLAG_Z;
    if ((A & 0xF) < (val & 0xF)) F |= FLAG_H;
    if (A < val)              F |= FLAG_C;
}

void INC_R(uint8_t *reg) {
    /*
     *   0x04  INC B
     *   0x0C  INC C
     *   0x14  INC D
     *   0x1C  INC E
     *   0x24  INC H
     *   0x2C  INC L
     *   0x3C  INC A
     */
    F &= FLAG_C;
    if ((*reg & 0xF) == 0xF) F |= FLAG_H;
    *reg += 1;
    if (*reg == 0)           F |= FLAG_Z;
}

void INC_HL() {
    /*
     *   0x34  INC (HL)
     */
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F &= FLAG_C;
    if ((val & 0xF) == 0xF) F |= FLAG_H;
    val += 1;
    if (val == 0)           F |= FLAG_Z;
    write_memory(addr, val);
}

void DEC_R(uint8_t *reg) {
    /*
     *   0x05  DEC B
     *   0x0D  DEC C
     *   0x15  DEC D
     *   0x1D  DEC E
     *   0x25  DEC H
     *   0x2D  DEC L
     *   0x3D  DEC A
     */
    F &= FLAG_C;
    F |= FLAG_N;
    if ((*reg & 0xF) == 0x0) F |= FLAG_H;
    *reg -= 1;
    if (*reg == 0)           F |= FLAG_Z;
}

void DEC_HL() {
    /*
     *   0x35  DEC (HL)
     */
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F &= FLAG_C;
    F |= FLAG_N;
    if ((val & 0xF) == 0x0) F |= FLAG_H;
    val -= 1;
    if (val == 0)           F |= FLAG_Z;
    write_memory(addr, val);
}

void ADD_HL_R16(uint8_t hi, uint8_t lo) {
    /*
     *   0x09  ADD HL,BC
     *   0x19  ADD HL,DE
     *   0x29  ADD HL,HL
     *   0x39  ADD HL,SP
     */
    uint16_t hl  = (H << 8) | L;
    uint16_t r16 = (hi << 8) | lo;
    uint32_t result = hl + r16;
    F &= FLAG_Z;
    if ((hl & 0xFFF) + (r16 & 0xFFF) > 0xFFF) F |= FLAG_H;
    if (result > 0xFFFF)                       F |= FLAG_C;
    H = (result >> 8) & 0xFF;
    L = result & 0xFF;
}

void INC_R16(uint8_t *hi, uint8_t *lo) {
    /*
     *   0x03  INC BC
     *   0x13  INC DE
     *   0x23  INC HL
     *   0x33  INC SP
     */
    uint16_t val = (*hi << 8) | *lo;
    val++;
    *hi = val >> 8;
    *lo = val & 0xFF;
}

void DEC_R16(uint8_t *hi, uint8_t *lo) {
    /*
     *   0x0B  DEC BC
     *   0x1B  DEC DE
     *   0x2B  DEC HL
     *   0x3B  DEC SP
     */
    uint16_t val = (*hi << 8) | *lo;
    val--;
    *hi = val >> 8;
    *lo = val & 0xFF;
}

void AND_R(uint8_t reg) {
    /*
     *   0xA0  AND A,B
     *   0xA1  AND A,C
     *   0xA2  AND A,D
     *   0xA3  AND A,E
     *   0xA4  AND A,H
     *   0xA5  AND A,L
     *   0xA7  AND A,A
     */
    A &= reg;
    F = FLAG_H;
    if (A == 0) F |= FLAG_Z;
}

void AND_HL() {
    /*
     *   0xA6  AND A,(HL)
     */
    A &= read_memory((H << 8) | L);
    F = FLAG_H;
    if (A == 0) F |= FLAG_Z;
}

void AND_N() {
    /*
     *   0xE6  AND A,d8
     */
    A &= read_memory(PC++);
    F = FLAG_H;
    if (A == 0) F |= FLAG_Z;
}

void OR_R(uint8_t reg) {
    /*
     *   0xB0  OR A,B
     *   0xB1  OR A,C
     *   0xB2  OR A,D
     *   0xB3  OR A,E
     *   0xB4  OR A,H
     *   0xB5  OR A,L
     *   0xB7  OR A,A
     */
    A |= reg;
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void OR_HL() {
    /*
     *   0xB6  OR A,(HL)
     */
    A |= read_memory((H << 8) | L);
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void OR_N() {
    /*
     *   0xF6  OR A,d8
     */
    A |= read_memory(PC++);
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void XOR_R(uint8_t reg) {
    /*
     *   0xA8  XOR A,B
     *   0xA9  XOR A,C
     *   0xAA  XOR A,D
     *   0xAB  XOR A,E
     *   0xAC  XOR A,H
     *   0xAD  XOR A,L
     *   0xAF  XOR A,A
     */
    A ^= reg;
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void XOR_HL() {
    /*
     *   0xAE  XOR A,(HL)
     */
    A ^= read_memory((H << 8) | L);
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void XOR_N() {
    /*
     *   0xEE  XOR A,d8
     */
    A ^= read_memory(PC++);
    F = 0;
    if (A == 0) F |= FLAG_Z;
}

void CPL() {
    /*
     *   0x2F  CPL
     */
    A = ~A;
    F |= FLAG_N | FLAG_H;
}

void NOP() {
    /*
     *   0x00  NOP
     */
}

void STOP() {
    /*
     *   0x10  STOP
     */
    PC++; // skip next byte (always 0x00)
}

void DAA() {
    /*
     *   0x27  DAA
     */
    uint8_t correction = 0;
    uint8_t carry = 0;

    if (F & FLAG_H || (!(F & FLAG_N) && (A & 0xF) > 9))
        correction |= 0x06;

    if (F & FLAG_C || (!(F & FLAG_N) && A > 0x99)) {
        correction |= 0x60;
        carry = 1;
    }

    A += (F & FLAG_N) ? -correction : correction;

    F &= ~(FLAG_H | FLAG_Z | FLAG_C);
    if (A == 0)  F |= FLAG_Z;
    if (carry)   F |= FLAG_C;
}


void BIT_R(uint8_t bit, uint8_t reg) {
    /*
     *   0x40-0x7F  BIT b,r (CB prefix)
     *
     *   0x40  BIT 0,B    0x41  BIT 0,C    0x42  BIT 0,D    0x43  BIT 0,E
     *   0x44  BIT 0,H    0x45  BIT 0,L    0x47  BIT 0,A
     *   0x48  BIT 1,B    0x49  BIT 1,C    0x4A  BIT 1,D    0x4B  BIT 1,E
     *   0x4C  BIT 1,H    0x4D  BIT 1,L    0x4F  BIT 1,A
     *   0x50  BIT 2,B    0x51  BIT 2,C    0x52  BIT 2,D    0x53  BIT 2,E
     *   0x54  BIT 2,H    0x55  BIT 2,L    0x57  BIT 2,A
     *   0x58  BIT 3,B    0x59  BIT 3,C    0x5A  BIT 3,D    0x5B  BIT 3,E
     *   0x5C  BIT 3,H    0x5D  BIT 3,L    0x5F  BIT 3,A
     *   0x60  BIT 4,B    0x61  BIT 4,C    0x62  BIT 4,D    0x63  BIT 4,E
     *   0x64  BIT 4,H    0x65  BIT 4,L    0x67  BIT 4,A
     *   0x68  BIT 5,B    0x69  BIT 5,C    0x6A  BIT 5,D    0x6B  BIT 5,E
     *   0x6C  BIT 5,H    0x6D  BIT 5,L    0x6F  BIT 5,A
     *   0x70  BIT 6,B    0x71  BIT 6,C    0x72  BIT 6,D    0x73  BIT 6,E
     *   0x74  BIT 6,H    0x75  BIT 6,L    0x77  BIT 6,A
     *   0x78  BIT 7,B    0x79  BIT 7,C    0x7A  BIT 7,D    0x7B  BIT 7,E
     *   0x7C  BIT 7,H    0x7D  BIT 7,L    0x7F  BIT 7,A
     */
    F &= FLAG_C;
    F |= FLAG_H;
    if (!(reg & (1 << bit))) F |= FLAG_Z;
}

void BIT_HL(uint8_t bit) {
    /*
     *   0x46  BIT 0,(HL)
     *   0x4E  BIT 1,(HL)
     *   0x56  BIT 2,(HL)
     *   0x5E  BIT 3,(HL)
     *   0x66  BIT 4,(HL)
     *   0x6E  BIT 5,(HL)
     *   0x76  BIT 6,(HL)
     *   0x7E  BIT 7,(HL)
     */
    uint8_t val = read_memory((H << 8) | L);
    F &= FLAG_C;
    F |= FLAG_H;
    if (!(val & (1 << bit))) F |= FLAG_Z;
}

void RES_R(uint8_t bit, uint8_t *reg) {
    /*
     *   0x80-0xBF  RES b,r (CB prefix)
     *
     *   0x80  RES 0,B    0x81  RES 0,C    0x82  RES 0,D    0x83  RES 0,E
     *   0x84  RES 0,H    0x85  RES 0,L    0x87  RES 0,A
     *   0x88  RES 1,B    0x89  RES 1,C    0x8A  RES 1,D    0x8B  RES 1,E
     *   0x8C  RES 1,H    0x8D  RES 1,L    0x8F  RES 1,A
     *   0x90  RES 2,B    0x91  RES 2,C    0x92  RES 2,D    0x93  RES 2,E
     *   0x94  RES 2,H    0x95  RES 2,L    0x97  RES 2,A
     *   0x98  RES 3,B    0x99  RES 3,C    0x9A  RES 3,D    0x9B  RES 3,E
     *   0x9C  RES 3,H    0x9D  RES 3,L    0x9F  RES 3,A
     *   0xA0  RES 4,B    0xA1  RES 4,C    0xA2  RES 4,D    0xA3  RES 4,E
     *   0xA4  RES 4,H    0xA5  RES 4,L    0xA7  RES 4,A
     *   0xA8  RES 5,B    0xA9  RES 5,C    0xAA  RES 5,D    0xAB  RES 5,E
     *   0xAC  RES 5,H    0xAD  RES 5,L    0xAF  RES 5,A
     *   0xB0  RES 6,B    0xB1  RES 6,C    0xB2  RES 6,D    0xB3  RES 6,E
     *   0xB4  RES 6,H    0xB5  RES 6,L    0xB7  RES 6,A
     *   0xB8  RES 7,B    0xB9  RES 7,C    0xBA  RES 7,D    0xBB  RES 7,E
     *   0xBC  RES 7,H    0xBD  RES 7,L    0xBF  RES 7,A
     */
    *reg &= ~(1 << bit);
}

void RES_HL(uint8_t bit) {
    /*
     *   0x86  RES 0,(HL)
     *   0x8E  RES 1,(HL)
     *   0x96  RES 2,(HL)
     *   0x9E  RES 3,(HL)
     *   0xA6  RES 4,(HL)
     *   0xAE  RES 5,(HL)
     *   0xB6  RES 6,(HL)
     *   0xBE  RES 7,(HL)
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, read_memory(addr) & ~(1 << bit));
}

void SET_R(uint8_t bit, uint8_t *reg) {
    /*
     *   0xC0-0xFF  SET b,r (CB prefix)
     *
     *   0xC0  SET 0,B    0xC1  SET 0,C    0xC2  SET 0,D    0xC3  SET 0,E
     *   0xC4  SET 0,H    0xC5  SET 0,L    0xC7  SET 0,A
     *   0xC8  SET 1,B    0xC9  SET 1,C    0xCA  SET 1,D    0xCB  SET 1,E
     *   0xCC  SET 1,H    0xCD  SET 1,L    0xCF  SET 1,A
     *   0xD0  SET 2,B    0xD1  SET 2,C    0xD2  SET 2,D    0xD3  SET 2,E
     *   0xD4  SET 2,H    0xD5  SET 2,L    0xD7  SET 2,A
     *   0xD8  SET 3,B    0xD9  SET 3,C    0xDA  SET 3,D    0xDB  SET 3,E
     *   0xDC  SET 3,H    0xDD  SET 3,L    0xDF  SET 3,A
     *   0xE0  SET 4,B    0xE1  SET 4,C    0xE2  SET 4,D    0xE3  SET 4,E
     *   0xE4  SET 4,H    0xE5  SET 4,L    0xE7  SET 4,A
     *   0xE8  SET 5,B    0xE9  SET 5,C    0xEA  SET 5,D    0xEB  SET 5,E
     *   0xEC  SET 5,H    0xED  SET 5,L    0xEF  SET 5,A
     *   0xF0  SET 6,B    0xF1  SET 6,C    0xF2  SET 6,D    0xF3  SET 6,E
     *   0xF4  SET 6,H    0xF5  SET 6,L    0xF7  SET 6,A
     *   0xF8  SET 7,B    0xF9  SET 7,C    0xFA  SET 7,D    0xFB  SET 7,E
     *   0xFC  SET 7,H    0xFD  SET 7,L    0xFF  SET 7,A
     */
    *reg |= (1 << bit);
}

void SET_HL(uint8_t bit) {
    /*
     *   0xC6  SET 0,(HL)
     *   0xCE  SET 1,(HL)
     *   0xD6  SET 2,(HL)
     *   0xDE  SET 3,(HL)
     *   0xE6  SET 4,(HL)
     *   0xEE  SET 5,(HL)
     *   0xF6  SET 6,(HL)
     *   0xFE  SET 7,(HL)
     */
    uint16_t addr = (H << 8) | L;
    write_memory(addr, read_memory(addr) | (1 << bit));
}

void RL_R(uint8_t *reg) {
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (*reg & 0x80) F |= FLAG_C;
    *reg = (*reg << 1) | carry;
    if (*reg == 0)   F |= FLAG_Z;
}

void RL_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (val & 0x80) F |= FLAG_C;
    val = (val << 1) | carry;
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void RLA() {
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (A & 0x80) F |= FLAG_C;
    A = (A << 1) | carry;
}

void RLC_R(uint8_t *reg) {
    F = 0;
    if (*reg & 0x80) F |= FLAG_C;
    *reg = (*reg << 1) | (*reg >> 7);
    if (*reg == 0)   F |= FLAG_Z;
}

void RLC_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F = 0;
    if (val & 0x80) F |= FLAG_C;
    val = (val << 1) | (val >> 7);
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void RLCA() {
    F = 0;
    if (A & 0x80) F |= FLAG_C;
    A = (A << 1) | (A >> 7);
}

void RR_R(uint8_t *reg) {
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (*reg & 0x01) F |= FLAG_C;
    *reg = (*reg >> 1) | (carry << 7);
    if (*reg == 0)   F |= FLAG_Z;
}

void RR_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (val & 0x01) F |= FLAG_C;
    val = (val >> 1) | (carry << 7);
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void RRA() {
    uint8_t carry = (F & FLAG_C) ? 1 : 0;
    F = 0;
    if (A & 0x01) F |= FLAG_C;
    A = (A >> 1) | (carry << 7);
}

void RRC_R(uint8_t *reg) {
    F = 0;
    if (*reg & 0x01) F |= FLAG_C;
    *reg = (*reg >> 1) | (*reg << 7);
    if (*reg == 0)   F |= FLAG_Z;
}

void RRC_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F = 0;
    if (val & 0x01) F |= FLAG_C;
    val = (val >> 1) | (val << 7);
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void RRCA() {
    F = 0;
    if (A & 0x01) F |= FLAG_C;
    A = (A >> 1) | (A << 7);
}

void SLA_R(uint8_t *reg) {
    F = 0;
    if (*reg & 0x80) F |= FLAG_C;
    *reg <<= 1;
    if (*reg == 0)   F |= FLAG_Z;
}

void SLA_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F = 0;
    if (val & 0x80) F |= FLAG_C;
    val <<= 1;
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void SRA_R(uint8_t *reg) {
    F = 0;
    if (*reg & 0x01) F |= FLAG_C;
    *reg = (*reg >> 1) | (*reg & 0x80);
    if (*reg == 0)   F |= FLAG_Z;
}

void SRA_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F = 0;
    if (val & 0x01) F |= FLAG_C;
    val = (val >> 1) | (val & 0x80);
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void SRL_R(uint8_t *reg) {
    F = 0;
    if (*reg & 0x01) F |= FLAG_C;
    *reg >>= 1;
    if (*reg == 0)   F |= FLAG_Z;
}

void SRL_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    F = 0;
    if (val & 0x01) F |= FLAG_C;
    val >>= 1;
    if (val == 0)   F |= FLAG_Z;
    write_memory(addr, val);
}

void SWAP_R(uint8_t *reg) {
    *reg = (*reg << 4) | (*reg >> 4);
    F = 0;
    if (*reg == 0) F |= FLAG_Z;
}

void SWAP_HL() {
    uint16_t addr = (H << 8) | L;
    uint8_t val = read_memory(addr);
    val = (val << 4) | (val >> 4);
    F = 0;
    if (val == 0) F |= FLAG_Z;
    write_memory(addr, val);
}

void CALL_N16() {
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    uint16_t addr = (hi << 8) | lo;
    SP--;
    write_memory(SP--, PC >> 8);
    write_memory(SP, PC & 0xFF);
    PC = addr;
}

void CALL_CC_N16(uint8_t condition) {
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    uint16_t addr = (hi << 8) | lo;
    if (condition) {
        SP--;
        write_memory(SP--, PC >> 8);
        write_memory(SP, PC & 0xFF);
        PC = addr;
    }
}

void JP_HL() {
    PC = (H << 8) | L;
}

void JP_N16() {
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    PC = (hi << 8) | lo;
}

void JP_CC_N16(uint8_t condition) {
    uint8_t lo = read_memory(PC++);
    uint8_t hi = read_memory(PC++);
    if (condition)
        PC = (hi << 8) | lo;
}

void JR_N16() {
    int8_t offset = (int8_t)read_memory(PC++);
    PC += offset;
}

void JR_CC_N16(uint8_t condition) {
    int8_t offset = (int8_t)read_memory(PC++);
    if (condition)
        PC += offset;
}

void RET() {
    uint8_t lo = read_memory(SP++);
    uint8_t hi = read_memory(SP++);
    PC = (hi << 8) | lo;
}

void RET_CC(uint8_t condition) {
    if (condition) {
        uint8_t lo = read_memory(SP++);
        uint8_t hi = read_memory(SP++);
        PC = (hi << 8) | lo;
    }
}

void RETI() {
    uint8_t lo = read_memory(SP++);
    uint8_t hi = read_memory(SP++);
    PC = (hi << 8) | lo;
}

void RST(uint8_t vec) {
    SP--;
    write_memory(SP--, PC >> 8);
    write_memory(SP, PC & 0xFF);
    PC = vec;
}

void CCF() {
    F &= ~FLAG_N;
    F &= ~FLAG_H;
    F ^= FLAG_C;
}

void SCF() {
    F &= ~FLAG_N;
    F &= ~FLAG_H;
    F |= FLAG_C;
}

void ADD_SP_E8() {
    int8_t e8 = (int8_t)read_memory(PC++);
    uint16_t result = SP + e8;
    F = 0;
    if (((SP ^ e8 ^ result) & 0x10))  F |= FLAG_H;
    if (((SP ^ e8 ^ result) & 0x100)) F |= FLAG_C;
    SP = result;
}

void PUSH_R16(uint8_t hi, uint8_t lo) {
    write_memory(--SP, hi);
    write_memory(--SP, lo);
}

void POP_R16(uint8_t *hi, uint8_t *lo) {
    *lo = read_memory(SP++);
    *hi = read_memory(SP++);
}

void PUSH_AF() {
    write_memory(--SP, A);
    write_memory(--SP, F & 0xF0);
}

void POP_AF() {
    F = read_memory(SP++) & 0xF0;
    A = read_memory(SP++);
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

            case 0x80: ADD_R(B); break;
            case 0x81: ADD_R(C); break;
            case 0x82: ADD_R(D); break;
            case 0x83: ADD_R(E); break;
            case 0x84: ADD_R(H); break;
            case 0x85: ADD_R(L); break;
            case 0x86: ADD_HL();  break;
            case 0x87: ADD_R(A); break;
            case 0xC6: ADD_N();   break;

            case 0x88: ADC_R(B); break;
            case 0x89: ADC_R(C); break;
            case 0x8A: ADC_R(D); break;
            case 0x8B: ADC_R(E); break;
            case 0x8C: ADC_R(H); break;
            case 0x8D: ADC_R(L); break;
            case 0x8E: ADC_HL();  break;
            case 0x8F: ADC_R(A); break;
            case 0xCE: ADC_N();   break;

            case 0x90: SUB_R(B); break;
            case 0x91: SUB_R(C); break;
            case 0x92: SUB_R(D); break;
            case 0x93: SUB_R(E); break;
            case 0x94: SUB_R(H); break;
            case 0x95: SUB_R(L); break;
            case 0x96: SUB_HL();  break;
            case 0x97: SUB_R(A); break;
            case 0xD6: SUB_N();   break;

            case 0x98: SBC_R(B); break;
            case 0x99: SBC_R(C); break;
            case 0x9A: SBC_R(D); break;
            case 0x9B: SBC_R(E); break;
            case 0x9C: SBC_R(H); break;
            case 0x9D: SBC_R(L); break;
            case 0x9E: SBC_HL();  break;
            case 0x9F: SBC_R(A); break;
            case 0xDE: SBC_N();   break;

            case 0xB8: CP_R(B); break;
            case 0xB9: CP_R(C); break;
            case 0xBA: CP_R(D); break;
            case 0xBB: CP_R(E); break;
            case 0xBC: CP_R(H); break;
            case 0xBD: CP_R(L); break;
            case 0xBE: CP_HL();  break;
            case 0xBF: CP_R(A); break;
            case 0xFE: CP_N();   break;

            case 0x04: INC_R(&B); break;
            case 0x0C: INC_R(&C); break;
            case 0x14: INC_R(&D); break;
            case 0x1C: INC_R(&E); break;
            case 0x24: INC_R(&H); break;
            case 0x2C: INC_R(&L); break;
            case 0x3C: INC_R(&A); break;
            case 0x34: INC_HL();   break;

            case 0x05: DEC_R(&B); break;
            case 0x0D: DEC_R(&C); break;
            case 0x15: DEC_R(&D); break;
            case 0x1D: DEC_R(&E); break;
            case 0x25: DEC_R(&H); break;
            case 0x2D: DEC_R(&L); break;
            case 0x3D: DEC_R(&A); break;
            case 0x35: DEC_HL();   break;

            case 0x09: ADD_HL_R16(B, C); break;
            case 0x19: ADD_HL_R16(D, E); break;
            case 0x29: ADD_HL_R16(H, L); break;
            case 0x39: ADD_HL_R16(SP >> 8, SP & 0xFF); break;

            case 0x03: INC_R16(&B, &C); break;
            case 0x13: INC_R16(&D, &E); break;
            case 0x23: INC_R16(&H, &L); break;
            case 0x33: SP++; break;

            case 0x0B: DEC_R16(&B, &C); break;
            case 0x1B: DEC_R16(&D, &E); break;
            case 0x2B: DEC_R16(&H, &L); break;
            case 0x3B: SP--; break;

            case 0xA0: AND_R(B); break;
            case 0xA1: AND_R(C); break;
            case 0xA2: AND_R(D); break;
            case 0xA3: AND_R(E); break;
            case 0xA4: AND_R(H); break;
            case 0xA5: AND_R(L); break;
            case 0xA6: AND_HL();  break;
            case 0xA7: AND_R(A); break;
            case 0xE6: AND_N();   break;

            case 0xB0: OR_R(B); break;
            case 0xB1: OR_R(C); break;
            case 0xB2: OR_R(D); break;
            case 0xB3: OR_R(E); break;
            case 0xB4: OR_R(H); break;
            case 0xB5: OR_R(L); break;
            case 0xB6: OR_HL();  break;
            case 0xB7: OR_R(A); break;
            case 0xF6: OR_N();   break;

            case 0xA8: XOR_R(B); break;
            case 0xA9: XOR_R(C); break;
            case 0xAA: XOR_R(D); break;
            case 0xAB: XOR_R(E); break;
            case 0xAC: XOR_R(H); break;
            case 0xAD: XOR_R(L); break;
            case 0xAE: XOR_HL();  break;
            case 0xAF: XOR_R(A); break;
            case 0xEE: XOR_N();   break;

            case 0x2F: CPL(); break;

            case 0x00: NOP();  break;
            case 0x10: STOP(); break;
            case 0x27: DAA();  break;

            case 0xCB: {
                uint8_t cb_op = read_memory(PC++);
                switch (cb_op) {

                    case 0x40: BIT_R(0, B); break;
                    case 0x41: BIT_R(0, C); break;
                    case 0x42: BIT_R(0, D); break;
                    case 0x43: BIT_R(0, E); break;
                    case 0x44: BIT_R(0, H); break;
                    case 0x45: BIT_R(0, L); break;
                    case 0x47: BIT_R(0, A); break;

                    case 0x48: BIT_R(1, B); break;
                    case 0x49: BIT_R(1, C); break;
                    case 0x4A: BIT_R(1, D); break;
                    case 0x4B: BIT_R(1, E); break;
                    case 0x4C: BIT_R(1, H); break;
                    case 0x4D: BIT_R(1, L); break;
                    case 0x4F: BIT_R(1, A); break;

                    case 0x50: BIT_R(2, B); break;
                    case 0x51: BIT_R(2, C); break;
                    case 0x52: BIT_R(2, D); break;
                    case 0x53: BIT_R(2, E); break;
                    case 0x54: BIT_R(2, H); break;
                    case 0x55: BIT_R(2, L); break;
                    case 0x57: BIT_R(2, A); break;

                    case 0x58: BIT_R(3, B); break;
                    case 0x59: BIT_R(3, C); break;
                    case 0x5A: BIT_R(3, D); break;
                    case 0x5B: BIT_R(3, E); break;
                    case 0x5C: BIT_R(3, H); break;
                    case 0x5D: BIT_R(3, L); break;
                    case 0x5F: BIT_R(3, A); break;

                    case 0x60: BIT_R(4, B); break;
                    case 0x61: BIT_R(4, C); break;
                    case 0x62: BIT_R(4, D); break;
                    case 0x63: BIT_R(4, E); break;
                    case 0x64: BIT_R(4, H); break;
                    case 0x65: BIT_R(4, L); break;
                    case 0x67: BIT_R(4, A); break;

                    case 0x68: BIT_R(5, B); break;
                    case 0x69: BIT_R(5, C); break;
                    case 0x6A: BIT_R(5, D); break;
                    case 0x6B: BIT_R(5, E); break;
                    case 0x6C: BIT_R(5, H); break;
                    case 0x6D: BIT_R(5, L); break;
                    case 0x6F: BIT_R(5, A); break;

                    case 0x70: BIT_R(6, B); break;
                    case 0x71: BIT_R(6, C); break;
                    case 0x72: BIT_R(6, D); break;
                    case 0x73: BIT_R(6, E); break;
                    case 0x74: BIT_R(6, H); break;
                    case 0x75: BIT_R(6, L); break;
                    case 0x77: BIT_R(6, A); break;

                    case 0x78: BIT_R(7, B); break;
                    case 0x79: BIT_R(7, C); break;
                    case 0x7A: BIT_R(7, D); break;
                    case 0x7B: BIT_R(7, E); break;
                    case 0x7C: BIT_R(7, H); break;
                    case 0x7D: BIT_R(7, L); break;
                    case 0x7F: BIT_R(7, A); break;

                    case 0x46: BIT_HL(0); break;
                    case 0x4E: BIT_HL(1); break;
                    case 0x56: BIT_HL(2); break;
                    case 0x5E: BIT_HL(3); break;
                    case 0x66: BIT_HL(4); break;
                    case 0x6E: BIT_HL(5); break;
                    case 0x76: BIT_HL(6); break;
                    case 0x7E: BIT_HL(7); break;

                    case 0x80: RES_R(0, &B); break;
                    case 0x81: RES_R(0, &C); break;
                    case 0x82: RES_R(0, &D); break;
                    case 0x83: RES_R(0, &E); break;
                    case 0x84: RES_R(0, &H); break;
                    case 0x85: RES_R(0, &L); break;
                    case 0x87: RES_R(0, &A); break;

                    case 0x88: RES_R(1, &B); break;
                    case 0x89: RES_R(1, &C); break;
                    case 0x8A: RES_R(1, &D); break;
                    case 0x8B: RES_R(1, &E); break;
                    case 0x8C: RES_R(1, &H); break;
                    case 0x8D: RES_R(1, &L); break;
                    case 0x8F: RES_R(1, &A); break;

                    case 0x90: RES_R(2, &B); break;
                    case 0x91: RES_R(2, &C); break;
                    case 0x92: RES_R(2, &D); break;
                    case 0x93: RES_R(2, &E); break;
                    case 0x94: RES_R(2, &H); break;
                    case 0x95: RES_R(2, &L); break;
                    case 0x97: RES_R(2, &A); break;

                    case 0x98: RES_R(3, &B); break;
                    case 0x99: RES_R(3, &C); break;
                    case 0x9A: RES_R(3, &D); break;
                    case 0x9B: RES_R(3, &E); break;
                    case 0x9C: RES_R(3, &H); break;
                    case 0x9D: RES_R(3, &L); break;
                    case 0x9F: RES_R(3, &A); break;

                    case 0xA0: RES_R(4, &B); break;
                    case 0xA1: RES_R(4, &C); break;
                    case 0xA2: RES_R(4, &D); break;
                    case 0xA3: RES_R(4, &E); break;
                    case 0xA4: RES_R(4, &H); break;
                    case 0xA5: RES_R(4, &L); break;
                    case 0xA7: RES_R(4, &A); break;

                    case 0xA8: RES_R(5, &B); break;
                    case 0xA9: RES_R(5, &C); break;
                    case 0xAA: RES_R(5, &D); break;
                    case 0xAB: RES_R(5, &E); break;
                    case 0xAC: RES_R(5, &H); break;
                    case 0xAD: RES_R(5, &L); break;
                    case 0xAF: RES_R(5, &A); break;

                    case 0xB0: RES_R(6, &B); break;
                    case 0xB1: RES_R(6, &C); break;
                    case 0xB2: RES_R(6, &D); break;
                    case 0xB3: RES_R(6, &E); break;
                    case 0xB4: RES_R(6, &H); break;
                    case 0xB5: RES_R(6, &L); break;
                    case 0xB7: RES_R(6, &A); break;

                    case 0xB8: RES_R(7, &B); break;
                    case 0xB9: RES_R(7, &C); break;
                    case 0xBA: RES_R(7, &D); break;
                    case 0xBB: RES_R(7, &E); break;
                    case 0xBC: RES_R(7, &H); break;
                    case 0xBD: RES_R(7, &L); break;
                    case 0xBF: RES_R(7, &A); break;

                    case 0x86: RES_HL(0); break;
                    case 0x8E: RES_HL(1); break;
                    case 0x96: RES_HL(2); break;
                    case 0x9E: RES_HL(3); break;
                    case 0xA6: RES_HL(4); break;
                    case 0xAE: RES_HL(5); break;
                    case 0xB6: RES_HL(6); break;
                    case 0xBE: RES_HL(7); break;

                    case 0xC0: SET_R(0, &B); break;
                    case 0xC1: SET_R(0, &C); break;
                    case 0xC2: SET_R(0, &D); break;
                    case 0xC3: SET_R(0, &E); break;
                    case 0xC4: SET_R(0, &H); break;
                    case 0xC5: SET_R(0, &L); break;
                    case 0xC7: SET_R(0, &A); break;

                    case 0xC8: SET_R(1, &B); break;
                    case 0xC9: SET_R(1, &C); break;
                    case 0xCA: SET_R(1, &D); break;
                    case 0xCB: SET_R(1, &E); break;
                    case 0xCC: SET_R(1, &H); break;
                    case 0xCD: SET_R(1, &L); break;
                    case 0xCF: SET_R(1, &A); break;

                    case 0xD0: SET_R(2, &B); break;
                    case 0xD1: SET_R(2, &C); break;
                    case 0xD2: SET_R(2, &D); break;
                    case 0xD3: SET_R(2, &E); break;
                    case 0xD4: SET_R(2, &H); break;
                    case 0xD5: SET_R(2, &L); break;
                    case 0xD7: SET_R(2, &A); break;

                    case 0xD8: SET_R(3, &B); break;
                    case 0xD9: SET_R(3, &C); break;
                    case 0xDA: SET_R(3, &D); break;
                    case 0xDB: SET_R(3, &E); break;
                    case 0xDC: SET_R(3, &H); break;
                    case 0xDD: SET_R(3, &L); break;
                    case 0xDF: SET_R(3, &A); break;

                    case 0xE0: SET_R(4, &B); break;
                    case 0xE1: SET_R(4, &C); break;
                    case 0xE2: SET_R(4, &D); break;
                    case 0xE3: SET_R(4, &E); break;
                    case 0xE4: SET_R(4, &H); break;
                    case 0xE5: SET_R(4, &L); break;
                    case 0xE7: SET_R(4, &A); break;

                    case 0xE8: SET_R(5, &B); break;
                    case 0xE9: SET_R(5, &C); break;
                    case 0xEA: SET_R(5, &D); break;
                    case 0xEB: SET_R(5, &E); break;
                    case 0xEC: SET_R(5, &H); break;
                    case 0xED: SET_R(5, &L); break;
                    case 0xEF: SET_R(5, &A); break;

                    case 0xF0: SET_R(6, &B); break;
                    case 0xF1: SET_R(6, &C); break;
                    case 0xF2: SET_R(6, &D); break;
                    case 0xF3: SET_R(6, &E); break;
                    case 0xF4: SET_R(6, &H); break;
                    case 0xF5: SET_R(6, &L); break;
                    case 0xF7: SET_R(6, &A); break;

                    case 0xF8: SET_R(7, &B); break;
                    case 0xF9: SET_R(7, &C); break;
                    case 0xFA: SET_R(7, &D); break;
                    case 0xFB: SET_R(7, &E); break;
                    case 0xFC: SET_R(7, &H); break;
                    case 0xFD: SET_R(7, &L); break;
                    case 0xFF: SET_R(7, &A); break;

                    case 0xC6: SET_HL(0); break;
                    case 0xCE: SET_HL(1); break;
                    case 0xD6: SET_HL(2); break;
                    case 0xDE: SET_HL(3); break;
                    case 0xE6: SET_HL(4); break;
                    case 0xEE: SET_HL(5); break;
                    case 0xF6: SET_HL(6); break;
                    case 0xFE: SET_HL(7); break;

                    case 0x00: RLC_R(&B); break;
                    case 0x01: RLC_R(&C); break;
                    case 0x02: RLC_R(&D); break;
                    case 0x03: RLC_R(&E); break;
                    case 0x04: RLC_R(&H); break;
                    case 0x05: RLC_R(&L); break;
                    case 0x06: RLC_HL();  break;
                    case 0x07: RLC_R(&A); break;

                    case 0x08: RRC_R(&B); break;
                    case 0x09: RRC_R(&C); break;
                    case 0x0A: RRC_R(&D); break;
                    case 0x0B: RRC_R(&E); break;
                    case 0x0C: RRC_R(&H); break;
                    case 0x0D: RRC_R(&L); break;
                    case 0x0E: RRC_HL();  break;
                    case 0x0F: RRC_R(&A); break;

                    case 0x10: RL_R(&B); break;
                    case 0x11: RL_R(&C); break;
                    case 0x12: RL_R(&D); break;
                    case 0x13: RL_R(&E); break;
                    case 0x14: RL_R(&H); break;
                    case 0x15: RL_R(&L); break;
                    case 0x16: RL_HL();  break;
                    case 0x17: RL_R(&A); break;

                    case 0x18: RR_R(&B); break;
                    case 0x19: RR_R(&C); break;
                    case 0x1A: RR_R(&D); break;
                    case 0x1B: RR_R(&E); break;
                    case 0x1C: RR_R(&H); break;
                    case 0x1D: RR_R(&L); break;
                    case 0x1E: RR_HL();  break;
                    case 0x1F: RR_R(&A); break;

                    case 0x20: SLA_R(&B); break;
                    case 0x21: SLA_R(&C); break;
                    case 0x22: SLA_R(&D); break;
                    case 0x23: SLA_R(&E); break;
                    case 0x24: SLA_R(&H); break;
                    case 0x25: SLA_R(&L); break;
                    case 0x26: SLA_HL();  break;
                    case 0x27: SLA_R(&A); break;

                    case 0x28: SRA_R(&B); break;
                    case 0x29: SRA_R(&C); break;
                    case 0x2A: SRA_R(&D); break;
                    case 0x2B: SRA_R(&E); break;
                    case 0x2C: SRA_R(&H); break;
                    case 0x2D: SRA_R(&L); break;
                    case 0x2E: SRA_HL();  break;
                    case 0x2F: SRA_R(&A); break;

                    case 0x30: SWAP_R(&B); break;
                    case 0x31: SWAP_R(&C); break;
                    case 0x32: SWAP_R(&D); break;
                    case 0x33: SWAP_R(&E); break;
                    case 0x34: SWAP_R(&H); break;
                    case 0x35: SWAP_R(&L); break;
                    case 0x36: SWAP_HL();  break;
                    case 0x37: SWAP_R(&A); break;

                    case 0x38: SRL_R(&B); break;
                    case 0x39: SRL_R(&C); break;
                    case 0x3A: SRL_R(&D); break;
                    case 0x3B: SRL_R(&E); break;
                    case 0x3C: SRL_R(&H); break;
                    case 0x3D: SRL_R(&L); break;
                    case 0x3E: SRL_HL();  break;
                    case 0x3F: SRL_R(&A); break;

                    default:
                        printf("Unknown CB: 0x%02X @ PC=0x%04X\n", cb_op, PC - 1);
                        break;
                }
            } break;

                    case 0x07: RLCA(); break;
                    case 0x0F: RRCA(); break;
                    case 0x17: RLA();  break;
                    case 0x1F: RRA();  break;

                    case 0xCD: CALL_N16(); break;

                    case 0xC4: CALL_CC_N16(!(F & FLAG_Z)); break;
                    case 0xCC: CALL_CC_N16(  F & FLAG_Z);  break;
                    case 0xD4: CALL_CC_N16(!(F & FLAG_C)); break;
                    case 0xDC: CALL_CC_N16(  F & FLAG_C);  break;

                    case 0xE9: JP_HL(); break;
                    case 0xC3: JP_N16(); break;

                    case 0xC2: JP_CC_N16(!(F & FLAG_Z)); break;
                    case 0xCA: JP_CC_N16(  F & FLAG_Z);  break;
                    case 0xD2: JP_CC_N16(!(F & FLAG_C)); break;
                    case 0xDA: JP_CC_N16(  F & FLAG_C);  break;

                    case 0x18: JR_N16(); break;

                    case 0x20: JR_CC_N16(!(F & FLAG_Z)); break;
                    case 0x28: JR_CC_N16(  F & FLAG_Z);  break;
                    case 0x30: JR_CC_N16(!(F & FLAG_C)); break;
                    case 0x38: JR_CC_N16(  F & FLAG_C);  break;

                    case 0xC9: RET(); break;
                    case 0xD9: RETI(); break;

                    case 0xC0: RET_CC(!(F & FLAG_Z)); break;
                    case 0xC8: RET_CC(  F & FLAG_Z);  break;
                    case 0xD0: RET_CC(!(F & FLAG_C)); break;
                    case 0xD8: RET_CC(  F & FLAG_C);  break;

                    case 0xC7: RST(0x00); break;
                    case 0xCF: RST(0x08); break;
                    case 0xD7: RST(0x10); break;
                    case 0xDF: RST(0x18); break;
                    case 0xE7: RST(0x20); break;
                    case 0xEF: RST(0x28); break;
                    case 0xF7: RST(0x30); break;
                    case 0xFF: RST(0x38); break;

                    case 0x3F: CCF(); break;
                    case 0x37: SCF(); break;

                    case 0xE8: ADD_SP_E8(); break;

                    case 0xF5: PUSH_AF(); break;
                    case 0xC5: PUSH_R16(B, C); break;
                    case 0xD5: PUSH_R16(D, E); break;
                    case 0xE5: PUSH_R16(H, L); break;

                    case 0xF1: POP_AF(); break;
                    case 0xC1: POP_R16(&B, &C); break;
                    case 0xD1: POP_R16(&D, &E); break;
                    case 0xE1: POP_R16(&H, &L); break;

            default:
                printf("Unknown opcode: 0x%02X @ PC=0x%04X\n", opcode, PC - 1);
                break;
        }
    }
}

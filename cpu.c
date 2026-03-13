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

            default:
                printf("Unknown opcode: 0x%02X @ PC=0x%04X\n", opcode, PC - 1);
                break;
        }
    }
}

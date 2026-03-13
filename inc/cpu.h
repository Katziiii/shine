#pragma once

#include <stdint.h>

#define FLAG_Z  0x80
#define FLAG_N  0x40
#define FLAG_H  0x20
#define FLAG_C  0x10

uint8_t get_opcode();
void cpu_exec () ;

uint8_t LD8_RR(uint8_t *reg1, uint8_t reg2);
uint8_t LD8_RN(uint8_t *reg);
uint16_t LD16_RN(uint8_t *hi, uint8_t *lo);
void LD_HL_R(uint8_t *reg);
void LD_HL_N();
uint8_t LD_R_HL();
void LD_R16_A(uint8_t hi, uint8_t lo);
void LD_N16_A();
void LDH_N16_A();
void LDH_C_A();
uint8_t LD_A_R16(uint8_t hi, uint8_t lo);
uint8_t LD_A_N16();
uint8_t LDH_A_N16();
uint8_t LDH_A_C();
void LD_HLI_A();
void LD_HLD_A();
uint8_t LD_A_HLD();
uint8_t LD_A_HLI();
void LD_N16_SP();
void LD_HL_SP_E8();
void LD_SP_HL();

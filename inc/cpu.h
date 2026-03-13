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

void ADD_R(uint8_t reg);
void ADD_HL();
void ADD_N();
void ADC_R(uint8_t reg);
void ADC_HL();
void ADC_N();
void SUB_R(uint8_t reg);
void SUB_HL();
void SUB_N();
void SBC_R(uint8_t reg);
void SBC_HL();
void SBC_N();
void CP_R(uint8_t reg);
void CP_HL();
void CP_N();
void INC_R(uint8_t *reg);
void INC_HL();
void DEC_R(uint8_t *reg);
void DEC_HL();

void ADD_HL_R16(uint8_t hi, uint8_t lo);
void INC_R16(uint8_t *hi, uint8_t *lo);
void DEC_R16(uint8_t *hi, uint8_t *lo);

void AND_R(uint8_t reg);
void AND_HL();
void AND_N();
void OR_R(uint8_t reg);
void OR_HL();
void OR_N();
void XOR_R(uint8_t reg);
void XOR_HL();
void XOR_N();
void CPL();

void NOP();
void STOP();
void DAA();

void BIT_R(uint8_t bit, uint8_t reg);
void BIT_HL(uint8_t bit);
void RES_R(uint8_t bit, uint8_t *reg);
void RES_HL(uint8_t bit);
void SET_R(uint8_t bit, uint8_t *reg);
void SET_HL(uint8_t bit);

void RL_R(uint8_t *reg);
void RL_HL();
void RLA();
void RLC_R(uint8_t *reg);
void RLC_HL();
void RLCA();
void RR_R(uint8_t *reg);
void RR_HL();
void RRA();
void RRC_R(uint8_t *reg);
void RRC_HL();
void RRCA();
void SLA_R(uint8_t *reg);
void SLA_HL();
void SRA_R(uint8_t *reg);
void SRA_HL();
void SRL_R(uint8_t *reg);
void SRL_HL();
void SWAP_R(uint8_t *reg);
void SWAP_HL();

void CALL_N16();
void CALL_CC_N16(uint8_t condition);
void JP_HL();
void JP_N16();
void JP_CC_N16(uint8_t condition);
void JR_N16();
void JR_CC_N16(uint8_t condition);
void RET();
void RET_CC(uint8_t condition);
void RETI();
void RST(uint8_t vec);

void CCF();
void SCF();

void ADD_SP_E8();
void PUSH_R16(uint8_t hi, uint8_t lo);
void POP_R16(uint8_t *hi, uint8_t *lo);
void PUSH_AF();
void POP_AF();

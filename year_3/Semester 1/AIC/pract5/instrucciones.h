/**
 * @file instrucciones.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene los códigos y los campos de función de las instrucciones del RV
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef instrucciones_H
#define instrucciones_H

/*** Includes **************************************************************/

#include "tipos.h"
#include "main.h"
#include "codop.h"
#include "g_string.h"

// Codificación del equivalente a una NOP
#define INSTRUC_NOP 0x00000013

/*
 * The RISC-V Instruction Set Manual
 * Volume I: Unprivileged ISA
 * Document Version 20191213
 * Table 24.1
 */

#define OPCODE_LOAD       0b0000011
#define OPCODE_LOAD_FP    0b0000111
#define OPCODE_CUSTOM_0   0b0001011
#define OPCODE_MISC_MEM   0b0001111
#define OPCODE_OP_IMM     0b0010011
#define OPCODE_AUIPC      0b0010111
#define OPCODE_OP_IMM_32  0b0011011

#define OPCODE_STORE      0b0100011
#define OPCODE_STORE_FP   0b0100111
#define OPCODE_CUSTOM_1   0b0101011
#define OPCODE_AMO        0b0101111
#define OPCODE_OP         0b0110011
#define OPCODE_LUI        0b0110111
#define OPCODE_OP_32      0b0111011

#define OPCODE_MADD       0b1000011
#define OPCODE_MSUB       0b1000111
#define OPCODE_NMSUB      0b1001011
#define OPCODE_NMADD      0b1001111
#define OPCODE_OP_FP      0b1010011
#define OPCODE_RESERVED_1 0b1010111
#define OPCODE_CUSTOM_2   0b1011011

#define OPCODE_BRANCH     0b1100011
#define OPCODE_JALR       0b1100111
#define OPCODE_RESERVED_0 0b1101011
#define OPCODE_JAL        0b1101111
#define OPCODE_SYSTEM     0b1110011
#define OPCODE_RESERVED_2 0b1110111
#define OPCODE_CUSTOM_3   0b1111011

#define OPCODE_PSEUDO     0

#define F3_FENCE    0
#define F3_FENCEI   1
#define F4_FM_NONE  0b0000
#define F4_FM_TSO   0b1000

#define F3_CSRRW    0b001
#define F3_CSRRS    0b010
#define F3_CSRRC    0b011
#define F3_CSRRWI   0b101
#define F3_CSRRSI   0b110
#define F3_CSRRCI   0b111

#define F12_ECALL   0
#define F12_EBREAK  1

#define F3_PRIV 0

#define F6_SRA 0b010000

#define DYNAMIC_RM 0b111

/*** Tipos *****************************************************************/

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    unsigned int funct7: 7;
} format_R_type;

// Usada para las instrucciones AMO
// Se puede utilizar esta versión o incluir los campos 'aq' y 'rl' como parte del 'funct7' del formato R
typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    unsigned int rl: 1;
    unsigned int aq: 1;
    unsigned int funct5: 5;
} format_R_amo_type;

// Usada para las instrucciones de como flotante
// Se puede utilizar esta versión o incluir el campo 'fmt' como parte del 'funct7' del formato R
typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int rm: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    unsigned int fmt: 2;  // S, D, H Q
    unsigned int funct5: 5;
} format_R_fp_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int rm: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    unsigned int fmt: 2;  // S, D, H Q
    unsigned int rs3: 5;
} format_R_fp_4rm_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    int imm11_0: 12;
} format_I_type;

// Usado por las loads de coma flotante
// TODO vale la pena renombrar funct3 por width ?
typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int width: 3;
    unsigned int rs1: 5;
    int imm11_0: 12;
} format_I_fp_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int funct12: 12;
} format_I_system_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int shamt: 6;
    unsigned int funct6: 6;
} format_I_shift_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int csr: 12;
} format_I_csr_R_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int funct3: 3;
    unsigned int uimm4_0: 5;
    unsigned int csr: 12;
} format_I_csr_I_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int imm4_0: 5;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    int imm11_5: 7;
} format_S_type;

// Usado por los stores de coma flotante
// TODO vale la pena renombrar funct3 por width ?
typedef struct {
    unsigned int opcode: 7;
    unsigned int imm4_0: 5;
    unsigned int width: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    int imm11_5: 7;
} format_S_fp_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int imm11: 1;
    unsigned int imm4_1: 4;
    unsigned int funct3: 3;
    unsigned int rs1: 5;
    unsigned int rs2: 5;
    unsigned int imm10_5: 6;
    int imm12: 1;
} format_B_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    int imm31_12: 20;
} format_U_type;

typedef struct {
    unsigned int opcode: 7;
    unsigned int rd: 5;
    unsigned int imm19_12: 8;
    unsigned int imm11: 1;
    unsigned int imm10_1: 10;
    int imm20: 1;
} format_J_type;

typedef union {
    // Codificación de la instrucción
    unsigned int valor;
    // Campo opcode común a todos los formatos
    unsigned int opcode: 7;

    // Formato útil para decodificar
    format_decode_type decode; // Definido en codop.h

    // R-type
    format_R_type R;
    // I-type
    format_I_type I;
    // S-type
    format_S_type S;
    // B-type
    format_B_type B;
    // U-type
    format_U_type U;
    // J-type
    format_J_type J;

    // Variantes
    // R-amo-subtype
    format_R_amo_type Ramo;
    // R-fp-subtype
    format_R_fp_type Rfp;
    // R-4rm-subtype
    format_R_fp_4rm_type R4rm;
    // I-shift-subtype
    format_I_shift_type Ishift;
    // I-system-subtype
    format_I_system_type Isystem;
    // I-csr-R-type
    format_I_csr_R_type IcsrR;
    // I-csr-I-type
    format_I_csr_I_type IcsrI;
    // I-type
    format_I_fp_type Ifp;
    // S-type
    format_S_fp_type Sfp;

} formato_instruccion_t;

/*** Macros ****************************************************************/

// Macros para obtener el tipo de instrucción

// Instrucciones de carga enteras
#define inst_int_load(i) (OPCODE(i) == OPCODE_LOAD)
#define inst_amo_load(i) (((i)==RV32A_LR_W) || ((i)==RV32A_LR_W_AQ) || ((i)==RV32A_LR_W_RL) || ((i)==RV32A_LR_W_AQRL) || \
                    ((i)==RV64A_LR_D) || ((i)==RV64A_LR_D_AQ) || ((i)==RV64A_LR_D_RL) || ((i)==RV64A_LR_D_AQRL))

// Instrucciones de carga fp
#define inst_fp_load(i)  (OPCODE(i) == OPCODE_LOAD_FP)
#define inst_load(i) (inst_int_load(i) || inst_fp_load(i) || inst_amo_load(i))

// Instrucciones de almacenamiento enteras
#define inst_int_store(i)  (OPCODE(i) == OPCODE_STORE)
#define inst_amo_store(i) (((i)==RV32A_SC_W) || ((i)==RV32A_SC_W_AQ) || ((i)==RV32A_SC_W_RL) || ((i)==RV32A_SC_W_AQRL) || \
                    ((i)==RV64A_SC_D) || ((i)==RV64A_SC_D_AQ) || ((i)==RV64A_SC_D_RL) || ((i)==RV64A_SC_D_AQRL))

// Instrucciones de almacenamiento fp
#define inst_fps_store(i) ((i) == RV32F_FSW)
#define inst_fpd_store(i)  ((i) == RV32D_FSD)
#define inst_fp_store(i)  (OPCODE(i) == OPCODE_STORE_FP)

#define inst_store(i) ( inst_int_store(i) || inst_fp_store(i) || inst_amo_store(i))

// Instrucciones de salto
#define inst_jal(i) (OPCODE(i) == OPCODE_JAL)
#define inst_jalr(i) (OPCODE(i) == OPCODE_JALR)
#define inst_jal_jalr(i) ((OPCODE(i) == OPCODE_JAL) || (OPCODE(i) == OPCODE_JALR) )
#define inst_branch(i)  (OPCODE(i) == OPCODE_BRANCH)
#define inst_salto(i) (inst_branch(i) || inst_jal_jalr(i))

// Instrucciones que leen el PC
#define inst_leePC(i) (inst_branch(i)|| inst_jal(i)|| (OPCODE(i) == OPCODE_AUIPC))

// ecall
#define inst_ecall(i) ((i) == RV32I_ECALL)

#define inst_int(i) ((SET(i) == RV32I) || (SET(i)==RV32M) || (SET(i) == RV32A)|| (SET(i)==RV64I) || (SET(i)==RV64M) || (SET(i) == RV64A))
#define inst_int_simple(i) ((SET(i) == RV32I) || (SET(i) == RV32A)|| (SET(i)==RV64I) || (SET(i) == RV64A))

// Instrucciones enteras que escriben registros enteros
#define inst_int_to_Rint(i) (OPCODE(i)==OPCODE_OP_IMM || OPCODE(i)==OPCODE_LUI|| OPCODE(i)==OPCODE_AUIPC || \
                    OPCODE(i)==OPCODE_OP || OPCODE(i)==OPCODE_LOAD || OPCODE(i)==OPCODE_AMO || \
                    OPCODE(i)==OPCODE_OP_IMM_32 || OPCODE(i)==OPCODE_OP_32 || \
                    inst_jal_jalr(i))

// Instrucciones de multiplicacion y division
#define inst_int_muldiv ((SET(i) == RV32M) || (SET(i) == RV64M))

// Instrucciones fused madd
#define inst_fp_madd_d(i) ((i)==RV32D_FMADD_D || (i)==RV32D_FMSUB_D || (i)==RV32D_FNMSUB_D || (i)==RV32D_FNMADD_D )
#define inst_fp_madd_s(i) ((i)==RV32F_FMADD_S || (i)==RV32F_FMSUB_S || (i)==RV32F_FNMSUB_S || (i)==RV32F_FNMADD_S )
#define inst_fp_madd(i) (inst_fp_madd_s(i) || inst_fp_madd_d(i) )

// Instrucciones que escriben simd
#define inst_simd(i) ((i)== OP_FP_ADD_PS || (i)== OP_FP_SUB_PS || (i)== OP_FP_MUL_PS || (i)== OP_FP_DIV_PS || (i)== OP_FP_L_PS || (i)== OP_FP_S_PS)

#define inst_fp(i) (inst_fp_d(i) || inst_fp_s(i) )

// Instrucciones fp que escriben registros enteros
#define inst_fp_to_Rint(i) ( inst_fp_compare(i) || inst_fp_cvt_to_int(i) || inst_fp_mv_to_int(i) || inst_fp_classify(i) )

// Instrucciones con operando imm
#define inst_imm(i) ((OPCODE(i)==OPCODE_OP_IMM) || (OPCODE(i)==OPCODE_OP_IMM_32) || (OPCODE(i)==OPCODE_AUIPC) || \
    (OPCODE(i)==OPCODE_LUI) || (OPCODE(i)==OPCODE_BRANCH) || (OPCODE(i)==OPCODE_JAL) || \
    (OPCODE(i)==OPCODE_JAL) || (OPCODE(i)==OPCODE_JALR))

// Instrucciones fp simples
#define inst_fp_s(i) (TIPOS(i).result==t_float)
// Instrucciones fp dobles
#define inst_fp_d(i) (TIPOS(i).result==t_double)

#define inst_int_Rfte1(i) (TIPOS(i).Rfte1==t_dword) || (TIPOS(i).Rfte1==t_word)
#define inst_int_Rfte2(i) (TIPOS(i).Rfte2==t_dword) || (TIPOS(i).Rfte2==t_word)
#define inst_fp_Rfte1(i) (TIPOS(i).Rfte1==t_float) || (TIPOS(i).Rfte1==t_double)
#define inst_fp_Rfte2(i) (TIPOS(i).Rfte2==t_float) || (TIPOS(i).Rfte2==t_double)
#define inst_fp_Rfte3(i) (TIPOS(i).Rfte3==t_float) || (TIPOS(i).Rfte3==t_double)
#define inst_to_Rint(i) (TIPOS(i).result==t_dword)
#define inst_to_Rfp(i) (TIPOS(i).result==t_float) || (TIPOS(i).result==t_double)


// OP_TRAP es una instrucción "interna" que se genera en caso de excepción
#define OP_TRAP   1001


// MIPS 32
#define OP_FP_ADD_PS  10154
#define OP_FP_SUB_PS  10155
#define OP_FP_MUL_PS  10156
#define OP_FP_DIV_PS  10157

#define OP_FP_L_PS  10180 // No existe en MIPS64
#define OP_FP_S_PS  10181 // No existe en MIPS64

/***** Prototypes *********************************************/

#ifndef CPROTO
#include "instrucciones_api.h"
#endif

#endif /* instrucciones_H */

/**
 * @file tipos.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las definiciones de tipos comunes
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef tipos_H
#define tipos_H

/***** Includes ***********************************************/

#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

/***** Constants **********************************************/

/*** Excepciones */

#define EXC_NONE                0

#define EXC_OUT_MEM_INST        1
#define EXC_OUT_MEM_DAT         2
#define EXC_UNALIGN_MEM_INST    3
#define EXC_UNALIGN_MEM_DAT     4

#define EXC_USER                32
#define EXC_TRAP(t)  (EXC_USER+(t))
#define EXC_SYSCALL            255
#define excepcion_fatal(i) ((i) == EXC_OUT_MEM_INST  || (i) == EXC_OUT_MEM_DAT  || (i) == EXC_UNALIGN_MEM_INST || (i) == EXC_UNALIGN_MEM_DAT || (i) == EXC_TRAP(0))
#define excepcion_syscall(i) ((i) == EXC_SYSCALL)

/* Juegos de instrucciones implementados */
#define RV32I   0
#define RV32M   1
#define RV32F   2
#define RV32D   3
#define RV32A   4
#define RV64I   5
#define RV64M   6
#define RV64F   7
#define RV64D   8
#define RV64A   9
#define PSEUDO 10

#define SUPPORT_RV64I

#ifdef SUPPORT_RV64I
#define XLEN 64
#else
#define XLEN 32
#endif

/* Registros para las .ps */
#define Lo 0
#define Hi 1

/* Rounding Mode Mnemonic Meaning */
#define RNE 0b000 // Round to Nearest, ties to Even
#define RTZ 0b001 // Round towards Zero
#define RDN 0b010 // RDN // Round Down (towards −∞)
#define RUP 0b011 // Round Up (towards +∞)
#define RMM 0b100 // RMM // Round to Nearest, ties to Max Magnitude
#define DYN 0b111 // In instruction’s rm field, selects dynamic rounding mode;

/***** Macros **************************************************/

#ifndef MAX
#define MAX(x, y) ( (x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ( (x) < (y) ? (x) : (y))
#endif

#define foreach(_i, _limit) for ((_i) = 0; (_i) < (_limit); (_i)++)
#define forall(_i, _name) for (_i = INICIO_##_name; _i <= FIN_##_name; _i++)

/***** Types **************************************************/

/* Esta definición se le atraganta al cproto. Alternativa: */
//typedef unsigned char ubyte;
#define ubyte uint8_t   /* Un byte: 8 bits */
typedef int8_t byte;    /* Un byte: 8 bits */
typedef int16_t half;   /* Media palabra: 16 bits */
typedef int32_t word;   /* Una palabra: 32 bits */
typedef uint32_t uword; /* Una palabra: 32 bits sin signo */
typedef int64_t dword;  /* Una doble palabra: 64 bits */
typedef uint64_t udword; /* Una doble palabra: 64 bits sin signo */

/* Esta definición se le atraganta al cproto. Alternativa: */
// typedef __int128 qword;
#define qword __int128 /* Una cuadruple palabra: 128 bits */
// typedef unsigned __int128 uqword;
#define uqword unsigned __int128 /* Una cuadruple palabra: 128 bits sin signo */

#if XLEN == 64
typedef dword xword; // XLEN=64
typedef udword uxword;
#else
typedef word xword; // XLEN=32
typedef uword uxword;
#endif

typedef uint32_t ciclo_t;

#define MAX_BYTE    (0xFF)
#define MAX_HALF    (0xFFFF)
#define MAX_WORD    (0xFFFFFFFF)
#define MAX_DWORD   (0xFFFFFFFFFFFFFFFF)

#define MAX_CICLO UINT32_MAX

#define MASK_N_BITS(n) ((1LL<<(n))-1)
#define MAX_N_BITS(n) ((1LL<<(n))-1)
#define MIN_N_BITS(n) (-(1LL<<(n)))

#define MAX_IMM     MAX_N_BITS(11)
#define MIN_IMM     MIN_N_BITS(11)

#define MAX_IMM_20     MAX_N_BITS(20)
#define MIN_IMM_20     MIN_N_BITS(20)

#define F_BYTE "%" PRId8
#define F_HALF "%" PRId16
#define F_WORD "%" PRId32
#define F_DWORD "%" PRId64

#define F_CICLO "%" PRIu32
#define F_LCICLO "%03" PRIu32

#define F_XCODE "%08" PRIx32

#define TAG_STR 64
#define L_ETIQ 128

#define SHORT_STR 256
#define MID_STR (2*SHORT_STR)
#define MEDIUM_STR MID_STR
#define LONG_STR (6*SHORT_STR)

/* Predictor */
#define MAX_BITS_BHSR 8
#define MAX_TABLA_BHSR 16


typedef enum {
    NO = 0,
    SI = 1
} boolean; /* Valor lógico */

typedef enum {
    t_ninguno,
    t_char,
    t_byte,
    t_half,
    t_word,
    t_dword,
    t_float,
    t_float_ps,
    t_double,

    t_code // Representa una instrucción
} tipo_t; /* Tipo de dato utilizado */

typedef union {
    //byte int_b;
    //half int_h;
    xword int_d; /* 32 o 64 bits */
    float fp_s;
    float fp_ps[2];
    double fp_d;
} valor_t; /* Dato utilizado */

typedef enum {
    reg_auto,
    reg_ar,
    reg_abi
} modo_reg_t;

typedef enum {
    format_full,
    format_compact,
    format_none
} modo_formato_t;

typedef int codop_t; /* Código de operación en la tabla de instrucciones */

typedef enum {
    FormatoR,
    FormatoI,
    FormatoIshift,
    FormatoS,
    FormatoB,
    FormatoU,
    FormatoJ,
    FormatoR4rm, // RRRR rounding mode
    FormatoR3rm, // RRR rounding mode       
    FormatoR2rm  // RR rounding mode
} formato_t;

/*** Formato de instruccion ****/

typedef struct {
    codop_t codop;
    formato_t tipo;

    ubyte rs1; /* Índice del registro fuente 1 */
    boolean rs1_usa_abi;  /* Índica si el programa usa el registro ABI o el formato numérico */

    ubyte rs2; /* Índice del registro fuente 2 */
    boolean rs2_usa_abi;  /* Índica si el programa usa el registro ABI o el formato numérico */

    ubyte rs3; /* Índice del registro fuente 3 */
    boolean rs3_usa_abi;  /* Índica si el programa usa el registro ABI o el formato numérico */

    ubyte rm; /* Índice del modo de redondeo */

    ubyte rd; /* Índice del registro destino */
    boolean rd_usa_abi;  /* Índica si el programa usa el registro ABI o el formato numérico */

    dword imm; /* Valor Inmediato. RiscV R32I usa hasta 20 bits */

    dword PC; /* Posición de memoria de la instrucción */
    uword valor; /* Codificación */

    char etiqueta[L_ETIQ]; /* Posible etiqueta */
} instruccion_t;


/*** CSRs ***/

typedef word CSR_t[4096];

/* Predictor */
typedef enum {
    NO_SALTA,
    NO_SALTA_UN_FALLO,
    SALTA_UN_FALLO,
    SALTA
} estado_predic_t; /* Estado del predictor de 2 bits */



typedef enum {
    HYBRID_n_BIT_S,
    HYBRID_n_BIT_H,
    TWO_LEVEL_n_BIT_S,
    TWO_LEVEL_n_BIT_H,
    BTB_n_BIT_S,
    BTB_n_BIT_H,
    BTB_1_BIT,
    BTB_2_BIT_H,
    BTB_2_BIT_S,
    PERFECTO,
    CLARIVIDENTE,
    PREDICT_NOT_TAKEN
} tipo_predictor_t; /* Tipos de predictor */

typedef struct {
    word pred1; /* Estado pred1 */
    word pred2; /* Estado pred2 */
    word BHSR1; /* BHSR */
    word BHSR2; /* BHSR */
    word selector; /* Predictor hibrido */
    word entrada; /* Entrada BTB */
    dword dir_destino; /* Direccion destino predicha */
} prediccion_t;


/*** Branch Target Buffer ******/
#define pwrtwo(x) (1 << (x))

typedef struct {
    dword PC; /* Dirección de la instrucción de salto */
    estado_predic_t estado; /* Estado del predictor */
    dword destino; /* Dirección de destino */

    /* Predictor dos niveles */
    word BHSR1[1 << MAX_BITS_BHSR]; /* Vector de Estado del predictor */

    /* Predictor dos niveles */
    word BHSR2[1 << MAX_BITS_BHSR]; /* Vector de Estado del predictor */

    /* Predictor híbrido */
    word estado_selector; /* Estado del selector local/global */

    ciclo_t orden; /* Antigüedad de la última consulta */
} entrada_btb_t;


/*** Estadísticas ***/

typedef struct {
    long ciclos;
    long instrucciones;
    long enteras;
    long multiciclo;
    long flops;
    long saltos_accedidos;
    long saltos_encontrados;
    long saltos_ejecutados;
    long saltos_acertados;
} estadisticas_t;

/*** Estructuras específicas ****/

// Ver archivo tipos_ruta.h
#include "tipos_ruta.h"

#endif /* end tipos.h */

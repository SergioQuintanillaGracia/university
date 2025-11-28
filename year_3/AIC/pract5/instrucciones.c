/**
 * @file instrucciones.c
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las funciones necesarias para la codificar, decodificar e imprimir las
 *    instrucciones
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#define instrucciones_C

// Colores
#define COL_IMM "#6200ea"
#define COL_IMM1 "#0d47a1"
#define COL_IMM2 "#1565c0"
#define COL_IMM3 "#1976d2"
#define COL_IMM4 COL_IMM
#define COL_RS1 "#827717"
#define COL_RS2 "#00c853"
#define COL_RS3 "#afb42b"
#define COL_RD "#1b5e20"
#define COL_F3 "#ea80fc"
#define COL_F7 "#e040fb"
#define COL_OP "#ff5252"
#define COL_RM "#90a4ae"

// Tiene que ser un entero
#define BITSIZE_FULL    11
#define BITSIZE_COMPACT 8

/***** Includes ***********************************************/

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "instrucciones.h"
#include "memoria.h"
#include "simbolos.h"
#include "registros.h"
#include "g_string.h"

/***** Funciones Exportables **********************************/

int bloque_instruccion(
        dword PC
) {
    int index;

    if (PC == -1) {
        return -1;
    }

    ts_busca_direccion(programa.simbolos, PC, &index);
    index %= NUM_STYLES;

    return index;
}

/***************************************************************
 *
 * Func: color_instruccion
 *
 * Desc: Devuelve un color
 *
 **************************************************************/

void color_instruccion(
        char *color,
        dword PC
) {
    static char *paleta[NUM_STYLES] = {
            "darkred", "darkcyan", "darkblue", "darkolivegreen", "darkorange", "deeppink", "royalblue", "brown", "blue",
            "magenta", "mediumblue", "darkslategray", "steelblue", "darkviolet", "forestgreen", "darkorchid"
    };

    int index = bloque_instruccion(PC);

    if (index < 0) {
        sprintf(color, "lightgray");
        return;
    } /* endif */

    strcpy(color, paleta[index]);

} /* end color_instruccion */

/**
 * Convierte a binario, dejando un espacio cada 4 bits
 *
 * @param n Valor a convertir
 * @param buffer Resultado
 * @param digits Num de digitos
 */
void itobin4(dword n, char *buffer, int digits) {
    int i, l, size;
    unsigned int q;

    // Número de dígitos más los espacios en blanco cada 4 dígitos
    size = digits + ((digits - 1) / 4);
    q = n;
    buffer[size] = '\0';
    for (i = 0, l = size - 1; i < digits; i++, l--) {
        buffer[l] = (q % 2 == 0) ? '0' : '1';
        q >>= 1;
        if (((i + 1) % 4) == 0) {
            l--;
            buffer[l] = ' ';
        }
    }
}

/**
 * Convierte a binario
 *
 * @param n Valor a convertir
 * @param buffer Resultado
 * @param digits Num de digitos
 */
void itobin(dword n, char *buffer, int digits) {
    int i, l, size;
    unsigned int q;

    size = digits;
    q = n;
    buffer[size] = '\0';
    for (i = 0, l = size - 1; i < digits; i++, l--) {
        buffer[l] = (q % 2 == 0) ? '0' : '1';
        q >>= 1;
    }
}


/***************************************************************
 *
 * Func: fn_riscv_decode
 *
 * Desc: Decodificador RISC-V
 *
 **************************************************************/

codop_t fn_riscv_decode(dword PC, unsigned int instr, formato_t *fmt, int *opcode,
                        int *rs1, int *rs2, int *rs3, int *rd, int *rm, int *imm,
                        char *nemo, char *full) {

    static char *modos_redondeo[5] = {
            "rne", "rtz", "rdn", "rup", "rmm"
    };
    int codop;
    int sub_fmt = 0;
    char etiqueta[LONG_STR];
    char desplazamiento[LONG_STR];
    char *p_nemo;

    formato_instruccion_t formato;

    modo_reg_t modo_reg;

    // strings que contienen los registros
    char i_rs1[10];
    char i_rs2[10];
    char i_rs3[10];
    char i_rd[10];
    char f_rs1[10];
    char f_rs2[10];
    char f_rs3[10];
    char f_rd[10];

    /* Representación de la instrucción decodificada para la representación de los registros */
    instruccion_t instr_asm;
    boolean valid_PC = comprueba_direccion_instruc(mmem, PC);

    desplazamiento[0] = '\0';
    if (valid_PC) {
        instr_asm = lee_mem_instruc(mmem, PC);
        if (strlen(instr_asm.etiqueta)) {
            sprintf(desplazamiento, " [%s]", instr_asm.etiqueta);
        }
    }

    formato.valor = instr;

    // El opcode es común para todos los formatos
    *opcode = formato.opcode;

    // R-format
    // Los registros están siempre en las mismas posiciones
    *rd = formato.R.rd;
    *rs1 = formato.R.rs1;
    *rs2 = formato.R.rs2;
    *rs3 = formato.R4rm.rs3;

    // I-format
    int imm_i = formato.I.imm11_0;
    int shamt_i = formato.Ishift.shamt;

    // S-format
    int imm_s = formato.S.imm11_5 << 5 | formato.S.imm4_0;

    // B-format
    int imm_b = formato.B.imm12 << 12 | formato.B.imm11 << 11 | formato.B.imm10_5 << 5 | formato.B.imm4_1 << 1;

    // U-format
    // No se desplaza porque lo que queremos almacenar es HI20
    int imm_u = formato.U.imm31_12;

    // J-format
    int imm_j = formato.J.imm20 << 20 | formato.J.imm19_12 << 12 | formato.J.imm11 << 11 | formato.J.imm10_1 << 1;

#define abi_reg_int(reg) sprintf(i_ ## reg, "%s", nombre_int_reg[*(reg)])
#define abi_reg_fp(reg) sprintf(f_ ## reg, "%s", nombre_fp_reg[*(reg)])

#define ar_reg_int(reg) sprintf(i_ ## reg, "x%d", *(reg))
#define ar_reg_fp(reg) sprintf(f_ ## reg, "f%d", *(reg))

#define auto_reg_int(i_asm, reg) \
    do { \
        if ((i_asm).reg ## _usa_abi) { \
            abi_reg_int(reg); \
        } else { \
            ar_reg_int(reg); \
        } \
    } while (0)

#define auto_reg_fp(i_asm, reg) \
    do { \
        if ((i_asm).reg ## _usa_abi) { \
            abi_reg_fp(reg); \
        } else { \
            ar_reg_fp(reg); \
        } \
    } while (0)

    if (!valid_PC) {
        modo_reg = reg_ar;
    } else {
        modo_reg = OPTION(show_abi_reg);
    }

    switch (modo_reg) {
        case reg_auto:
            auto_reg_int(instr_asm, rs1);
            auto_reg_int(instr_asm, rs2);
            auto_reg_int(instr_asm, rs3);
            auto_reg_int(instr_asm, rd);

            auto_reg_fp(instr_asm, rs1);
            auto_reg_fp(instr_asm, rs2);
            auto_reg_fp(instr_asm, rs3);
            auto_reg_fp(instr_asm, rd);
            break;
        case reg_ar:
            ar_reg_int(rs1);
            ar_reg_int(rs2);
            ar_reg_int(rs3);
            ar_reg_int(rd);

            ar_reg_fp(rs1);
            ar_reg_fp(rs2);
            ar_reg_fp(rs3);
            ar_reg_fp(rd);
            break;
        case reg_abi:
            abi_reg_int(rs1);
            abi_reg_int(rs2);
            abi_reg_int(rs3);
            abi_reg_int(rd);

            abi_reg_fp(rs1);
            abi_reg_fp(rs2);
            abi_reg_fp(rs3);
            abi_reg_fp(rd);
            break;
    }

    sprintf(nemo, "unknown");
    sprintf(full, "unknown %0x %0x %0x %0x",
            formato.decode.opcode, formato.decode.funct7, formato.decode.funct5, formato.decode.funct3);

    if (!valid_PC) {
        *imm = 0;
        return RV32I_ADDI;
    }
    codop = obtener_decodificacion(PC, formato.decode, &p_nemo, &sub_fmt, fmt);

    strcpy(nemo, p_nemo);
    *imm = 0;
    *rm = formato.Rfp.rm;

    switch (sub_fmt) {
        case FMT_INT_RRI:
            *imm = imm_i;
            sprintf(full, "%s %s,%s,%d%s", nemo, i_rd, i_rs1, *imm, desplazamiento);
            break;
        case FMT_INT_RRSH:
            *imm = shamt_i;
            sprintf(full, "%s %s,%s,%d%s", nemo, i_rd, i_rs1, *imm, desplazamiento);
            break;
        case FMT_INT_RI:
            *imm = imm_u;
            sprintf(full, "%s %s,0x%x%s", nemo, i_rd, *imm, desplazamiento);
            break;
        case FMT_INT_RA:
            *imm = imm_u;
            sprintf(full, "%s %s,%d%s", nemo, i_rd, *imm, desplazamiento);
            break;
        case FMT_INT_RRR:
            sprintf(full, "%s %s,%s,%s", nemo, i_rd, i_rs1, i_rs2);
            break;
        case FMT_J_RA:
            *imm = imm_j;
            ts_simbolo_direccion(programa.simbolos,
                                 PC + (*imm), etiqueta, FLAG(etiquetas));
            sprintf(full, "%s %s,%d [%s]", nemo, i_rd, *imm, etiqueta);
            break;
        case FMT_J_RRA:
            *imm = imm_i;
            sprintf(full, "%s %s,%s,%d%s", nemo, i_rd, i_rs1, *imm, desplazamiento);
            break;
        case FMT_INT_B_RRA:
            *imm = imm_b;
            ts_simbolo_direccion(programa.simbolos,
                                 PC + (*imm), etiqueta, FLAG(etiquetas));
            sprintf(full, "%s %s,%s,%d [%s]", nemo, i_rs1, i_rs2, *imm, etiqueta);
            break;
        case FMT_INT_L_RSB:
            *imm = imm_i;
            sprintf(full, "%s %s,%d(%s)%s", nemo, i_rd, *imm, i_rs1, desplazamiento);
            break;
        case FMT_INT_S_RSB:
            *imm = imm_s;
            sprintf(full, "%s %s,%d(%s)%s", nemo, i_rs2, *imm, i_rs1, desplazamiento);
            break;
        case FMT_INT_RB:
            sprintf(full, "%s %s,(%s)", nemo, i_rd, i_rs1);
            break;
        case FMT_INT_RRB:
            sprintf(full, "%s %s,%s,(%s)", nemo, i_rd, i_rs1, i_rs2);
            break;
        case FMT_CSR_RxR:
            fprintf(stderr, "error: Subformato (%d) no implementado", sub_fmt);
            exit(1);
            break;
        case FMT_CSR_RxI:
            fprintf(stderr, "error: Subformato (%d) no implementado", sub_fmt);
            exit(1);
            break;
        case FMT_INT_ECALL:
        case FMT_INT_EBREAK:
            // *imm = imm_i;
            sprintf(full, "%s", nemo);
            break;
        case FMT_FP_L_FSB:
            *imm = imm_i;
            sprintf(full, "%s %s,%d(%s)%s", nemo, f_rd, *imm, i_rs1, desplazamiento);
            break;
        case FMT_FP_S_FSB:
            *imm = imm_s;
            sprintf(full, "%s %s,%d(%s)%s", nemo, f_rs2, *imm, i_rs1, desplazamiento);
            break;
        case FMT_FP_FFFFr:
            if (*rm == DYNAMIC_RM)
                sprintf(full, "%s %s,%s,%s,%s", nemo, f_rd, f_rs1, f_rs2, f_rs3);
            else
                sprintf(full, "%s %s,%s,%s,%s,%s", nemo, f_rd, f_rs1, f_rs2, f_rs3, modos_redondeo[*rm]);
            break;
        case FMT_FP_FFFr:
            if (*rm == DYNAMIC_RM)
                sprintf(full, "%s %s,%s,%s", nemo, f_rd, f_rs1, f_rs2);
            else
                sprintf(full, "%s %s,%s,%s,%s", nemo, f_rd, f_rs1, f_rs2, modos_redondeo[*rm]);
            break;
        case FMT_FP_FFr:
            if (*rm == DYNAMIC_RM)
                sprintf(full, "%s %s,%s", nemo, f_rd, f_rs1);
            else
                sprintf(full, "%s %s,%s,%s", nemo, f_rd, f_rs1, modos_redondeo[*rm]);
            break;
        case FMT_FP_FFF:
            sprintf(full, "%s %s,%s,%s", nemo, f_rd, f_rs1, f_rs2);
            break;
        case FMT_FP_FF:
            sprintf(full, "%s %s,%s", nemo, f_rd, f_rs1);
            break;
        case FMT_FP_RFr:
            if (*rm == DYNAMIC_RM)
                sprintf(full, "%s %s,%s", nemo, i_rd, f_rs1);
            else
                sprintf(full, "%s %s,%s,%s", nemo, i_rd, f_rs1, modos_redondeo[*rm]);
            break;
        case FMT_FP_RF:
            sprintf(full, "%s %s,%s", nemo, i_rd, f_rs1);
            break;
        case FMT_FP_RFF:
            sprintf(full, "%s %s,%s,%s", nemo, i_rd, f_rs1, f_rs2);
            break;
        case FMT_FP_FRr:
            if (*rm == DYNAMIC_RM)
                sprintf(full, "%s %s,%s", nemo, f_rd, i_rs1);
            else
                sprintf(full, "%s %s,%s,%s", nemo, f_rd, i_rs1, modos_redondeo[*rm]);
            break;
        case FMT_FP_FR:
            sprintf(full, "%s %s,%s", nemo, f_rd, i_rs1);
            break;
        default:
            fprintf(stderr, "error: Invalid subformat (%d)", sub_fmt);
            exit(1);
    } // end format

    return codop;
}

void imprime_instruccion_color(
        char *instruc,
        dword PC,
        boolean encolor
) {
    char color[LONG_STR];
    instruccion_t instr;

    if (PC == -1) {
        sprintf(instruc, "-nop-");
    } else {
        int indice = indice_instruccion(mmem, PC);
        if (indice != INST_UNKNOWN) {
            char * texto = obtiene_ensamblador(mmem, indice);

            if (strlen(texto) == 0) {
                char nemotecnico[SHORT_STR];
                int opcode, rs1, rs2, rs3, rd, rm, imm;
                formato_t fmt;

                instr = obtiene_instruccion(mmem, indice);

                fn_riscv_decode(PC, instr.valor, &fmt, &opcode, &rs1, &rs2, &rs3, &rd, &rm, &imm,
                                nemotecnico, texto);
            }

            if (encolor == SI) {
                color_instruccion(color, PC);
                sprintf(instruc, "<font color=\"%s\">%s</font>", color, texto);
            } else {
                sprintf(instruc, "%s", texto);
            }
        } else {
            strcpy(instruc, "unknown");
        }
    }

} /* end imprime_instruccion_color */

void extrae_campo(char *texto, uword valor, int b2, int b1) {
    int masc;
    int campo;
    int n_bits = b2 - b1 + 1;

    masc = (1 << n_bits) - 1;
    campo = valor >> b1;
    campo = campo & masc;

    itobin(campo, texto, n_bits);
}

/**
 * Obtiene el formato de un campo de la instrucción
 *
 * @param binario
 * @param campos
 * @param dump
 * @param texto
 * @param color
 * @param b2
 * @param b1
 */
void muestra_campo(
        g_string_t *binario, g_string_t *campos, uword dump,
        char *texto, char *color, int b2, int b1) {

    char aux[SHORT_STR];
    int n_bits = b2 - b1 + 1;

    extrae_campo(aux, dump, b2, b1);

    switch (OPTION(show_format)) {
        case format_full:
            g_str_printf(binario,
                         "<span style='color:%s' title='%s'>%s</span>", color, texto, aux);
            if (n_bits == 1) {
                g_str_printf(campos,
                             "<span class='field' style='color: %s; border-color: %s; width: %dpx' title='%s'>%c</span>",
                             color, color, n_bits * BITSIZE_FULL - 2, texto, texto[0]);
            } else {
                g_str_printf(campos,
                             "<span class='field' style='color: %s; border-color: %s; width: %dpx'>%s</span>",
                             color, color, n_bits * BITSIZE_FULL - 2, texto);

            }
            break;
        case format_compact:
            g_str_printf(campos,
                         "<span class='field' style='color: %s; border-color: %s; width: %dpx' title='%s'>%s</span>",
                         color, color, n_bits * BITSIZE_COMPACT - 2, texto, aux);
            break;
        default:
            ;
    }

}

void muestra_formato(
        g_string_t *resultado,
        formato_t formato,
        uword dump
) {

    G_STR_DECL(binario);
    G_STR_DECL(campos);

    g_str_clear(resultado);

    switch (formato) {
        case FormatoR:
            g_str_copy(&campos, "R ");
            muestra_campo(&binario, &campos, dump, "f7", COL_F7, 31, 25);
            muestra_campo(&binario, &campos, dump, "rs2", COL_RS2, 24, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoR4rm:
            g_str_copy(&campos, "R ");
            muestra_campo(&binario, &campos, dump, "rs3", COL_RS3, 31, 27);
            muestra_campo(&binario, &campos, dump, "f7", COL_F7, 26, 25);
            muestra_campo(&binario, &campos, dump, "rs2", COL_RS2, 24, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "rm", COL_RM, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoR3rm:
        case FormatoR2rm:
            g_str_copy(&campos, "R ");
            muestra_campo(&binario, &campos, dump, "f7", COL_F7, 31, 25);
            muestra_campo(&binario, &campos, dump, "rs2", COL_RS2, 24, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "rm", COL_RM, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoI:
            g_str_copy(&campos, "I ");
            muestra_campo(&binario, &campos, dump, "imm", COL_IMM, 31, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoIshift:
            g_str_copy(&campos, "I ");
            muestra_campo(&binario, &campos, dump, "f6", COL_F7, 31, 26);
            muestra_campo(&binario, &campos, dump, "shamt", COL_IMM, 25, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoS:
            g_str_copy(&campos, "S ");
            muestra_campo(&binario, &campos, dump, "i[11:5]", COL_IMM1, 31, 25);
            muestra_campo(&binario, &campos, dump, "rs2", COL_RS2, 24, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "i[4:0]", COL_IMM2, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoB:
            g_str_copy(&campos, "B ");
            muestra_campo(&binario, &campos, dump, "i[12]", COL_IMM1, 31, 31);
            muestra_campo(&binario, &campos, dump, "i[10:5]", COL_IMM2, 30, 25);
            muestra_campo(&binario, &campos, dump, "rs2", COL_RS2, 24, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "i[4:1]", COL_IMM3, 11, 8);
            muestra_campo(&binario, &campos, dump, "i[11]", COL_IMM4, 7, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoU:
            g_str_copy(&campos, "U ");
            muestra_campo(&binario, &campos, dump, "imm", COL_IMM, 31, 20);
            muestra_campo(&binario, &campos, dump, "rs1", COL_RS1, 19, 15);
            muestra_campo(&binario, &campos, dump, "f3", COL_F3, 14, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;
        case FormatoJ:
            g_str_copy(&campos, "J ");
            muestra_campo(&binario, &campos, dump, "i[20]", COL_IMM1, 31, 31);
            muestra_campo(&binario, &campos, dump, "i[10:1]", COL_IMM4, 30, 21);
            muestra_campo(&binario, &campos, dump, "i[11]", COL_IMM3, 20, 20);
            muestra_campo(&binario, &campos, dump, "i[19:12]", COL_IMM2, 19, 12);
            muestra_campo(&binario, &campos, dump, "rd", COL_RD, 11, 7);
            muestra_campo(&binario, &campos, dump, "op", COL_OP, 6, 0);
            break;

        default:
            muestra_campo(&binario, &campos, dump, "todo", "red", 31, 0);

    }

    if (OPTION(show_format) == format_full) {
        g_str_printf(resultado, "%s %s",
                     g_str_string(&binario), g_str_string(&campos));
    } else {
        g_str_copy(resultado, g_str_string(&campos));
    }
}

void imprime_formato(
        g_string_t *formato,
        dword PC
) {

    int opcode, rs1, rs2, rs3, rd, rm, imm;
    formato_t fmt;
    char nemotecnico[SHORT_STR];
    instruccion_t instr;

    G_STR_DECL(formato_campos);

    g_str_clear(formato);

    if (PC == -1) {
        g_str_copy(formato, "-nop-");
    } else {
        int indice = indice_instruccion(mmem, PC);
        if (indice != INST_UNKNOWN) {
            char * texto = obtiene_ensamblador(mmem, indice);

            instr = obtiene_instruccion(mmem, indice);

            fn_riscv_decode(PC, instr.valor, &fmt, &opcode, &rs1, &rs2, &rs3, &rd, &rm, &imm,
                            nemotecnico, texto);

            muestra_formato(&formato_campos, fmt, instr.valor);

            g_str_printf(formato, F_XCODE " | %s",
                         (unsigned int) instr.valor, g_str_string(&formato_campos));

        } else {
            g_str_copy(formato, "unknown");
        }
    }

} /* end imprime_formato */

void genera_MInstruccion() {
    instruccion_t inst;

    codop_t codop;
    dword i;
    formato_instruccion_t formato;

    for (i = inicio_region(mmem, r_text); i < final_region(mmem, r_text); i += 4) {
        inst = lee_mem_instruc(mmem, i);
        codop = inst.codop;

        // Inicializa todos los campos que dependen de codop: opcode y los campos functX
        formato.decode = DECODE(codop);
        switch (inst.tipo) {
            case FormatoR:
                formato.R.rd = inst.rd;
                // formato.R.funct3 = FUNC3(codop);
                formato.R.rs1 = inst.rs1;
                formato.R.rs2 = inst.rs2;
                // formato.R.funct7 = FUNC7(codop);
                break;
            case FormatoR4rm:
                formato.R4rm.rd = inst.rd;
                formato.R4rm.rm = inst.rm;
                formato.R4rm.rs1 = inst.rs1;
                formato.R4rm.rs2 = inst.rs2;
                // formato.R4rm.fmt = FUNC7(codop) & 0b11; // Mascara innecesaria
                formato.R4rm.rs3 = inst.rs3;
                break;
            case FormatoR3rm:
            case FormatoR2rm:
                formato.Rfp.rd = inst.rd;
                formato.Rfp.rm = inst.rm;
                formato.Rfp.rs1 = inst.rs1;
                formato.Rfp.rs2 = inst.rs2; // En R2rm inst.rs2 contiene FUNC5
                // formato.Rfp.fmt = FUNC7(codop) & 0b11; // Mascara innecesaria
                // formato.Rfp.funct5 = FUNC7(codop) >> 2;
                break;
            case FormatoI:
                formato.I.rd = inst.rd;
                // formato.I.funct3 = FUNC3(codop);
                formato.I.rs1 = inst.rs1;
                formato.I.imm11_0 = (int) inst.imm;
                break;
            case FormatoIshift:
                formato.Ishift.rd = inst.rd;
                // formato.Ishift.funct3 = FUNC3(codop); // En I_fp = width
                formato.Ishift.rs1 = inst.rs1;
                formato.Ishift.shamt = (unsigned int) inst.imm;
                // formato.Ishift.funct6 = FUNC7(codop) >> 1;
                break;
            case FormatoJ:
                formato.J.rd = inst.rd;
                // El bit 0 no está. Es inst.imm[20:1]
                formato.J.imm19_12 = (inst.imm & 0b01111111100000000000) >> 11;
                formato.J.imm11 = (inst.imm & 0b00000000010000000000) >> 10;
                formato.J.imm10_1 = (inst.imm & 0b00000000001111111111);
                formato.J.imm20 = (int) (inst.imm & 0b10000000000000000000) >> 19;
                break;
            case FormatoS:
                formato.S.imm4_0 = (inst.imm & 0b000000011111);
                // formato.S.funct3 = FUNC3(codop);
                formato.S.rs1 = inst.rs1;
                formato.S.rs2 = inst.rs2;
                formato.S.imm11_5 = (int) (inst.imm & 0b111111100000) >> 5;
                break;
            case FormatoB:
                // El bit 0 no está. Es inst.imm[12:1]
                formato.B.imm11 = (inst.imm & 0b010000000000) >> 10;
                formato.B.imm4_1 = (inst.imm & 0b000000001111);
                // formato.B.funct3 = FUNC3(codop);
                formato.B.rs1 = inst.rs1;
                formato.B.rs2 = inst.rs2;
                formato.B.imm10_5 = (inst.imm & 0b001111110000) >> 4;
                formato.B.imm12 = (int) (inst.imm & 0b100000000000) >> 11;
                break;
            case FormatoU:
                formato.U.rd = inst.rd;
                formato.U.imm31_12 = (int) inst.imm;
                break;
        }
        inst.valor = formato.valor;
        escribe_mem_instruc(mmem, i, inst);
    }

}


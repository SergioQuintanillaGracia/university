%{
    /**************************************************************************
     *
     * Departamento de Informática de Sistema y Computadores (DISCA)
     * Universidad Politécnica de Valencia.
     *
     * Author: Sergio Sáez (ssaez@disca.upv.es)
     *             Pedro López (plopez@disca.upv.es)
     *
     * File: input.yacc.c
     *
     * Date: Tue Feb 18 10:00:00 GMT 2020
     *
     * Description:
     *    Contiene la descripción sintáctica del lenguaje ensamblador
     *
     * This program is free software; you can redistribute it and/or modify
     * it under the terms of the GNU General Public License as published by
     * the Free Software Foundation; either version 2 of the License, or
     * (at your option) any later version.
     *
     * This program is distributed in the hope that it will be useful,
     * but WITHOUT ANY WARRANTY; without even the implied warranty of
     * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     * GNU General Public License for more details.
     *
     * You should have received a copy of the GNU General Public License
     * along with this program; if not, write to the Free Software
     * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
     *
     *************************************************************************/

    /***** Includes ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "main.h"
#include "tipos.h"
#include "instrucciones.h"
#include "codop.h"
#include "memoria.h"
#include "registros.h"
#include "enlazador.h"

    /*** Declaraciones externas */

    extern FILE * yyin;
    extern int yyerror(const char *);
    extern void free(void*);
    extern int yylex();

   void nueva_instruccion(codop_t codop, formato_t tipo,
    			   int Rdestino, int Rfuente1, int Rfuente2, int Rfuente3,
    			   dword inmediato, char * etiqueta, int Redondeo);

    /*** Variables globales */

    char mensaje[256];
    long linenumber = 1;
    char desplazamiento[256];
    int hay_valor=0;
    int hay_etiq=0;

    tipo_t tipo = t_dword;

    dword pos[num_regiones];
    region_t region = r_invalid;
    mapa_memoria_t * mm = NULL;
    tabla_simbolos_t * ts = NULL;

    int idx_ireg = 1;
    int idx_fpreg = 0;
    int idx_btb = 0;

    boolean no_ireg = NO;
    boolean no_fpreg = NO;
    boolean no_btb = NO;

    int i = 0;
    instruccion_t instruccion;
    valor_t valor;
    valor_t reg_fp;
    int RV_instruccion[5];

    int pending_label = 0;
    region_t pending_region = r_invalid;

    int offset;
    int symbol;
    char etiqueta[256];
    
    int Redondeo_id (char *rm) {
        if (strcmp(rm,"rne")==0)
            return 0;
        else if (strcmp(rm,"rtz")==0)
            return 1;
        else if (strcmp(rm,"rdn")==0)
            return 2;
        else if (strcmp(rm,"rup")==0)
            return 3;
        else if (strcmp(rm,"rmm")==0)
            return 4;
        else return -1;
    }
	
	
    /*****************************************************************************/

    %}

%define parse.error verbose

%union {
    double fvalue;
    int64_t ivalue;
    char cvalue[128];
}

%token <ivalue> NL
%token <ivalue> T_INT_LIT
%token <fvalue> T_FP_LIT
%token <cvalue> T_ALPHANUM
%token <cvalue> T_STRING
%token <cvalue> FR_MODE
%token <ivalue> REG_INT REG_FP
%token <ivalue> DE_DATA DE_TEXT DE_SPACE DE_GLOBL
%token <ivalue> DE_IREG DE_FREG DE_ASCIIZ DE_BTB
%token <ivalue> DE_BYTE DE_HALF DE_WORD DE_DWORD
%token <ivalue> DE_FLOAT DE_DOUBLE
%token <ivalue> FORM_INT_RRI FORM_INT_RRSH FORM_INT_RRR FORM_INT_RA FORM_INT_RI
%token <ivalue> FORM_J_RA FORM_J_RRA FORM_INT_B_RRA FORM_J_A
%token <ivalue> FORM_INT_L_RSB FORM_INT_S_RSB
%token <ivalue> FORM_INT_ECALL FORM_INT_EBREAK
%token <ivalue> FORM_FP_FFF FORM_FP_FFFr FORM_FP_FFFFr FORM_FP_FFr
%token <ivalue> FORM_FP_RFF FORM_FP_RFr FORM_FP_RF FORM_FP_FR FORM_FP_FRr FORM_FP_FF
%token <ivalue> FORM_FP_L_FSB FORM_FP_S_FSB
%token <ivalue> FORM_INT_RB FORM_INT_RRB

%token <ivalue> INST_FENCE
%token <ivalue> INST_PRUEBA
%token <ivalue> FORM_CSR_RxR FORM_CSR_RxI
%token <ivalue> FORM_INM FORM_INM_L FORM_INM_S FORM_INM_DI FORM_INM_B FORM_INM_B_FI FORM_INM_B_FFI FORM_INM_T
%token <ivalue> FORM_PSEUDO_NOP FORM_PSEUDO_RR
%token <ivalue> FORM_PSEUDO_BRA
%token <ivalue> FORM_PSEUDO_J FORM_PSEUDO_JR FORM_PSEUDO_RET
%token <ivalue> FORM_PSEUDO_TAIL
%token <ivalue> FORM_PSEUDO_R


%token <ivalue> FORM_PSEUDO_CSRR FORM_PSEUDO_CSRW FORM_PSEUDO_CSRS FORM_PSEUDO_CSRC

%token <ivalue> FORM_PSEUDO_CSRWI FORM_PSEUDO_CSRSI FORM_PSEUDO_CSRCI FORM_PSEUDO_FRCSR FORM_PSEUDO_FSCSR FORM_PSEUDO_FRRM FORM_PSEUDO_FSRM
%token <ivalue> FORM_PSEUDO_FRFLAGS FORM_PSEUDO_FSFLAGS
%token <ivalue> FORM_PSEUDO_BRRA
%token <ivalue> FORM_PSEUDO_RRi
%token <ivalue> FORM_PSEUDO_FF
//%token <ivalue> FORM_PSEUDO_LA
%token <ivalue> FORM_PSEUDO_LLA
%token <ivalue> FORM_PSEUDO_L FORM_PSEUDO_S FORM_PSEUDO_FL FORM_PSEUDO_FS

%token <ivalue> FORM_PSEUDO_LI

%token <ivalue> FORM_PSEUDO_CALL


%token <ivalue> FORM_PSEUDO_TRAP

%token <ivalue> INST_RET INST_SYSCALL
%token <ivalue> FORM_REG FORM_REG_FF
%token <ivalue> FORM_INM_FP_L FORM_INM_FP_S
%token <ivalue> FORM_FP_REG FORM_FP_REG_FF FORM_FP_REG_DF
%token <ivalue> FORM_REG_R_FP FORM_REG_FP_R
%token <ivalue> FORM_J FORM_J_F
%token <cvalue> LABEL

%token <ivalue> PINST_LA PINST_LI
%token <ivalue> M_LO M_HI M_DISP
%token <ivalue> DE_PC FORM_INTERNAL_NOP

%type <ivalue> r_inmediato_hi r_inmediato_disp r_posicion r_posicion_larga r_offset

%start r_inicio

/*** Seccion de reglas *******************************************************/

%%

r_inicio : LABEL
{
    if (es_region_datos(region)) {
    	if (pending_label) {
            ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
            pending_label= 0;
        }
        // Las etiquetas en zonas de datos se quedan diferidas por si la dirección se tiene que alinear
        pending_label = 1;
        pending_region = region;
        sprintf (etiqueta, "%s", $1);
    } else if (es_region_codigo(region)) {
        // TODO revisar si esto es necesario
	if (pending_label) {
           ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
           pending_label= 0;
        }
        ts_inserta_etiqueta(ts, $1, pos[region], region);
        break;
    } else {
        yyerror("Etiqueta indefinida");
    } /* endif */}
r_linea_ensamblador
{}

| r_linea_ensamblador
{}

|
{
}
;

r_linea_ensamblador:
r_directiva NL
{}
r_inicio
{}

| r_instruccion NL
{}
r_inicio
{}

| NL r_inicio
{}

| r_directiva
{}

| r_instruccion
{}
;

r_directiva:
DE_DATA
{
    region = r_data;
}

| DE_TEXT
{
    region = r_text;
}

| DE_GLOBL T_ALPHANUM
{
    ts_inserta_simbolo(ts, $2);
}

| DE_BYTE
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_byte;
}
r_lista_enteros
{}

| DE_HALF
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_half;
}
r_lista_enteros
{}

| DE_WORD
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_word;
}
r_lista_enteros
{}

| DE_DWORD
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_dword;
}
r_lista_enteros
{}

| DE_FLOAT
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_float;
}
r_lista_reales
{}

| DE_DOUBLE
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
    tipo = t_double;
}
r_lista_reales
{}

| DE_SPACE T_INT_LIT
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }

    if (pending_label) {
        ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
        pending_label= 0;
    }
    pos[region] += $2;

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

| DE_ASCIIZ T_STRING
{
    if (!es_region_datos(region)) {
        sprintf(mensaje, "No estás en una sección de datos (%s)", mm->regiones[region].nombre);
        yyerror(mensaje);
    }

    if (!esta_alineada_dir_byte(pos[region]))
        pos[region] = mem_direccion_alineada(pos[region], sizeof(byte));
    if (pending_label) {
        ts_inserta_etiqueta(ts, etiqueta, pos[pending_region], pending_region);
        pending_label=0;
    }

    for (i = 0; i <= (strlen($2) + 1); i++) // Sumo 1 por el null terminated
    {
        valor.int_d = (byte) $2[i];
        mem_escribe_region_datos(mm, region, pos[region] + i, valor, t_byte, NO);
    }
    pos[region] += sizeof (byte) * strlen($2) + 1; // Sumo 1 por el null terminated

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

| DE_IREG
{
    if (no_ireg) {
    	sprintf(mensaje, "Los registros enteros ya han sido inicializados en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_enteros
{
  init_ireg = SI;
}

| DE_FREG
{
    if (no_fpreg) {
    	sprintf(mensaje, "Los registros de como flotante ya han sido inicializados en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_fp
{
  init_fpreg = SI;
}

| DE_BTB
{
    if (no_btb) {
    	sprintf(mensaje, "La BTB ya ha sido inicializada en otro módulo");
        yyerror(mensaje);
    }
}
r_valores_btb
{
  init_btb = SI;
}
;

r_lista_enteros:
T_INT_LIT
{
    switch (tipo) {
        case t_byte:
            if (!esta_alineada_dir_byte(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(byte));
            if ($1 > MAX_BYTE)
                yyerror("Literal fuera del rango del tipo 'byte'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (byte) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_byte, NO);
            pos[region] += sizeof (byte);
            break;
        case t_half:
            if (!esta_alineada_dir_half(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(half));
            if ($1 > MAX_HALF)
                yyerror("Literal fuera del rango del tipo 'half'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (half) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_half, NO);
            pos[region] += sizeof (half);
            break;
        case t_word:
            if (!esta_alineada_dir_word(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(word));
            if ($1 > MAX_WORD)
                yyerror("Literal fuera del rango del tipo 'word'");
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (word) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_word, NO);
            pos[region] += sizeof (word);
            break;
        case t_dword:
            if (!esta_alineada_dir_dword(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(dword));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.int_d = (dword) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_dword, NO);
            pos[region] += sizeof (dword);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_enteros_cont
{}
;

r_lista_enteros_cont:
',' r_lista_enteros
{}

|
{
}
;

r_lista_reales:
T_FP_LIT
{
    switch (tipo) {
        case t_float:
            if (!esta_alineada_dir_float(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(float));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.fp_s = (float) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_float, NO);
            pos[region] += sizeof (float);
            break;
        case t_double:
            if (!esta_alineada_dir_double(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(double));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
            valor.fp_d = (double) $1;
            mem_escribe_region_datos(mm, region, pos[region], valor, t_double, NO);
            pos[region] += sizeof (double);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_reales_cont
{}

| T_INT_LIT
{
    switch (tipo) {
        case t_float:
            if (!esta_alineada_dir_float(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(float));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
	    valor.fp_s = (float) $1;
	    mem_escribe_region_datos(mm, region, pos[region], valor, t_float, NO);
            pos[region] += sizeof (float);
            break;
        case t_double:
            if (!esta_alineada_dir_double(pos[region]))
                pos[region] = mem_direccion_alineada(pos[region], sizeof(double));
            if (pending_label) {
            	ts_inserta_etiqueta(ts, etiqueta, pos[region], r_data);
            	pending_label=0;
            }
	    valor.fp_d = (double) $1;
	    mem_escribe_region_datos(mm, region, pos[region], valor, t_double, NO);
            pos[region] += sizeof (double);
            break;
        default:
            yyerror("PÁNICO");
            break;
    } /* endswitch */

    if (pos[region] > tam_region[region]) {
    	sprintf(mensaje, "Excedido el tamaño de la memoria: %s", mm->regiones[region].nombre);
        yyerror(mensaje);
    }
}

r_lista_reales_cont
{}
;

r_lista_reales_cont:
',' r_lista_reales
{}

|
{
}
;

r_offset:
'+' T_INT_LIT
{
    $$ = $2;
}

|
'-' T_INT_LIT
{
    $$ = - ($2);
}

|
T_INT_LIT
{
    if ($1 >= 0) {
      yyerror("Syntax error. Se esperaba el operador aritmético '+' o '-' o el separador ','.");
    }
    $$ = $1;
}

|
{
    $$ = 0;
}
;

r_indice_ireg:
REG_INT '='
{
    idx_ireg = $1;
}

| {}
;

r_valores_enteros:
r_indice_ireg T_INT_LIT
{
    if (idx_ireg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
    if (idx_ireg < 0) {
        /*
        Es un registro en formato xnn
        El analizador lexico nos lo devuelve con un valor negativo para distinguirlo
        Hay que regenerar el valor correcto
        */
        idx_ireg  += TAM_REGISTROS;
    }
    escribe_int_reg(idx_ireg, (dword) $2, t_dword, NO);
    idx_ireg++;}

r_valores_enteros_cont
{}

|
r_indice_ireg T_ALPHANUM r_offset
{
    if (idx_ireg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
    ts_inserta_direccion(ts, $2, idx_ireg, $3, DIR_REG);
    idx_ireg++;}
r_valores_enteros_cont
{}
;

r_valores_enteros_cont:
',' r_valores_enteros
{}

|
{
}
;

r_indice_fpreg:
REG_FP '='
{
    idx_fpreg = $1;
}

| {}
;

r_valores_fp:
r_indice_fpreg T_FP_LIT
{
    if (idx_fpreg >= TAM_REGISTROS)
        yyerror("Excedido el numero de registros");
   if (idx_fpreg < 0) {
        /*
        Es un registro en formato xnn
        El analizador lexico nos lo devuelve con un valor negativo para distinguirlo
        Hay que regenerar el valor correcto
        */
        idx_fpreg  += TAM_REGISTROS;
    }

    reg_fp.fp_d= (double) $2;
    escribe_fp_reg(idx_fpreg, reg_fp, t_double, NO);
    idx_fpreg++;
    }
r_valores_fp_cont
{}
;

r_valores_fp_cont:
',' r_valores_fp
{}

|
{
}
;

r_valores_btb:
T_ALPHANUM r_offset '&' T_INT_LIT '&' T_ALPHANUM r_offset
{
   if (idx_btb >= TAM_BUFFER_PREDIC)
      yyerror("Excedido el número de entradas en la BTB");

   ts_inserta_direccion(ts, $1, idx_btb, $2, DIR_BTB_PC);
   BTB[idx_btb].orden = idx_btb+1;
   BTB[idx_btb].estado = $4;
   ts_inserta_direccion(ts, $6, idx_btb, $7, DIR_BTB_DEST);

   idx_btb++;
   }
;

r_inmediato_disp:
T_INT_LIT
{
    // sprintf(desplazamiento, F_DWORD, $1);
    desplazamiento[0] = '\0';
    $$ = $1;}

| T_ALPHANUM
{
    hay_valor=0;
    // Por omisión usa %%disp()
    if (f_macros)
        sprintf(desplazamiento,"%%disp(%s)",$1);
    else
        strcpy(desplazamiento, $1);
    ts_inserta_referencia(ts, $1, region, pos[region], DATA_REL_LO_12, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_HI '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%hi(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], ABS_HI_20, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_LO '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%lo(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], ABS_LO_12, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_DISP '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%disp(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], DATA_REL_LO_12, tr_data);
    $$ = 0; // Referencia por resolver
    }
;

r_inmediato_hi:
T_INT_LIT
{
    sprintf(desplazamiento, F_DWORD, $1);
    $$ = $1;}

| T_ALPHANUM
{
    // Por omisión usa %%hi()
    hay_valor=0;
    if (f_macros)
        sprintf(desplazamiento,"%%hi(%s)",$1);
    else
        strcpy(desplazamiento, $1);
    ts_inserta_referencia(ts, $1, region, pos[region], ABS_HI_20, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_HI '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%hi(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], ABS_HI_20, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_LO '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%lo(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], ABS_LO_12, tr_data);
    $$ = 0; // Referencia por resolver
    }

| M_DISP '[' T_ALPHANUM ']'
{
    hay_valor=0;
    sprintf(desplazamiento,"%%disp(%s)",$3);
    ts_inserta_referencia(ts, $3, region, pos[region], DATA_REL_LO_12, tr_data);
    $$ = 0; // Referencia por resolver
    }
;

r_posicion:
T_INT_LIT
{
    $$ = $1;
}

| T_ALPHANUM
{
    ts_inserta_referencia(ts, $1, region, pos[region], PC_REL_LO_12_SHIFT, tr_code);
    $$ = 0; // Referencia por resolver
}
;

r_posicion_larga:
T_INT_LIT
{
    $$ = $1;
}

| T_ALPHANUM
{
    ts_inserta_referencia(ts, $1, region, pos[region], PC_REL_LO_20_SHIFT, tr_code);
    $$ = 0; // Referencia por resolver
}
;

r_instruccion:
/*** Pseudo Instrucciones ***/
FORM_PSEUDO_NOP
{
    nueva_instruccion(RV32I_ADDI, FormatoI, 0, 0, 0, 0, 0, "", 0);
}

// xxx rd, rs
| FORM_PSEUDO_RR REG_INT ',' REG_INT
{
    switch ($1) {
    	case PSEUDO_NEG:
                nueva_instruccion(RV32I_SUB, FormatoR, $2, _zero, $4, 0, 0, "", 0);
        	break;
 	case PSEUDO_NEGW:
                nueva_instruccion(RV64I_SUBW, FormatoR, $2, _zero, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_SNEZ:
    		nueva_instruccion(RV32I_SLTU, FormatoR, $2, _zero, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_SLTZ:
                nueva_instruccion(RV32I_SLT, FormatoR, $2, $4, _zero, 0, 0, "", 0);
    		break;
    	case PSEUDO_SGTZ:
                nueva_instruccion(RV32I_SLT, FormatoR, $2, _zero, $4, 0, 0, "", 0);
    		break;
    }
}

// bxxx rs, offset
| FORM_PSEUDO_BRA REG_INT ','  r_posicion
{
     switch ($1) {
    	case PSEUDO_BEQZ:
                nueva_instruccion(RV32I_BEQ, FormatoB, 0, $2, 0, 0, ($4 - pos[region])/2, desplazamiento, 0);
    		break;
    	case PSEUDO_BNEZ:
                nueva_instruccion(RV32I_BNE, FormatoB, 0, $2, 0, 0, ($4 - pos[region])/2, desplazamiento, 0);
    		//instruccion.codop = RV32I_BNE;
    		break;
    	case PSEUDO_BLEZ:
                nueva_instruccion(RV32I_BGE, FormatoB, 0, 0, $2, 0, ($4 - pos[region])/2, desplazamiento, 0);
    		break;
    	case PSEUDO_BGEZ:
                nueva_instruccion(RV32I_BGE, FormatoB, 0, $2, 0, 0, ($4 - pos[region])/2, desplazamiento, 0);
    		break;
    	case PSEUDO_BLTZ:
                nueva_instruccion(RV32I_BLT, FormatoB, 0, $2, 0, 0, ($4 - pos[region])/2, desplazamiento, 0);
                break;
         case PSEUDO_BGTZ:
                nueva_instruccion(RV32I_BLT, FormatoB, 0, 0, $2, 0, ($4 - pos[region])/2, desplazamiento, 0);
    		break;
    }
}

// alu_fp f,f 
| FORM_PSEUDO_FF REG_FP ',' REG_FP  
{
    switch ($1) {
    	case PSEUDO_FMV_S:
                nueva_instruccion(RV32F_FSGNJ_S, FormatoR, $2, $4, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_FABS_S:
                nueva_instruccion(RV32F_FSGNJX_S, FormatoR, $2, $4, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_FNEG_S:
                nueva_instruccion(RV32F_FSGNJN_S, FormatoR, $2, $4, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_FMV_D:
                nueva_instruccion(RV32D_FSGNJ_D, FormatoR, $2, $4, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_FABS_D:
                nueva_instruccion(RV32D_FSGNJX_D, FormatoR, $2, $4, $4, 0, 0, "", 0);
    		break;
    	case PSEUDO_FNEG_D:
                nueva_instruccion(RV32D_FSGNJN_D, FormatoR, $2, $4, $4, 0, 0, "", 0);
 	}
}

// j offset
| FORM_PSEUDO_J r_posicion_larga
{
    nueva_instruccion(RV32I_JAL, FormatoJ, 0, 0, 0, 0, ($2 - pos[region])/2, desplazamiento, 0);
}


// jr rs
| FORM_PSEUDO_JR REG_INT
{
    nueva_instruccion(RV32I_JALR, FormatoI, $2, 0, 0, 0, 0, desplazamiento, 0);
}

// ret
| FORM_PSEUDO_RET
{
    nueva_instruccion(RV32I_JALR, FormatoI, 0, 1, 0, 0, 0, "", 0);
}


//rdinstret
//rdinsreth
//rdcycle
//rdcycleh
//rdtime
//rdtimeh

//csrr
//csrw
//csrs
//csrc
//csrwi 
//csrsi 
//csrci 
//frcsr 
//frrm 
//fsrm 
//frflags 
//fsflags 

// bxxx rs, rt, offset
| FORM_PSEUDO_BRRA REG_INT ','  REG_INT ',' r_posicion
{
    bzero(&instruccion, sizeof (instruccion_t));
    switch ($1) {
    	case PSEUDO_BGT:
            nueva_instruccion(RV32I_BLT, FormatoB, 0, $4, $2, 0, ($6 - pos[region])/2, desplazamiento, 0);
            break;
    	case PSEUDO_BLE:
            nueva_instruccion(RV32I_BGE, FormatoB, 0, $4, $2, 0, ($6 - pos[region])/2, desplazamiento, 0);
            break;
    	case PSEUDO_BGTU:
            break;
            nueva_instruccion(RV32I_BLTU, FormatoB, 0, $4, $2, 0, ($6 - pos[region])/2, desplazamiento, 0);
    	case PSEUDO_BLEU:
            nueva_instruccion(RV32I_BGEU, FormatoB, 0, $4, $2, 0, ($6 - pos[region])/2, desplazamiento, 0);
            break;
    }
}

// xxx rd, rs
| FORM_PSEUDO_RRi REG_INT ',' REG_INT
{
    bzero(&instruccion, sizeof (instruccion_t));
    switch ($1) {
    	case PSEUDO_MV:
            nueva_instruccion(RV32I_ADDI, FormatoI, $2, $4, 0, 0, 0, "", 0);
            break;
    	case PSEUDO_NOT:
            nueva_instruccion(RV32I_XORI, FormatoI, $2, $4, 0, 0, -1, "", 0);
            break;
	case PSEUDO_SEXT_W:
            nueva_instruccion(RV64I_ADDIW, FormatoI, $2, $4, 0, 0, 0, "", 0);
            break;
 	case PSEUDO_SEQZ:
            nueva_instruccion(RV32I_SLTIU, FormatoI, $2, $4, 0, 0, 1, "", 0);
            break;
    }
}

    
// li rd, imm
| FORM_PSEUDO_LI REG_INT ',' T_INT_LIT

{
    if (($4 >= MIN_IMM) && ($4 < MAX_IMM)) {
	sprintf(desplazamiento, F_DWORD, $4);
	nueva_instruccion(RV32I_ORI, FormatoI, $2, 0, 0, 0, $4, "", 0);
    } else {
       udword valor;

       sprintf(desplazamiento,"%%hi[" F_DWORD "]",valor);	
       valor = $4 >> 12;
       valor &= 0xFFFFF;
       nueva_instruccion(RV32I_LUI, FormatoU, $2, 0, 0, 0, valor, desplazamiento, 0);

       sprintf(desplazamiento,"%%lo[" F_DWORD "]",$4);
       valor = $4;
       valor &= 0xFFF;
       nueva_instruccion(RV32I_ORI, FormatoI, $2, $2, 0, 0, valor, desplazamiento, 0);
    }
}


// lla rd, symbol
| FORM_PSEUDO_LLA REG_INT ',' T_ALPHANUM
{
    sprintf(desplazamiento,"%%pcrel_hi(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_HI_20, tr_data);
    nueva_instruccion(RV32I_AUIPC, FormatoU, $2, 0, 0, 0, 0, desplazamiento, 0);

    sprintf(desplazamiento,"%%pcrel_lo(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_LO_12, tr_data);
    nueva_instruccion(RV32I_ADDI, FormatoI, $2, $2, 0, 0, 0, desplazamiento, 0);
}
               
       
// call offset
| FORM_PSEUDO_CALL T_ALPHANUM
{
    sprintf(desplazamiento,"%%pcrel_hi(%s)",$2);
    ts_inserta_referencia(ts, $2, region, pos[region], PC_REL_HI_20, tr_data);
    nueva_instruccion(RV32I_AUIPC, FormatoU, 1, 0, 0, 0, 0, desplazamiento, 0);

    sprintf(desplazamiento,"%%pcrel_lo(%s)",$2);
    ts_inserta_referencia(ts, $2, region, pos[region], PC_REL_LO_12, tr_data);
    nueva_instruccion(RV32I_JALR, FormatoI, 1, 1, 0, 0, 0, desplazamiento, 0);
}

// tail offset
| FORM_PSEUDO_TAIL T_ALPHANUM
{
    sprintf(desplazamiento,"%%pcrel_hi(%s)",$2);
    ts_inserta_referencia(ts, $2, region, pos[region], PC_REL_HI_20, tr_data);
    nueva_instruccion(RV32I_AUIPC, FormatoU, 6, 0, 0, 0, 0, desplazamiento, 0);

    sprintf(desplazamiento,"%%pcrel_lo(%s)",$2);
    ts_inserta_referencia(ts, $2, region, pos[region], PC_REL_LO_12, tr_data);
    nueva_instruccion(RV32I_JALR, FormatoI, 0, 6, 0, 0, 0, desplazamiento, 0);
}


| FORM_PSEUDO_TRAP T_INT_LIT
{
    nueva_instruccion(RV32I_ORI, FormatoI, _a7, 0, 0, 0, 10, "", 0);
    nueva_instruccion(RV32I_ECALL, FormatoI, 0, 0, 0, 0, 0, "", 0);
}


// ALU x,x,#imm
| FORM_INT_RRI REG_INT ',' REG_INT ',' r_inmediato_disp
{
    nueva_instruccion($1, FormatoI, $2, $4, 0, 0, $6, desplazamiento, 0);
}

// SHIFT x,x,#shift
| FORM_INT_RRSH REG_INT ',' REG_INT ',' T_INT_LIT
{
    nueva_instruccion($1, FormatoIshift, $2, $4, 0, 0, $6, "", 0);
}

// ALU x,x,x
| FORM_INT_RRR REG_INT ',' REG_INT ',' REG_INT
{
    nueva_instruccion($1, FormatoR, $2, $4, $6, 0, 0, "", 0);
}

// lui x, #imm
| FORM_INT_RI REG_INT ',' r_inmediato_hi
{
    nueva_instruccion($1, FormatoU, $2, 0, 0, 0, $4, desplazamiento, 0);
}

// auipc x, posicion
| FORM_INT_RA REG_INT ',' T_INT_LIT
{
    nueva_instruccion($1, FormatoU, $2, 0, 0, 0, $4, desplazamiento, 0);
}

// jal x, posicion
| FORM_J_RA REG_INT ',' r_posicion_larga
{
    nueva_instruccion($1, FormatoJ, $2, 0, 0, 0, ($4 - pos[region])/2, desplazamiento, 0);
}

// jalr x,x,posicion
| FORM_J_RRA REG_INT ',' REG_INT ',' r_posicion
{
    nueva_instruccion($1, FormatoI, $2, $4, 0, 0, $6, desplazamiento, 0);
}
    
// branch x,x,posicion
| FORM_INT_B_RRA REG_INT ',' REG_INT ',' r_posicion
{
    nueva_instruccion($1, FormatoB, 0, $2, $4, 0, $6 - pos[region], desplazamiento, 0);
}

// load x,imm(x)
| FORM_INT_L_RSB REG_INT ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, $2, $6, 0, 0, $4, desplazamiento, 0);
}
    
    
// l* x, symbol
// load pseudo instrucciones
| FORM_INT_L_RSB REG_INT ',' T_ALPHANUM
{
    sprintf(desplazamiento,"%%pcrel_hi(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_HI_20, tr_data);
    nueva_instruccion(RV32I_AUIPC, FormatoU, $2, 0, 0, 0, 0, desplazamiento, 0);

    sprintf(desplazamiento,"%%pcrel_lo(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_LO_12, tr_data);
    nueva_instruccion($1, FormatoI, $2, $2, 0, 0, 0, desplazamiento, 0);
}


// store x,imm(x)
| FORM_INT_S_RSB REG_INT ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoS, 0, $6, $2, 0, $4, desplazamiento, 0 );
}

// s* x, symbol, x
// store pseudo instrucciones
| FORM_INT_S_RSB REG_INT ',' T_ALPHANUM ',' REG_INT
{
    sprintf(desplazamiento,"%%pcrel_hi(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_HI_20, tr_data);
    nueva_instruccion(RV32I_AUIPC, FormatoU, $6, 0, 0, 0, 0, desplazamiento, 0);

    sprintf(desplazamiento,"%%pcrel_lo(%s)",$4);
    ts_inserta_referencia(ts, $4, region, pos[region], PC_REL_LO_12, tr_data);
    nueva_instruccion($1, FormatoS, 0, $6, $2, 0, 0, desplazamiento, 0);
}

// ecall
| FORM_INT_ECALL
{
    nueva_instruccion($1, FormatoI, 0, 0, 0, 0, FUNC7($1), "", 0);
}
    
// ebreak
| FORM_INT_EBREAK
{
   nueva_instruccion($1, FormatoI, 0, 0, 0, 0, FUNC7($1), "", 0);
}

// alu_fp f,f,f
| FORM_FP_FFF REG_FP ',' REG_FP ',' REG_FP 
{
    nueva_instruccion($1, FormatoR, $2, $4, $6, 0, 0, "", 0);
}

// alu_fp f,f,f,rm
| FORM_FP_FFFr REG_FP ',' REG_FP ',' REG_FP ',' FR_MODE
{
      nueva_instruccion($1, FormatoR3rm, $2, $4, $6, 0, 0, "", Redondeo_id($8));
}

// alu_fp f,f,f (rm=por defecto)
| FORM_FP_FFFr REG_FP ',' REG_FP ',' REG_FP 
{
    nueva_instruccion($1, FormatoR3rm, $2, $4, $6, 0, 0, "", DYNAMIC_RM);
}


// alu_fp f,f,f,f,rm
| FORM_FP_FFFFr REG_FP ',' REG_FP ',' REG_FP ',' REG_FP ',' FR_MODE
{
    nueva_instruccion($1, FormatoR4rm, $2, $4, $6, $8, 0, "", Redondeo_id($10));
}

// alu_fp f,f,f,f (rm=por defecto)
| FORM_FP_FFFFr REG_FP ',' REG_FP ',' REG_FP ',' REG_FP 
{    
    nueva_instruccion($1, FormatoR4rm, $2, $4, $6, $8, 0, "", DYNAMIC_RM);
}


// alu_fp f,f,rm
| FORM_FP_FFr REG_FP ',' REG_FP  ',' FR_MODE
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "", Redondeo_id($6));
}

// alu_fp f,f (rm=por defecto)
| FORM_FP_FFr REG_FP ',' REG_FP  
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "", DYNAMIC_RM);
}

// alu_fp f,f 
| FORM_FP_FF REG_FP ',' REG_FP  
{
    nueva_instruccion($1, FormatoR, $2, $4,  FUNC5($1), 0, 0, "", 0);
}

// alu_fp f,x,rm
| FORM_FP_FRr REG_FP ',' REG_INT  ',' FR_MODE 
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "", Redondeo_id($6));
}

//  alu_fp f,x (rm por defecto)
| FORM_FP_FRr REG_FP ',' REG_INT 
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "",DYNAMIC_RM);
}

// alu_fp f,x
| FORM_FP_FR REG_FP ',' REG_INT 
{
    nueva_instruccion($1, FormatoR, $2, $4,  FUNC5($1), 0, 0, "",0);
}
    
// alu_fp x,f,rm
| FORM_FP_RFr REG_INT ',' REG_FP  ',' FR_MODE 
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "",  Redondeo_id($6)); 
}

//  alu_fp x,f (rm por defecto)
| FORM_FP_RFr REG_INT ',' REG_FP 
{
    nueva_instruccion($1, FormatoR2rm, $2, $4,  FUNC5($1), 0, 0, "",  DYNAMIC_RM); 
}

// alu_fp x,f
| FORM_FP_RF REG_INT ',' REG_FP 
{
    nueva_instruccion($1, FormatoR, $2, $4,  FUNC5($1), 0, 0, "",  0); 
}

// comparacion flotante x,f,f 
| FORM_FP_RFF REG_INT ',' REG_FP ',' REG_FP 
{
    nueva_instruccion($1, FormatoR, $2, $4,  $6, 0, 0, "",  0); 
}

// fl* f, imm(x)
| FORM_FP_L_FSB REG_FP ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoI, $2, $6, 0, 0, $4, desplazamiento, 0);
}    
    
// fl* f, symbol
// Symbol es un valor entero de una dirección relativa dentro del segmento de código
// TODO: Si se quiere usar etiquetas hay que cambiar las instrucciones generadas
| FORM_FP_L_FSB REG_FP ',' T_INT_LIT
{
    symbol = $4 - pos[region];
    nueva_instruccion(RV32I_AUIPC, FormatoU, $2, 0, 0, 0, symbol >> 12, desplazamiento, 0);
    nueva_instruccion($1, FormatoI, $2, $2, 0, 0, symbol & 0b111111111111, desplazamiento, 0);
}

// fs* fp, imm(x)
| FORM_FP_S_FSB REG_FP ',' r_inmediato_disp '(' REG_INT ')'
{
    nueva_instruccion($1, FormatoS, 0, $6, $2, 0, $4, desplazamiento, 0 );
}

// fs* f, symbol, x
// Symbol es un valor entero de una dirección relativa dentro del segmento de código
// TODO: Si se quiere usar etiquetas hay que cambiar las instrucciones generadas
| FORM_FP_S_FSB REG_FP  ',' T_INT_LIT ',' REG_INT
{
    symbol = $4 - pos[region];
    nueva_instruccion(RV32I_AUIPC, FormatoU, $6, 0, 0, 0, symbol >> 12, desplazamiento, 0);  
    nueva_instruccion($1, FormatoS, 0, $6, $2, 0, symbol & 0b111111111111, desplazamiento, 0 );
}    
    
// Instr atomica load
| FORM_INT_RB REG_INT ',' '(' REG_INT ')' 
{
    nueva_instruccion($1, FormatoR, $2, $5, FUNC5($1), 0, 0, "", 0);
}


// Instr atomica store y am*
| FORM_INT_RRB REG_INT ',' REG_INT ',' '(' REG_INT ')' 
{
   nueva_instruccion($1, FormatoR, $2, $4, $7, 0, 0, "", 0);
}

// fence

// CRS

%%

/*** Definición de código ***********************************************/

void nueva_instruccion(
    codop_t codop,
    formato_t tipo,
    int Rdestino,
    int Rfuente1,
    int Rfuente2,
    int Rfuente3,
    dword inmediato,
    char * etiqueta,
    int Redondeo
) {
    if (!es_region_codigo(region)) {
        yyerror("Se debe abrir una sección de código (v.g. .text) antes de insertar instrucciones");
    }

    bzero(&instruccion, sizeof (instruccion_t));

    instruccion.codop = codop;
    instruccion.tipo = tipo;
    instruccion.rd = (Rdestino < 0) ? Rdestino + TAM_REGISTROS : Rdestino;
    instruccion.rd_usa_abi = (Rdestino < 0) ? NO : SI;
    instruccion.rs1 = (Rfuente1 < 0) ? Rfuente1 + TAM_REGISTROS : Rfuente1;
    instruccion.rs1_usa_abi = (Rfuente1 < 0) ? NO : SI;
    instruccion.rs2 = (Rfuente2 < 0) ? Rfuente2 + TAM_REGISTROS : Rfuente2;
    instruccion.rs2_usa_abi = (Rfuente2 < 0) ? NO : SI;
    instruccion.rs3 = (Rfuente3 < 0) ? Rfuente3 + TAM_REGISTROS : Rfuente3;
    instruccion.rs3_usa_abi = (Rfuente3 < 0) ? NO : SI;
    instruccion.imm = inmediato;
    //printf("inmediato: %d %u %x\n", inmediato, inmediato, inmediato);
    instruccion.rm = Redondeo;
    //strcpy(instruccion.etiqueta, etiqueta);
    strcpy(instruccion.etiqueta, etiqueta);
    mem_escribe_region_instruc(mm, region, pos[region], instruccion);
    pos[region] += 4;
}

int yyerror
(
    const char *s
        ) {
    fprintf(stderr, "%s:%ld. Error: %s\n", ts->fichero, linenumber, s);
    exit(1);
} /* end yyerror */


void ensamblador (
    fichero_objeto_t * obj
      ) {

    FILE * f_input;

    /* Inicializamos variables, por si hacemos dos pasadas de ensamblador */
    linenumber = 1;
    hay_valor=0;
    hay_etiq=0;

    /* Si están inicializados en otro módulo no se pueden volver a inicializar */
    no_ireg = init_ireg;
    no_fpreg = init_fpreg;
    no_btb = init_btb;

    idx_ireg = 1;
    idx_fpreg = 0;
    idx_btb = 0;
    i = 0;

    mm = &obj->memoria;
    ts = &obj->ts;

    for (i= 0; i< num_regiones; i++) {
       pos[i] = 0;
    }
    region = r_invalid;

    f_input = fopen(obj->ts.fichero, "r");
    if (f_input == NULL) {
       fprintf(stderr, "Error abriendo el fichero '%s'.\n", obj->ts.fichero);
       exit(1);
    } /* endif */

    yyin = f_input;
    if (yyin == NULL) return;

    /*** Realiza el 'parsing' del fichero de entrada ***/

    yyparse();

    // Ajustando los límites de las regiones utilizadas por el objeto
    for (i= 0; i< num_regiones; i++) {
    	  mm->regiones[i].final = pos[i];
    }

    fclose(f_input);

} /* end ensamblador */


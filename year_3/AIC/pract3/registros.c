/***************************************************************************
 *
 * Arquitectura de Computadores.
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politècnica de Valencia.
 *
 * Autor: Pedro López (plopez@disca.upv.es)
 *        Sergio Sáez (ssaez@disca.upv.es)
 *
 * Fichero: control.c
 *
 * Descripción:
 *   Contiene las funciones para leer y escribir el banco de registros
 *
 ****************************************************************************/

#define registros_C

#include "registros.h"
#include "tipos.h"
#include "main.h"

char *nombre_int_reg[] = {
    "zero", "ra",
    "sp", "gp", "tp",
    "t0", "t1", "t2",
    "s0", "s1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};

char *nombre_fp_reg[] = {
    "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",
    "fs0", "fs1",
    "fa0", "fa1",
    "fa2", "fa3", "fa4", "fa5", "fa6", "fa7",
    "fs2", "fs3", "fs4", "fs5", "fs6", "fs7", "fs8", "fs9", "fs10", "fs11",
    "ft8", "ft9", "ft10", "ft11"
};

valor_t lee_int_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rint[n].tipo;
    *dirty = Rint[n].dirty;
    return Rint[n].valor;

}

void escribe_int_reg(
        dword n,
        dword dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rint[n].valor.int_d = dato;
    Rint[n].tipo = tipo_dato;
    if (activar_dirty)
        Rint[n].dirty = SI;
}

valor_t lee_fp_reg(
        dword n,
        tipo_t *tipo,
        boolean *dirty
        ) {
    *tipo = Rfp[n].tipo;
    *dirty = Rfp[n].dirty;
    return Rfp[n].valor;
}

void escribe_fp_reg(
        dword n,
        valor_t dato,
        tipo_t tipo_dato,
        boolean activar_dirty
        ) {
    Rfp[n].valor = dato;
    Rfp[n].tipo = tipo_dato;
    if (activar_dirty)
        Rfp[n].dirty = SI;
}


/**
 * @file registros.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones para el acceso al banco de registros
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef REGISTROS_H
#define REGISTROS_H

/*** Includes ******************************************/

#include "tipos.h"


/*** Definiciones **************************************/

/*** ABI  */
#define _zero 0
#define _ra  1
#define _sp  2
#define _gp  3
#define _tp  4
#define _t0  5
#define _t1  6
#define _t2  7
#define _s0  8
#define _fp  8
#define _s1  9
#define _a0  10
#define _a1  11
#define _a2  12
#define _a3  13
#define _a4  14
#define _a5  15
#define _a6  16
#define _a7  17
#define _s2  18
#define _s3  19
#define _s4  20
#define _s5  21
#define _s6  22
#define _s7  23
#define _s8  24
#define _s9  25
#define _s10  26
#define _s11  27
#define _t3  28
#define _t4  29
#define _t5  30
#define _t6  31

#define _ft0    0
#define _ft1    1
#define _ft2    2
#define _ft3    3
#define _ft4    4
#define _ft5    5
#define _ft6    6
#define _ft7    7
#define _fs0    8
#define _fs1    9
#define _fa0    10
#define _fa1    11
#define _fa2    12
#define _fa3    13
#define _fa4    14
#define _fa5    15
#define _fa6    16
#define _fa7    17
#define _fs2    18
#define _fs3    19
#define _fs4    20
#define _fs5    21
#define _fs6    22
#define _fs7    23
#define _fs8    24
#define _fs9    25
#define _fs10   26
#define _fs11   27
#define _ft8    28
#define _ft9    29
#define _ft10   30
#define _ft11   31

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CPROTO
#include "registros_api.h"
#endif


#ifdef __cplusplus
}
#endif

extern char *nombre_int_reg[];
extern char *nombre_fp_reg[];

#endif /* REGISTROS_H */


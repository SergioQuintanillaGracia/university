/**
 * @file presentacion.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones para el módulo de presentación
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef presentacion_H
#define presentacion_H

/***** Includes ***********************************************/

#include "tipos.h"
#include "memoria.h"

/***** Constants **********************************************/

#define MAX_INSTRUC     64 /* Número máximo de instrucciones visibles */
#define MAX_CICLOS_INIC 45 /* Número máximo de ciclos visibles
                                   inicialmente */
#define MAX_CICLOS      50 /* Número máximo de ciclos visibles */

/***** Types **************************************************/

typedef enum {
    ningun_formato,
    formato_txt,
    formato_html,
    formato_json
} formato_presentacion_t;

typedef struct {
    // void imprime_inicio (void);
    void (*imprime_inicio_f)(void);

    // void imprime_final (void);
    void (*imprime_final_f)(void);

    // void inicio_ciclo (ciclo_t ciclo);
    void (*inicio_ciclo_f)(ciclo_t ciclo);

    // void fin_ciclo (ciclo_t ciclo);
    void (*fin_ciclo_f)(ciclo_t ciclo);

    // void imprime_estado (void);
    void (*imprime_estado_f)(void);

    // void imprime_crono (void);
    void (*imprime_crono_f)(void);

    // void imprime_predictor (void);
    void (*imprime_predictor_f)(void);

    // void imprime_predictor (void);
    void (*imprime_memoria_f)(void);

    // void init_instruc (dword PC, ciclo_t orden, const char * overwrite);
    void (*init_instruc_f)(dword PC, ciclo_t orden, const char * overwrite);

    // void muestra_fase (char *fase, ciclo_t orden, boolean exception);
    void (*muestra_fase_f)(char *fase, ciclo_t orden, boolean exception);

    // void muestra_datos(dword from, dword to, mem_status_t status)
    void (*muestra_datos_f)(region_t r, dword from, dword to, mem_status_t status);
} presentacion_interface_t;

/***** Macros *************************************************/

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y) (((x)<(y)) ? (x) : (y))

#ifdef MAX
#undef MAX
#endif
#define MAX(x, y) (((x)>(y)) ? (x) : (y))

#define F_ESTADO F_CICLO "/" F_CICLO

#define FIRMA_1_1(s1, s2)                       \
    do {                                        \
        strcpy(f_campo, (s1));                  \
        strcpy(f_valor, (s2));                  \
        vstr= procesa_firma(f_campo, f_valor);  \
    } while (0)

#define FIRMA_1_2(s1, f2, p2)                   \
    do {                                        \
        strcpy(f_campo, (s1));                  \
        sprintf(f_valor, (f2), (p2));           \
        vstr= procesa_firma(f_campo, f_valor);  \
    } while (0)

#define FIRMA_2_1(f1, p1, s2)                   \
    do {                                        \
        sprintf(f_campo, (f1),(p1));            \
        strcpy(f_valor, (s2));                  \
        vstr= procesa_firma(f_campo, f_valor);  \
    } while (0)

#define FIRMA_2_2(f1, p1, f2, p2)               \
    do {                                        \
        sprintf(f_campo, (f1), (p1));           \
        sprintf(f_valor, (f2), (p2));           \
        vstr= procesa_firma(f_campo, f_valor);  \
    } while (0)

#define FIRMA_2_3(f1, p1, f2, p2a, p2b)         \
    do {                                        \
        sprintf(f_campo, (f1), (p1));           \
        sprintf(f_valor, (f2), (p2a), (p2b));   \
        vstr= procesa_firma(f_campo, f_valor);  \
    } while (0)


#define imprime_instruccion(str, dir) __imprime_instruccion(str, dir, __FILE__, __LINE__)

/***** Aliases ************************************************/

// void imprime_inicio (void);
#define imprime_inicio presentacion.imprime_inicio_f

// void imprime_final (void);
#define imprime_final presentacion.imprime_final_f

// void inicio_ciclo (ciclo_t ciclo);
#define inicio_ciclo(ciclo) presentacion.inicio_ciclo_f(ciclo)

// void fin_ciclo (ciclo_t ciclo);
#define fin_ciclo(ciclo) presentacion.fin_ciclo_f(ciclo)

// void imprime_estado (void);
#define imprime_estado presentacion.imprime_estado_f

// void imprime_crono (void);
#define imprime_crono presentacion.imprime_crono_f

// void imprime_predictor (void);
#define imprime_predictor presentacion.imprime_predictor_f

// void imprime_memoria (void);
#define imprime_memoria presentacion.imprime_memoria_f

// void init_instruc (dword PC, ciclo_t orden);
#define init_instruc presentacion.init_instruc_f

// void muestra_fase (char *fase, ciclo_t orden);
#define muestra_fase(fase, orden) presentacion.muestra_fase_f(fase, orden, NO)
#define muestra_fase_excepcion(fase, orden) presentacion.muestra_fase_f(fase, orden, SI)

// void muestra_datos (region_t r, dword from, dword to, mem_status_t status);
#define muestra_datos(r, from, to, status) presentacion.muestra_datos_f(r, from, to, status)

/***** Prototypes *********************************************/

#ifndef CPROTO

#include "presentacion_api.h"

#endif

#ifdef presentacion_C  /*** Privado ***/

char p_str[LONG_STR]; /* Cadena auxiliar */

/** Variables para las macros de firmas */
char f_campo[LONG_STR];
char f_valor[LONG_STR];
char *vstr;

boolean error_firma = NO;

#else    /*** Público ***/

extern char p_str[LONG_STR]; /* Cadena auxiliar */

extern char f_campo[LONG_STR];
extern char f_valor[LONG_STR];
extern char *vstr;

extern boolean error_firma;

extern presentacion_interface_t presentacion;

#endif

#endif /* end presentacion.h */


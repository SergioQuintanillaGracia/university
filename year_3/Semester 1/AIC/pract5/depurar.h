/**
 * @file depurar.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Funciones de depuración
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

// Cambiado para eliminar warnings
//  fprintf(stderr,"Error: %s; función:%s; línea:%d\n",(__FILE__?__FILE__:""), (__func__?__func__:""),(__LINE__?__LINE__:0));

#define fnError(formato, args...)  { \
    fprintf(stderr,"Error: %s; función:%s; línea:%d\n",(__FILE__?__FILE__:""), __func__,(__LINE__?__LINE__:0));    \
    fprintf(stderr, formato, ##args);\
    abort();                         \
    exit(1);}

#define fnWarning(formato, args...)  { \
    printf("Atención: %s; función:%s; línea:%d\n",(__FILE__?__FILE__:""), (__func__?__func__:""),(__LINE__?__LINE__:0));  \
    printf(formato, ##args);}

#ifdef DEBUG
#define fnDebug(formato, args...) {\
    printf("Debug: %s; función:%s; línea:%d\n",(__FILE__?__FILE__:""), __func__,(__LINE__?__LINE__:0)); \
    printf(formato, ##args);}
#else
#define fnDebug(formato, args...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */


/**
 * @file memoria-principal.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Definiciones y tipos para el nivel de memoria principal
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef memoria_principal_H
#define memoria_principal_H

/* -- Includes -- */

#include <stdio.h>

#include "memoria.h"
#include "tipos.h"

#include "jerarquia.h"

/* -- Constants -- */

// Tamaño del BUS de memoria en bits
#define BUS_BITS 64
// Tamaño del BUS de memoria en bytes
#define BUS_SIZE ((BUS_BITS)>>3)

#define CLOSED_ROW (-1)
#define UNKNOWN_ROW (-2)

#define ROW_SIZE    32

/* -- Types -- */

// Declaración anticipada de un banco de memoria
struct banco_memoria_struct;

typedef struct operacion_mem_principal_struct {
    operacion_nivel_t info;         /**< Información común sobre una operación */

    uint8_t CL;                     /**< CAS Latency - para esta operación */
    uint8_t tRCD;                   /**< RAS to CAS delay - para esta operación  */
    uint8_t tRP;                    /**< RAS Precharge - para esta operación  */

    uint8_t t_acceso;               /**< Tiempo de acceso total, contando la transferencia */

    uint16_t idx_banco;             /**< Banco de memoria sobre el que se va a realizar la operación */
    int32_t idx_fila;               /**< Fila que se necesita en esta operación */
} operacion_mem_principal_t;

// Operación candidata sobre un banco
typedef struct {
    operacion_mem_principal_t *oper;    /**< Instrucción candidata */
    ciclo_t inicio;                     /**< Antigüedad de la operación */
} operacion_banco_t;

typedef struct {
    uint32_t tam_memoria;   /**< Tamaño de la memoria principal */

    // Buffers de fila. Supone un canal por banco, dado que no comprueba
    //   la ocupación del bus de la MP para enviar las órdenes o realizar las transferencias
    uint16_t num_bancos;    /**< Número de bancos existentes en la MP */
    uint16_t tam_banco;     /**< Tamaño de un buffer de fila en bytes */

    uint8_t CL;             /**< CAS Latency */
    uint8_t tRCD;           /**< RAS to CAS delay */
    uint8_t tRP;            /**< RAS Precharge */

    uint8_t BW;             /**< Ancho de banda en bytes/ciclo */
} config_memoria_principal_t;

typedef enum {
    EST_BANCO_LIBRE,
    EST_BANCO_CERRANDO,
    EST_BANCO_LEYENDO,
    EST_BANCO_ACCEDIENDO,
    EST_BANCO_TRANSFIRIENDO
} estado_banco_memoria_t;

typedef struct banco_memoria_struct {
    dword direccion;                 /**< Dirección base del banco por sencillez */
    int32_t fila_abierta;            /**< Indice de la fila de memoria abierta. -1 para ninguna */
    estado_banco_memoria_t estado;   /**< Indica si hay una operación accediendo */

    char * signal;                   /**< Señal en curso, para su visualización */
} banco_memoria_t;

typedef struct memoria_principal_struct {
    nivel_jerarquia_memoria_t mem;         /**< Datos y operaciones de este nivel de memoria */

    config_memoria_principal_t config;     /**< Geometría de la memoria principal */

    banco_memoria_t * bancos;              /**< Bancos de memoria */
} memoria_principal_t;

/* -- Type related macros -- */

// DIR: <indice-fila> <indice-banco> <offset-banco>
// Dirección de palabra dentro de la memoria
#define DIREC_PALABRA_BUS(dir) ((dir) / BUS_SIZE)
// Offset en bytes dentro de la palabra de bus
#define DESPLAZ_PALABRA_BUS(dir) ((dir) % BUS_SIZE)
// Índice de palabra dentro del buffer de bus
#define INDICE_PALABRA_BUS(dir) (((dir) % (mp)->config.tam_banco) / BUS_SIZE)

// Dirección del banco de memoria dentro de la memoria
#define DIREC_BANCO_MEMORIA(mp, dir) ((dir) / (mp)->config.tam_banco)
// Offset en bytes dentro del buffer de bus
#define DESPLAZ_BANCO_MEMORIA(mp, dir) ((dir) % (mp)->config.tam_banco)
// Dirección base del banco de memoria dentro de la memoria
#define DIREC_BASE_BANCO_MEMORIA(mp, dir) ((dir) - DESPLAZ_BANCO_MEMORIA(mp, dir))
// Índice del banco de memoria dentro de la fila
#define INDICE_BANCO_MEMORIA(mp, dir) (DIREC_BANCO_MEMORIA(mp, dir) % (mp)->config.num_bancos)

// Dirección de fila dentro de la memoria
#define DIREC_FILA_MEMORIA(mp, dir) ((dir) / ((mp)->config.num_bancos * (mp)->config.tam_banco))
// Offset en bytes dentro de la fila
#define DESPLAZ_FILA_MEMORIA(mp, dir) ((dir) % ((mp)->config.num_bancos * (mp)->config.tam_banco))
// Índice de fila dentro de la memoria principal. Igual que la dirección de fila
#define INDICE_FILA_MEMORIA(mp, dir) DIREC_FILA_MEMORIA(mp, dir)

#define DIREC_FILA_ABIERTA(mp, fila, banco) ((((fila) * (mp)->config.num_bancos) + (banco)) * (mp)->config.tam_banco)

/* -- Prototypes -- */

#ifndef CPROTO

#include "memoria_principal_api.h"

#endif

#endif /* end memoria-principal.h */

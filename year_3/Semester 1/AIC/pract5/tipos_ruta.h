/**
 * @file tipos_ruta.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Contiene las definiciones de tipos que dependen de la ruta de datos
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef tipos_ruta_H
#define tipos_ruta_H

/***** Includes ***********************************************/

#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

/***** Constants **********************************************/

#define INST_UNKNOWN (-1)

/***** Types **************************************************/

typedef enum {
    l_bypassing,
    l_forwarding
} dep_mem_t;

typedef enum {
    NONE,
    EX,
    WB
} estado_t; /* Estado de una operación en el ROB */

typedef enum {
    PENDIENTE,
    PREPARADA, /* Este estado sólo lo utiliza la memoria de datos */
    EN_CURSO,
    FINALIZADA
} estado_oper_t; /* Estado de una operación en una ER */

typedef int marca_t; /* Tipo marca/código */

typedef enum {
    OPER_ENTEROS,
    OPER_SUMREST,
    OPER_MULTDIV,
    OPER_MEMDATOS,
    OPER_DIRECCIONES,
    NUM_TIPOS_OPERADORES
} tipo_operador_t;

typedef enum {
    EXCEPCION,
    RESULT,
    CONDICION,
    DIRECCION
} codigo_bus_t;

/*** Banco de registros ********/

typedef struct {
    valor_t valor; /* Valor del registro */
    marca_t rob; /* Marca del registro */
} reg_t;

/*** Registros inter-etapas ************************************************/

typedef struct { /* IF/ID */
    uword instruccion;
    char etiqueta[L_ETIQ];
    codop_t codop; /* TODO Parche para los predictores */

    dword PC; /* Posición de memoria de la instrucción */
    ciclo_t orden; /* Orden de la instrucción */

    boolean prediccion; /* Indica se se ha predicho salto o no */
    prediccion_t pred_data; /* Datos auxiliares  */

    int excepcion; /* Indica si se ha producido alguna excepción y cual */
    boolean cancelar; /* Índica que esta instrucción se debe descartar en ISSUE */
    boolean ignorar; /* Índica que esta instrucción ya se ha lanzado anteriormente */
} reg_if_iss_t;

typedef struct {
    dword NPC;          /* Siguiente PC */
    ciclo_t orden;      /* Mantiene el orden de las instrucciones */
    int indice;         /* Índice de la siguiente instrucción */
} reg_control_t;

/*** Estructuras internas de la ruta de datos de coma flotante *************/

/*** Estaciones de reserva ******/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    codop_t OP; /* Código de operación a realizar */

    marca_t Q1; /* Marca del primer operando. ALU */
    valor_t V1; /* Valor del primer operando. ALU */

    marca_t Q2; /* Marca del segundo operando. ALU y TE */
    valor_t V2; /* Valor del segundo operando. ALU y TE */

    marca_t Q3; /* Marca del tercer operando. ALU (MADD) */
    valor_t V3; /* Valor del tercer operando. ALU (MADD) */

    dword direccion; /* Dirección de acceso a memoria. TL y TE. Enteras */
    dword desplazamiento; /* Desplazamiento en el acceso a memoria. TL y TE */
    char etiqueta[L_ETIQ]; /* Idem, pero etiqueta, TL y TE */

    boolean confirm; /* Indica si la operación de memoria
                      *  ha sido confirmada. TE */

    estado_oper_t estado; /* Indica si la operación está pendiente, preparada, en curso o si
                           *  ya ha terminado y el resultado está en la RS */
    unsigned int operador; /* Indica qué operador la está atendiendo mientras está en el estado EN_CURSO */
    valor_t resultado; /* Resultado de la operación */
    boolean condicion; /* En las RS enteras, para la condición de salto */

    marca_t rob; /* Indica para quien es la operación.  */

    int excepcion; /* Indica si se ha producido alguna excepción
                    * al ejecutar está operación */

    int redondeo; /* Rounding mode en FP */
    codigo_bus_t control;

    ciclo_t orden; /* Orden de la instrucción */
} estacion_t;

/*** Reorder buffer *************/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    codop_t OP; /* Código de operación a realizar */

    boolean completado; /* Estado de la operación */

    dword dest; /* Registro destino, SB */
    // ^ podria ser un byte!!!
    valor_t valor; /* Resultado de la operación */

    dword direccion; /* Dirección de salto */
    boolean condicion; /* Resultado de la condición */
    boolean prediccion; /* Indica se se ha predicho salto o no */

    prediccion_t pred_data; /* Datos auxiliares  */

    int excepcion; /* Indica si se ha producido alguna excepción
                                   al ejecutar está instrucción */
    dword PC; /* Posición de memoria de la instrucción */
    codigo_bus_t control;
    ciclo_t orden; /* Orden de la instrucción */
} reorder_t;

/*** Operador *******************/

typedef struct {
    boolean ocupado; /* Bit de ocupado */
    tipo_operador_t tipo; /* Tipo de operador */
    int estacion; /* Estación de reserva en uso */
    marca_t codigo; /* Código del reorder buffer */
    ciclo_t ciclo; /* Ciclo actual de la operación */
    ciclo_t Teval; /* Tiempo de evaluación */

    ciclo_t orden; /* Orden de la instrucción */
} operador_t;

/*** Bus común ******************/

typedef struct {
    boolean ocupado;        /* Linea de ocupado */
    marca_t codigo;         /* Lineas para los códigos */
    valor_t valor;          /* Lineas de datos: resultado, dirección o excepción */
    boolean condicion;
    codigo_bus_t control;   /* Indica qué se está transfiriendo */
} bus_comun_t;


/*** Estadisticas ***/

typedef struct {
    int RS_ENTEROS;
    int RS_SUMREST;
    int RS_MULTDIV;
    int BUFFER_CARGA;
    int BUFFER_ALMACEN;
    int OP_ENTEROS;
    int OP_SUMREST;
    int OP_MULTDIV;
    int OP_MEMDATOS;
    int OP_DIRECCIONES;
    int REORDER;
    int BTB;
} utilizacion_t;

#endif /* end tipos_ruta.h */

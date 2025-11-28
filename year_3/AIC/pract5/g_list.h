/**
 * @file g_list.h
 *
 * Departamento de Informática de Sistemas y Computadores (DISCA)
 * Universidad Politécnica de Valencia.
 *
 * @author Sergio Sáez <ssaez@disca.upv.es>
 * @author Pedro López <plopez@disca.upv.es>
 *
 * @brief
 *    Soporte para el manejo de listas dinámicas
 *
 * @copyright [CC BY-NC-ND 4.0]
 *   Esta obra está bajo una licencia de Creative Commons Atribución-NoComercial-SinDerivadas
 *   Para evitar dudas, no se tiene permiso bajo esta Licencia para compartir Material Adaptado.
 *   Más detalles en https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode.es
 */

#ifndef g_list_H
#define g_list_H

/*---- Macros ----*/

/** Single linked list. Only one head **/

#define LIST_INIT(HEAD) \
do {                    \
    HEAD = NULL;        \
} while (0)

#define LIST_INSERT_ITEM(TYPE, HEAD, ITEM) \
do {                                       \
    TYPE * l_item = (TYPE *)(ITEM);        \
    l_item->next = (HEAD);                 \
    HEAD = l_item;                         \
} while (0)

#define LIST_APPEND_ITEM(TYPE, HEAD, ITEM) \
do {                                       \
    TYPE ** p_item = &(HEAD);              \
    while (*p_item != NULL) {              \
        p_item = &((*p_item)->next);       \
    }                                      \
    (ITEM)->next = NULL;                   \
    *p_item = (TYPE *)(ITEM);              \
} while (0)

#define LIST_EXTRACT_ITEM(TYPE, HEAD) \
({                                    \
    TYPE * l_item = HEAD;             \
    HEAD = l_item->next;              \
    l_item;                           \
})

#define LIST_DELETE_ITEM(TYPE, HEAD, ITEM) \
do {                                       \
    TYPE ** p_item = &(HEAD);              \
    while (*p_item != NULL) {              \
        if (*p_item == (ITEM)) {           \
            *p_item = (*p_item)->next;     \
            break;                         \
        }                                  \
        p_item = &((*p_item)->next);       \
    }                                      \
} while(0)

/** Single linked queue. One head and one tail **/

#define QUEUE_INIT(HEAD, TAIL) \
do {                           \
    HEAD = NULL;               \
    TAIL = NULL;               \
} while (0)

#define QUEUE_INSERT_ITEM(TYPE, HEAD, TAIl, ITEM) \
do {                                              \
    TYPE * q_item = (TYPE *)(ITEM);               \
    q_item->next = (HEAD);                        \
    HEAD = q_item;                                \
    if (q_item->next == NULL) {                   \
        TAIL = q_item;                            \
    }                                             \
} while (0)

#define QUEUE_APPEND_ITEM(TYPE, HEAD, TAIL, ITEM) \
do {                                              \
    TYPE * q_item = (TYPE *)(ITEM);               \
    if ((TAIL) == NULL) {                         \
        HEAD = q_item;                            \
    } else {                                      \
        (TAIL)->next = q_item;                    \
    }                                             \
    q_item->next = NULL;                          \
    TAIL = q_item;                                \
} while (0)

#define QUEUE_EXTRACT_ITEM(TYPE, HEAD, TAIL) \
({                                           \
    TYPE * q_item = (HEAD);                  \
    HEAD = q_item->next;                     \
    if ((HEAD) == NULL) {                    \
        TAIL = NULL;                         \
    }                                        \
    q_item;                                  \
})

#define QUEUE_DELETE_ITEM(TYPE, HEAD, TAIL, ITEM) \
do {                                              \
    TYPE * last = NULL;                           \
    TYPE ** p_item = &(HEAD);                     \
    while (*p_item != NULL) {                     \
        if (*p_item == (ITEM)) {                  \
            *p_item = (*p_item)->next;            \
            if (*p_item == NULL)                  \
                TAIL = last;                      \
            break;                                \
        }                                         \
        last = *p_item;                           \
        p_item = &((*p_item)->next);              \
    }                                             \
} while(0)

#endif

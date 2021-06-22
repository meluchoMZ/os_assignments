/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#ifndef __LIST_H
#define __LIST_H

#include <stdlib.h> 
#include <time.h>
#define TAM 256
#define ELEM_TAM 4096
/* Constantes para os tipos de reserva de memoria. */
#define MALLOC 0
#define MMAP 1
#define SHARED 2

typedef char string[TAM];
typedef struct Elem {
    void* dir_malloc;
    int key_fd;
    char fich_nome[256];
    unsigned long int tamano;
    char ult_acc[256];
    int tipo;
}elem;
typedef struct stList {
    elem* datos[ELEM_TAM];
    int final;
} list;

void init_mlist(list* mlist);
void inserir_mlist (elem* item, list* mlist);
int imprimir_malloc(list mlist);
int imprimir_mmap(list mlist);
int imprimir_shared(list mlist);
void imprimir_mlist (list mlist);
int buscar_shared (list mlist, int key);
void borrar_mlist (list* mlist);
void borrar_nodo_addr(list* mlist, void* addr);
void borrar_nodo_tam (list* mlist, unsigned long int tam);
void borrar_nodo_mmap (list* mlist, char nome[]);
void borrar_nodo_shared (list* mlist, void * addr);
int buscar_addr (list mlist, void *addr);

#endif
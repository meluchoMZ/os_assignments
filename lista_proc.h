/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#ifndef __LIST_PROC_H
#define __LIST_PROC_H

#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "sinais.h"
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#define TAM_P 256
#define VACIA 0
#define ERRVACIA -1
#define TERM 0
#define SIG 1
#define ACTIVO 0
#define PARADO 1 
#define TERMINADO 2


typedef struct pdata {
    pid_t pid_proc;
    int prioridade;
    char lin_comm[TAM_P];
    char tempo_ini[TAM_P];
    int estado;
    int final;
} pdatos;
typedef struct pnodo {
    pdatos* datos;
    struct pnodo* seg_pelem;
} pelem;
typedef struct Lista_proc {
    pelem* p_inicio;
    pelem* p_final;
    int p_numnodos;
} plist;

int insertar_plist (plist* pl, pid_t pid, int prior, char lcom[], char tmp[], int est, int fin);
int borrar_pnodo (plist* pl, pid_t pid);
pelem* buscar_pnodo (plist pl, pid_t pid);
void borrar_plist (plist* pl);
void imprimir_plist(plist pl);
void actualizar_plist(plist *pl); 
void info_pid(plist pl, int pid);
int existe_plist(plist pl, int pid);
void borrarprocs (plist* pl, int que);
char* convertir_estado(int estado);
#endif

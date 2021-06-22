/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#ifndef __MEMORIA_H
#define __MEMORIA_H
/* Estas son necesarias para a parte de memoria (P2). */
#include "fendido.h"
#include "list.h"
#include "utilidades.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#define LEERCOMPLETO ((ssize_t) -1)	


/* Variabes globais para o primeiro caso de Cmd_mem */
int global1, global2, global3;
/* Funcions parte de memoria. */

void* asignar_malloc(unsigned long int tam, list* mlist);
void * ObtenerMemoriaShmget (key_t clave, size_t tam, list* mlist);
void Cmd_AsignarCreateShared (char *arg[], list* mlist);
void * MmapFichero (char * fichero, int protection, list* mlist);
void Cmd_AsignarMmap (char *arg[], list* mlist);
ssize_t LeerFichero (char *fich, void *p, ssize_t n);
void Cmd_borrakey (char *args[]);
void doRecursiva (int n);
void volcar (void* addr, int bytes);
void llenar (void* addr, int bytes, int byte);
ssize_t Escribir_fichero (char *fich, void *p, ssize_t n, int flag);
void Cmd_mem (elemento* it, list mlist, char cadea[]);
void Cmd_asignar (elemento* it, list *mlist, char cadea[]);
void Cmd_desasignar (elemento* aux, list *mlist, char cadea[]);
void Cmd_recursiva_n (elemento* aux, char cadea[]) ;
void Cmd_volcar (elemento*aux, char cadea[]);
void Cmd_llenar (elemento*aux, char cadea[]);
void borrar_key (elemento* aux, char cadea []);
void Cmd_rfich (elemento* aux, char cadea[]);
void Cmd_wfich (elemento* aux, char cadea[]);
#endif
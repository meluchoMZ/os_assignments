/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#include "list.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/shm.h>
/*****************************************************************************/
void init_mlist(list* mlist) {
    mlist->final = -1;
}
/*****************************************************************************/
void inserir_mlist (elem* item, list* mlist) {
    if (mlist->final == ELEM_TAM) {
        printf("Imposible reservar memoria: Array completo.\n");
    } else {
        mlist->final++;
        mlist->datos[mlist->final] = malloc(sizeof(elem));
        if (mlist->datos[mlist->final] == NULL) {
            mlist->final--;
            perror("Imposible reservar memoria: ");
        } else {
            mlist->datos[mlist->final]->dir_malloc = item->dir_malloc;
            mlist->datos[mlist->final]->key_fd = item->key_fd;
            strcpy(mlist->datos[mlist->final]->ult_acc, item->ult_acc);
            strcpy(mlist->datos[mlist->final]->fich_nome, item->fich_nome);
            mlist->datos[mlist->final]->tamano = item->tamano;
            mlist->datos[mlist->final]->tipo = item->tipo;
        }
    }
}
/*****************************************************************************/
int imprimir_malloc (list mlist) {
    int contador = 0;
    if (mlist.final != -1) {
        for (int i = 0; i <= mlist.final; i++) {
            if (mlist.datos[i]->tipo == MALLOC) {
                printf("%p: size:%ld.", mlist.datos[i]->dir_malloc, mlist.datos[i]->tamano);
                printf(" malloc  "); printf("%s\n", mlist.datos[i]->ult_acc);
                contador++;
            }
        }
    }
    return contador;
}
/*****************************************************************************/
int imprimir_mmap (list mlist) {
    int contador = 0;
    if (mlist.final != -1) {
        for (int i = 0; i <= mlist.final; i++) {
            if (mlist.datos[i]->tipo == MMAP) {
                printf("%p: size:%ld.", mlist.datos[i]->dir_malloc, mlist.datos[i]->tamano);
                printf(" mmap  %s (fd:%d) ", mlist.datos[i]->fich_nome, mlist.datos[i]->key_fd);
                printf("%s\n", mlist.datos[i]->ult_acc);
                contador++;
            }
        }
    }
    return contador;
}
/*****************************************************************************/
int imprimir_shared (list mlist) {
    int contador = 0;
    if (mlist.final != -1) {
        for (int i = 0; i <= mlist.final; i++) {
            if (mlist.datos[i]->tipo == SHARED) {
                printf("%p: size:%ld.", mlist.datos[i]->dir_malloc, mlist.datos[i]->tamano);
                printf(" shared memory (key %d) ", mlist.datos[i]->key_fd);
                printf("%s\n", mlist.datos[i]->ult_acc);
                contador++;
            }
        }
    }
    return contador;
}
/*****************************************************************************/
void imprimir_mlist (list mlist) {
    if (mlist.final != -1) {
        imprimir_malloc(mlist);
        imprimir_mmap(mlist);
        imprimir_shared(mlist);
    } else {
    	printf("Error: Lista vacia.\n");
    }
}
/*****************************************************************************/
void borrar_mlist (list* mlist) {
    if (mlist->final != -1){
        for (int i = mlist->final; i >= 0; i--){
        	switch (mlist->datos[i]->tipo) {
                case MALLOC : free(mlist->datos[i]->dir_malloc); break;
                case SHARED : shmdt(mlist->datos[i]->dir_malloc); 
                              shmctl(shmget(mlist->datos[i]->key_fd,0,0666),IPC_RMID,NULL);  
                              break;
                case MMAP   : munmap(mlist->datos[i]->dir_malloc, mlist->datos[i]->tamano); break;
            }
            free(mlist->datos[i]);
            mlist->final--;
        }
        mlist->final = -1;
    }
}
/*****************************************************************************/
void borrar_nodo(list* mlist, int pos) {
    free(mlist->datos[pos]);
    for (int i = pos; i < mlist->final; i++) {
        mlist->datos[i] = mlist->datos[i + 1];
    }
    mlist->final--;
}
/*****************************************************************************/
int buscar_addr (list mlist, void *addr) {
    int out = -1;
    for (int i = 0; i <= mlist.final; i++) {
        if ((mlist.datos[i]->dir_malloc == addr)) {
            return i;
        }
    }
    return out;
}
/*****************************************************************************/
int buscar_shared (list mlist, int key) {
    int out = -1;
    for (int i = 0; i <= mlist.final; i++) {
        if ((key == mlist.datos[i]->key_fd) && (mlist.datos[i]->tipo == SHARED)) 
            return i;
    }
    return out;
}
/*****************************************************************************/
int buscar_nome (list mlist, char nome[]) {
    int out = -1;
    for (int i = 0; i <= mlist.final; i++) {
        if ((strcmp(nome, mlist.datos[i]->fich_nome) == 0) && (mlist.datos[i]->tipo == MMAP)) 
            return i;
    }
    return out;
}
/*****************************************************************************/
int buscar_tam (list mlist, unsigned long int tam) {
    int out = -1;
    for (int i = 0; i <= mlist.final; i++) {
        if ((mlist.datos[i]->tamano == tam) && (mlist.datos[i]->tipo == MALLOC)) 
            return i;
    }
    return out;
}
/*****************************************************************************/
void borrar_nodo_addr (list* mlist, void * addr) {
    int index = buscar_addr(*mlist, addr);
    if (index == -1) {
        printf("Non se pode desasignar memoria. Non existe ou non hai permisos suficientes.\n");
    } else {
        printf("Desasignados %ld bytes de %p.\n", mlist->datos[index]->tamano, mlist->datos[index]->dir_malloc);
        free(mlist->datos[index]->dir_malloc);
        borrar_nodo(mlist, index);
    }
}
/*****************************************************************************/
void borrar_nodo_tam (list* mlist, unsigned long int tam){
    int index = buscar_tam(*mlist, tam);
    if (index == -1) {
        printf("Non se pode desasignar memoria: Non existe ou non hai permisos suficientes.\n");
    } else {
        printf("Desasignados %ld bytes de %p.\n", mlist->datos[index]->tamano, mlist->datos[index]->dir_malloc);
        free(mlist->datos[index]->dir_malloc);
        borrar_nodo(mlist,index);
    }
}
/*****************************************************************************/
void borrar_nodo_mmap (list* mlist, char nome[]) {
    int index = buscar_nome(*mlist, nome);
    if (index == -1) {
        printf("Non se pode desasignar memoria: Non existe ou non hai permisos suficientes.\n");
    } else {
        if (munmap(mlist->datos[index]->dir_malloc, mlist->datos[index]->tamano) == 0) {
            printf("Desasignado fichero mapeado '%s' de %p.\n", mlist->datos[index]->fich_nome, mlist->datos[index]->dir_malloc);
            borrar_nodo(mlist,index);
        } else {perror("Imposible desasignar memoria mmap");}
    }
}
/*****************************************************************************/
void borrar_nodo_shared (list* mlist, void * addr) {
    int index = buscar_addr(*mlist, addr);
    if (index == -1) {
        printf("Non se pode desasignar memoria. Non existe ou non hai permisos suficientes.\n");
    } else {
        printf("Desasignada shared memory de clave %d de %p.\n", mlist->datos[index]->key_fd, mlist->datos[index]->dir_malloc);
        borrar_nodo(mlist, index);
    }
}
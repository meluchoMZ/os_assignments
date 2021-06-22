/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "lista_proc.h"

void init_plist (plist* pl) {
    pl->p_inicio = NULL;
    pl->p_final = NULL;
    pl->p_numnodos = VACIA;
}
/*****************************************************************************/
int insertar_plist (plist* pl, pid_t pid, int prior, char lcom[], char tmp[], int est, int fin) {
    pelem* aux;
    if (pl->p_numnodos == VACIA) {
        if ((pl->p_inicio = malloc(sizeof(pelem))) == NULL) {return errno;}
        if ((pl->p_inicio->datos = malloc(sizeof(pdatos))) == NULL) {return errno;}
        pl->p_inicio->datos->pid_proc = pid;
        pl->p_inicio->datos->prioridade = prior;
        if (strcpy(pl->p_inicio->datos->lin_comm,lcom) == NULL) {return errno;}
        if (strcpy(pl->p_inicio->datos->tempo_ini,tmp) == NULL) {return errno;}
        pl->p_inicio->datos->estado = est;
        pl->p_inicio->datos->final = fin;
        pl->p_inicio->seg_pelem = NULL;
        pl->p_final = pl->p_inicio;
        pl->p_numnodos++;
    } else {
        aux = pl->p_final;
        if ((aux = malloc(sizeof(pelem))) == NULL) {return errno;}
        if ((aux->datos = malloc(sizeof(pdatos))) == NULL) {return errno;}
        aux->datos->pid_proc = pid;
        aux->datos->prioridade = prior;
        if (strcpy(aux->datos->lin_comm,lcom) == NULL) {return errno;}
        if (strcpy(aux->datos->tempo_ini,tmp) == NULL) {return errno;}
        aux->datos->estado = est;
        aux->datos->final = fin;
        aux->seg_pelem = NULL;
        pl->p_final->seg_pelem = aux;
        pl->p_final = aux;
        pl->p_numnodos++;
    }
    return 0;
}
/*****************************************************************************/
pelem* buscar_pnodo (plist pl, pid_t pid) {  /* EEEEEEEEEEEEESSSSSSSSSSTTTTTTTTTTTTTTEEEEEEEEEEE*/
    /* Devolve o nodo ANTERIOR ao que se quere borrar. */
    pelem* aux = pl.p_inicio;
    while (aux->seg_pelem != NULL && aux->seg_pelem->datos->pid_proc != pid) {
        aux = aux->seg_pelem;
    }
    if (aux->seg_pelem == NULL) return NULL;
    return aux;
}
/*****************************************************************************/
int borrar_pnodo (plist* pl, pid_t pid) {
    pelem* aux; pelem* aux2;
    /* caso borrar 1º nodo */
    if (pid == pl->p_inicio->datos->pid_proc) {
        if (pl->p_inicio == pl->p_final) pl->p_final = NULL;
        aux = pl->p_inicio;
        pl->p_inicio = aux->seg_pelem;
        free(aux->datos);
        free(aux);
        pl->p_numnodos--;
    } else {
        /* caso borrar o ultimo */
        if (pid == pl->p_final->datos->pid_proc) {
            aux = buscar_pnodo(*pl, pid); /* aux e o nodo penultimo */
            aux2 = pl->p_final;
            pl->p_final = aux;
            aux->seg_pelem = NULL;
            free(aux2->datos);
            free(aux2);
            pl->p_numnodos--;
        } else { /* caso borrar polo medio */
            if ((aux = buscar_pnodo(*pl, pid)) == NULL) {return -1;} /* no esta ese nodo na lista */
            else {
                aux2 = aux->seg_pelem;
                aux->seg_pelem = aux2->seg_pelem;
                free(aux2->datos);
                free(aux2);
                pl->p_numnodos--;
            }
        }
    }
    return 0;
}
/*****************************************************************************/
void borrar_plist (plist* pl) {
    pelem* aux = pl->p_inicio;
    if (pl->p_numnodos != VACIA) {
        while (pl->p_numnodos > 0) {
            free(aux->datos);
            aux = aux->seg_pelem;
            free(pl->p_inicio);
            pl->p_inicio = aux;
            pl->p_numnodos--;
        }
    }
    pl->p_final = NULL;
}
/*****************************************************************************/
void imprimir_plist(plist pl) {
    pelem* it;
    if (pl.p_numnodos == VACIA) {printf("Error: Lista vacia.\n");}
    else {
        it = pl.p_inicio;
        while(it != NULL) {
            printf("%d: %d %s %s %s", it->datos->pid_proc, it->datos->prioridade, 
            it->datos->lin_comm, it->datos->tempo_ini,convertir_estado(it->datos->estado));
            if (it->datos->estado == ACTIVO && it->datos->final != SIGCONT) {
                printf(" %s\n","----");
            } else {
                printf(" %s\n",NombreSenal(it->datos->final));
            }
            it = it->seg_pelem;
        }
    }
}
/*****************************************************************************/
void actualizar_plist(plist *pl) {
    pelem *it;
    int w, estado;
    if (pl->p_numnodos != VACIA) {
        it = pl->p_inicio;
        while (it != NULL) {
            estado = 0;
            if (it->datos->estado != TERMINADO) {it->datos->prioridade = getpriority(PRIO_PROCESS, it->datos->pid_proc);}
            w = waitpid(it->datos->pid_proc, &estado, WNOHANG | WUNTRACED | WCONTINUED);
            if (w == it->datos->pid_proc) {
                if (WIFEXITED(estado)) {
                    it->datos->estado = TERMINADO;
                    it->datos->final = SIGTERM;
                } else if (WIFSIGNALED(estado)) {
                    it->datos->estado = TERMINADO;
                    it->datos->final = WTERMSIG(estado);
                } else if (WIFSTOPPED(estado)) {
                    it->datos->estado = PARADO;
                    it->datos->final = WSTOPSIG(estado);
                } else if (WIFCONTINUED(estado)) {
                    it->datos->estado = ACTIVO;
                    it->datos->final = SIGCONT;
                } else {
                    it->datos->estado = ACTIVO;
                    it->datos->final = -1;
                }
            }
            it = it->seg_pelem;
        }
    }
}
/*****************************************************************************/
void info_pid(plist pl, int pid) {
    pelem* it = pl.p_inicio;
    while (it != NULL && it->datos->pid_proc != pid) {
        it = it->seg_pelem;
    }
    if (it != NULL) {
        printf("%d: %d %s %s %s", it->datos->pid_proc, it->datos->prioridade, 
            it->datos->lin_comm, it->datos->tempo_ini,convertir_estado(it->datos->estado));
            if (it->datos->estado == ACTIVO && it->datos->final != SIGCONT) {
                printf(" %s\n","----");
            } else {
                printf(" %s\n",NombreSenal(it->datos->final));
            }
    }
}
/*****************************************************************************/
int existe_plist(plist pl, int pid) {
    pelem* it = pl.p_inicio;
    int retorno = 1;
    while (it != NULL && it->datos->pid_proc != pid) it = it->seg_pelem;
    if (it != NULL) retorno = 0;
    return retorno;
}
/*****************************************************************************/
void borrarprocs (plist* pl, int que) {
    pelem* it = pl->p_inicio; pelem* it2;
    while (it != NULL) {
        it2 = it->seg_pelem;
        switch (que) {
        case TERM : if (it->datos->final == Senal("TERM")) {borrar_pnodo(pl, it->datos->pid_proc);} break;
        case SIG :  if (it->datos->final != Senal("TERM") && it->datos->estado != ACTIVO && it->datos->estado != PARADO) {borrar_pnodo(pl, it->datos->pid_proc);} break;
        }
        it = it2;
    }
}
/*****************************************************************************/
char* convertir_estado(int estado) {
    switch (estado) {
        case PARADO : return ("PARADO");
        case TERMINADO : return ("TERMINADO");
    }
    return ("ACTIVO");
}
/*****************************************************************************/
/*
void main (void) {
    
    plist p;
    pelem* it;
    init_plist(&p);
    imprimir_plist(p);
    printf("%d\n",insertar_plist(&p, (pid_t) 7, 5, "num", "un", 4, 0));
    printf("%d\n", p.p_numnodos);
    imprimir_plist(p);
    printf("%d\n",insertar_plist(&p, (pid_t) 5, 5, "num", "dois", 4, 0));
    printf("%s\n",p.p_final->datos->tempo_ini);
    printf("%s\n",p.p_inicio->seg_pelem->datos->tempo_ini);
    imprimir_plist(p);
    printf("%d\n",insertar_plist(&p, (pid_t) 8, 5, "num", "tres", 4, 0));
    imprimir_plist(p);
    printf("%d\n",insertar_plist(&p, (pid_t) 6, 5, "num", "catro", 4, 0));
    it = buscar_pnodo(p, 5);
    printf("pid debia ser 7 -> %d\n", it->datos->pid_proc);
    it = buscar_pnodo(p, 6);
    printf("pid debia ser 8 -> %d\n", it->datos->pid_proc);
    it = buscar_pnodo(p, 9);
    printf("o punteiro debia ser nil: %p\n", it);
    imprimir_plist(p); printf("\n");
    printf("borrase o primeiro\n");
    borrar_pnodo(&p, 7);
    imprimir_plist(p);
    printf("buscar_pnodo de 6 debe devolver o anterior que seria o pid 8 →→→→ %d\n", buscar_pnodo(p, 6)->datos->pid_proc);
    printf("borrase o ultimo\n");
    borrar_pnodo(&p, 6);
    imprimir_plist(p);
    printf("%d\n",insertar_plist(&p, (pid_t) 7, 5, "num", "un", 4, 0));
    printf("%d\n",insertar_plist(&p, (pid_t) 3, 5, "num", "dois", 4, 0));
    printf("despois de insertar o 7 e o 5 para poder borralos\n");
    imprimir_plist(p);
    printf("borrar  nodo medio 1\n");
    borrar_pnodo(&p, 8);
    imprimir_plist(p);
    printf("borrar nodo medio 2\n");
    borrar_pnodo(&p, 7);
    imprimir_plist(p);
    printf("facer que borre un inexitente despois de engadir un nodo\n");
    printf("%d\n",insertar_plist(&p, (pid_t) 7, 5, "num", "un", 4, 0));
    borrar_pnodo(&p, 56);
    imprimir_plist(p);
    //borrar_pnodo(&p, (pid_t) 8);
    //imprimir_plist(p); 
    //printf("a\n");
    //borrar_pnodo(&p, (pid_t) 7);
    //imprimir_plist(p); printf("\n");
    //borrar_pnodo(&p, (pid_t) 6);
    //imprimir_plist(p); printf("\n");
    borrar_plist(&p);
    printf("-Z %d\n", p.p_numnodos);
    imprimir_plist(p);
}*/

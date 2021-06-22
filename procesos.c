/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "procesos.h"

void Cmd_priority (elemento* aux, char cadea[]) {
    int errno_ant, gprior;
    aux = aux->next;
    if (aux == NULL || seguinte(seguinte(aux)) != NULL) {print_cadea(cadea);printf(" no encontrado\n"); return;}
    if (aux->next == NULL) {
        errno_ant = errno;
        errno = 0;
        gprior = getpriority(PRIO_PROCESS,(id_t) convertir_a_int(*aux->datos));
        if (errno == 0) {printf("A prioridade do proceso %s e %d \n", *aux->datos, gprior); errno = errno_ant;}
        else {perror("Non se puido obter informacion do proceso solicitado");}
    } else {
        if (setpriority(PRIO_PROCESS,(id_t) convertir_a_int(*aux->datos),(int) convertir_a_int(*aux->next->datos)) != 0) {perror("Non se puido cambiar a prioridade do proceso");}
        else {printf("A nova prioridade do proceso e: %s\n", *aux->next->datos);}
    }
}
/*****************************************************************************/
void Cmd_fork (elemento* aux, char cadea[]) {
    pid_t pid;
    if (aux->next != NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
    pid = fork();
    if (pid == -1) {perror("Imposible crear proceso fillo"); return;}
    else {
        if (pid != 0) {waitpid(pid, NULL,0);printf("Shell fillo finalizado. Volvendo a executar shell pai...\n");}
        else {printf("Iniciando shell proceso fillo...\n");}
    }
}
/*****************************************************************************/
void Cmd_exec (elemento* aux, char cadea[]) {
    char* argv[TAM_ARR]; 
    if (aux->next == NULL) {print_cadea(cadea);printf(" no encontrado\n");return;}
    else {
        if (*aux->next->datos[0] == '@') {
            if (setpriority(PRIO_PROCESS, (id_t) getpid(), atoi (*aux->next->datos + 1)) != 0) {perror("Non se puido cambiar a prioridade do proceso");}
            aux = aux->next;
        }
        pasar_a_array(aux->next, argv, PASAR_NORMAL);
        execvp(*aux->next->datos, argv);
        perror("Imposible executar o programa");
    }
}
/*****************************************************************************/
void exec_pplano (elemento* aux) {
    char* argv[TAM_ARR];
    pid_t pid;
    int fallo = 0;
    pid = fork();
    if (pid == -1) {perror("Non se puido crear o proceso"); return;}
    if (pid != 0) {waitpid(pid, NULL, 0); return;}
    if (pid == 0) {
        if (*aux->datos[0] == '@') {
            if (setpriority(PRIO_PROCESS, (id_t) getpid(), atoi(*aux->datos+1)) != 0) perror("Non se puido cambiar a prioridade");
            aux = aux -> next;
            if (aux == NULL) printf("ERROR: Non se especificou o programa a executar.\n");
        }
        pasar_a_array(aux, argv, PASAR_NORMAL);
        execvp(*aux->datos, argv);
            fallo = 1;
        if (fallo == 1) {
            perror("Non se puido executar o programa");
            kill(getpid(), SIGTERM);
            return;
        }
    }
}
/*****************************************************************************/
void exec_splano (elemento* aux, plist* pl, char cadea[], int tipo) {
    char* argv[TAM_ARR];
    char* hora = malloc(TAM_ARR * sizeof(char));
    pid_t pid;
    int fallo = 0;
    pid = fork();
    if (pid == -1) {perror("Non se puido crear o proceso"); return;}
    if (pid == 0) {
        if (*aux->datos[0] == '@') {
            if (setpriority(PRIO_PROCESS, (id_t) getpid(), atoi(*aux->datos+1)) != 0) perror("Non se puido cambiar a prioridade");
            aux = aux -> next;
            if (aux == NULL) printf("ERROR: Non se especificou o programa a executar.\n");
        }
        pasar_a_array(aux, argv, tipo);
        execvp(*aux->datos, argv);
            fallo = 1;
        if (fallo == 1) {
            perror("Non se puido executar o programa");
            kill(getpid(), SIGTERM);
            return;
        }
    }
    cadea[strlen(cadea) - 1] = '\0';
    obter_ult_acc(hora); hora[strlen(hora) - 1] = '\0';
    insertar_plist(pl, pid, getpriority(PRIO_PROCESS,pid),cadea,hora,ACTIVO,0);
    free(hora);
}
/*****************************************************************************/
void Cmd_pplano_splano_prog (elemento* aux, plist* pl, char cadea[], int tipo) {
    elemento* aux2 = aux;
    if (aux == NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
    switch (tipo) {
        case PPLANO : exec_pplano(aux); break;
        case SPLANO : exec_splano(aux, pl, cadea, PASAR_NORMAL); break;
        case PROG : while (aux2->next != NULL) 
                        aux2 = aux2->next;
                    if (strncmp(*aux2->datos, "&", 1) == 0) {
                        exec_splano(aux, pl, cadea, PASAR_SPLANO);
                    } else exec_pplano(aux);
    }
}
/*****************************************************************************/
void Cmd_listarprocs (elemento* aux, char cadea[], plist *pl) {
    if (aux != NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
    actualizar_plist(pl);
    imprimir_plist(*pl);
}
/*****************************************************************************/
void Cmd_proc (elemento* aux, char cadea[], plist* pl) {
    if (aux == NULL) {actualizar_plist(pl); imprimir_plist(*pl); return;}
    if (strcmp(*aux->datos, "-fg") == 0) {
        aux = aux->next;
        if (aux == NULL || aux->next != NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
        if (existe_plist(*pl, atoi(*aux->datos)) != 0) {
            actualizar_plist(pl);
            imprimir_plist(*pl);
        } else {
            waitpid(atoi(*aux->datos), NULL, 0);
            borrar_pnodo(pl, atoi(*aux->datos));
        }
    } else {
        if (aux->next != NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
        if (existe_plist(*pl, atoi(*aux->datos)) != 0) {actualizar_plist(pl); imprimir_plist(*pl); return;}
        else info_pid(*pl, atoi(*aux->datos));
    }
}
/*****************************************************************************/
void Cmd_borrarprocs (elemento* aux, char cadea[], plist* pl) {
    if (aux == NULL || aux->next != NULL) {print_cadea(cadea); printf(" no encontrado\n"); return;}
    if (strcmp(*aux->datos, "-term") == 0) {
        borrarprocs(pl, TERM);
    } else if (strcmp(*aux->datos, "-sig") == 0) {
        borrarprocs(pl, SIG);
    } else {print_cadea(cadea); printf(" no encontrado\n"); return;}
}

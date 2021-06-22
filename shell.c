/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

/* Directivas de preprocesados. */
#include <stdio.h> /* Conten as funcions basicas de entrada saida. */
#include <string.h> /* Conten funcions para o manexo de strings. */
#include <time.h> /* Conten funcions para o manexo do tempo do sistema. */
#include "utilidades.h"
#include "fendido.h"  /* Conten as funcions para o manexo da cadea de entrada. */
#include "hist.h"  /* Conten as funcions do historial */
/* Estas duas son necesarias para peticions ao sistema coma getpid(). */
#include <sys/types.h> 
#include <unistd.h>
/* Estas duas son necesarias para o malloc e o errno respectivamente. */
#include <stdlib.h>
#include <errno.h>

#include "ficheiros.h"


#include "memoria.h"  /* Conten as funcions da parte de memoria. */
/* Estas son necesarias para a parte de procesos (P3). */
#include "lista_proc.h"
#include "sinais.h"
#include "procesos.h"


/* Definicions de constantes. */
//#define MAX_TAM 256 /* Tamanho maximo de cadea que pode ler o shell. */
#define TAM_COM 7 /* Tamanho maximo de comando. */
#define TAM_MOD 256 /* Tamanho maximo de modificador(e path en caso de cdir). */
//#define MEM 4096 /* Tamanho maximo de memoria */

/* Tipos definidos polo usuario. */

/* Cabeceiras de funcions. */
void xestor(void);
void imprimirPrompt(void);
/*char procesarEntrada(char cadea[], char comando[], char mod[]);*/
void procesar_entrada(char cadea[], lista *l);
int executar(mem* historial, char cadea[], lista* l, list* mlist, plist* pl);


/* Programa principal. */
int main(void){
  xestor();
  return 0;
}
/******************************************************************************/
void imprimirPrompt(void){
  printf("-> ");
}
/******************************************************************************/
/* 
 * Bucle que controla todo o programa. Estan definidas nel as variables 
 * globais. 
 */
void xestor(void){
  mem historial;
  lista l;
  list mlist;
  plist pl;
  inicializar_lista(&l);
  init_mlist(&mlist);
  historial.final = -1;
  char cadea[MAX_TAM + 1];
  int finalizar = 1;
  while(finalizar == 1){
    imprimirPrompt();
    fgets(cadea, MAX_TAM, stdin);
    procesar_entrada(cadea, &l);
    finalizar = executar(&historial, cadea, &l, &mlist, &pl);
    borrar_lista(&l);
  }
  borrar_mlist(&mlist);
  borrar_historial(&historial);
  borrar_plist(&pl);
}
/******************************************************************************/
void procesar_entrada(char cadea[], lista *l){
  string aux;
  int i = 0; /* Variable contador. Permite movernos no string */
  do {
    /* Incializacion do array para controlar o funcionamento correcto do shell. */
    for (int x = 0; (x != MAX_TAM); x++){
      aux[x] = '\0';
    }
    while (cadea[i] == ' '){
      i++;
    }
    int j = 0; /* Declarase a variable que nos permite movernos no array. */
    /*
     * Neste bucle gardamos os elementos do 1º comando ata que encha o array,
     * atope un espazo (xa que implicaria fin de palabra) ou chegue a un salto
     * de linha (ca que remata a cadea).
     */
    while ((i < i + TAM_COM + 1) && (cadea[i] != (char) 32 ) && (cadea[i] != '\0') && (cadea[i] != '\n')){
      aux[j] = cadea[i];
      i++;
      j++;
    }
    if (aux[0] != '\0'){
      insertar(l,aux);
    }
  } while (cadea[i] != '\n');
}
/******************************************************************************/
/* Escolle o caminho de datos a partir das entradas. */
int executar(mem * historial, char cadea[],lista* l, list* mlist, plist* pl){
  int bool = 1; /* Valor de saida. Usase no manager. */
  elemento* aux = l->inicio;
  char d[256];
  if (aux != NULL){
    escribir_historial(historial,cadea);
    if (strcmp(*aux->datos,"autores") == 0){
      if (segundo(*l) == NULL){
        printf("Mario Rial Romero   : m.rial.romero\nMiguel Blanco Godon : miguel.blanco.godon\n");
      } else {
        aux = segundo(*l);
        if ((strcmp(*aux->datos, "-l") == 0) && (terceiro(*l) == NULL)){
          printf("m.rial.romero\nmiguel.blanco.godon\n");
        } else if ((strcmp(*aux->datos, "-n") == 0) && (terceiro(*l) == NULL)){
          printf("Mario Rial Romero\nMiguel Blanco Godon\n");
        } else {
          print_cadea(cadea); printf(" no encontrado\n");
        }
      } 
    } else if ((strcmp(*aux->datos, "exit") == 0) || (strcmp(*aux->datos, "fin") == 0) || (strcmp(*aux->datos, "end") == 0)) {
        if (segundo(*l) == NULL){
          bool = 0;
        } else {
          print_cadea(cadea); printf(" no encontrado\n");
        }
        
    } else if (strcmp(*aux->datos, "pid") == 0) {
        aux = segundo(*l);
        if (aux == NULL){
          pid_t pid;
          pid = getpid();
          printf("%d\n", pid);
        } else if ((strcmp(*aux->datos, "-p") == 0) && (terceiro(*l) == NULL)){
            pid_t ppid ;
            ppid = getppid();
            printf("%d\n", ppid);
        } else {
            print_cadea(cadea); printf(" no encontrado\n");
        }
    } else if (strcmp(*aux->datos, "fecha") == 0){
        aux = segundo(*l);
        if (aux == NULL){
          imprimir_data();
        } else {
            print_cadea(cadea); printf(" no encontrado\n");
        }
    } else if (strcmp(*aux->datos, "hora") == 0){
        aux = segundo(*l);
        if (aux == NULL) {
          imprimir_hora();
        } else {
            print_cadea(cadea); printf(" no encontrado\n");
        }
    } else if (strcmp(*aux->datos, "hist") == 0){
        aux = segundo(*l);
        if (aux == NULL){
          mostrar_historial(*historial);
        } else if ((strcmp(*aux->datos, "-c") == 0) && (terceiro(*l) == NULL)) {
            borrar_historial(historial);
            printf("Historico borrado\n");
        } else {
            print_cadea(cadea); printf(" no encontrado\n");
        }
    } else if (strcmp(*aux->datos, "cdir") == 0){
        aux = segundo(*l);
        if (aux == NULL){
            getcwd(d,256);
            puts(d);
        } else {
          if (chdir(*aux->datos) != 0){
            perror("Imposible cambiar de directorio: ");
          }
        }
    } else if (strcmp(*aux->datos, "crear") == 0){
          aux = segundo(*l);
          if (aux != NULL){
            if (strcmp(*aux->datos, "-d") == 0){
              if (terceiro(*l) == NULL){
                listar_crear_borrar(NULL, 0);
              } else {
                aux = terceiro(*l);
		while (aux != NULL) {
	                if (mkdir(*aux->datos, 0777) != 0) {
        	          printf("Imposible crear directorio '%s' : %s\n", *aux->datos, strerror(errno));
                	}
			aux = seguinte(aux);
		} 
              }
            } else {
		while (aux != NULL) {
	                FILE* file = fopen(*aux->datos, "r");
	                if (file != NULL){ 
	                  fclose(file);
	                  errno = EEXIST;
	                  printf("Imposible crear ficheiro '%s' : %s\n", *aux->datos, strerror(errno));
	                } else {
	                  file = fopen(*aux->datos,"w");
	                  fclose(file);
			}
		aux  = seguinte(aux);
                }
            }
          } else {
            listar_crear_borrar(NULL, 0);
          }  
    } else if (strcmp(*aux->datos, "listar") == 0) {
            listar(*l);
    } else if (strcmp(*aux->datos, "borrar") == 0) {
	    	aux = segundo(*l);
		  if (aux == NULL) {
		  	listar_crear_borrar(NULL, 0);
		  } else if (strcmp(*aux->datos, "-r")  == 0){
			  aux = terceiro(*l);
        if (aux == NULL){
          listar_crear_borrar(NULL, 0);
        } else {
	  		borrado_recursivo(aux);
        }
		} else {
			while (aux != NULL) {
				if(remove(*aux->datos) != 0){
					printf("Imposible borrar '%s' : %s\n", *aux->datos, strerror(errno));
				}
				aux = seguinte(aux);
			}
		}
    } else  if (strcmp(*aux->datos, "info" ) == 0) {
        info(l);
        /* A PARTIRES DAQUI PRACTICA 2 */
    } else if (strcmp(*aux->datos, "asignar") == 0) {
        Cmd_asignar(aux, mlist, cadea);
    } else if (strcmp(*aux->datos, "desasignar") == 0){        
        Cmd_desasignar(aux, mlist, cadea);
    } else if (strcmp(*aux->datos, "recursiva") == 0) {
        Cmd_recursiva_n(aux, cadea);
    } else if (strcmp(*aux->datos, "mem") == 0) {
        Cmd_mem(segundo(*l), *mlist, cadea);
    } else if (strcmp(*aux->datos, "volcar") == 0) {
        Cmd_volcar(aux, cadea);
    } else if (strcmp(*aux->datos, "llenar") == 0) {
        Cmd_llenar(aux, cadea);
    } else if(strcmp(*aux->datos, "borrarkey") == 0) {
        borrar_key(aux, cadea);
    } else if (strcmp(*aux->datos, "rfich") == 0) {
        Cmd_rfich(aux, cadea);
    } else if (strcmp(*aux->datos, "wfich") == 0){
        Cmd_wfich(aux, cadea);
    /* A PARTIRES DAQUI PARTE DE PROCESOS */
    } else if (strcmp(*aux->datos, "priority") == 0) {
        Cmd_priority(aux,cadea);
    } else if (strcmp(*aux->datos, "fork") == 0) {
        Cmd_fork(aux, cadea);
    } else if (strcmp(*aux->datos, "exec") == 0) {
        Cmd_exec(aux,cadea);
    } else if (strcmp(*aux->datos, "pplano") == 0) {
        Cmd_pplano_splano_prog(aux->next, pl, cadea, PPLANO);
    } else if (strcmp(*aux->datos, "splano") == 0) {
        Cmd_pplano_splano_prog(aux->next, pl, cadea, SPLANO);
    } else if (strcmp(*aux->datos, "proc") == 0) {
        Cmd_proc(aux->next, cadea, pl);
    } else if (strcmp(*aux->datos, "listarprocs") == 0) {
        Cmd_listarprocs(aux->next, cadea, pl);
    } else if (strcmp(*aux->datos, "borrarprocs") == 0) {
        Cmd_borrarprocs(aux->next, cadea, pl);
    } else { 
        Cmd_pplano_splano_prog(aux, pl, cadea, PROG);
    }
  }
  return bool;
}
/******************************************************************************/

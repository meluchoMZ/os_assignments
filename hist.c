/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "hist.h"
/******************************************************************************/
/* Engade unha nova entrada o historial insertando polo final. */
void escribir_historial(mem* historial, char cadea[]){
   historial->final++;
  historial->element[historial->final] = malloc(sizeof(string));
  /* Comprobamos erros ao reservar memoria. Se non se pode imprime error. */
  if (historial->element == NULL){
    errno = 1;
    perror("Imposible escribir en historial: ");
    historial->final--;
  } else {
    strcpy(*historial->element[historial->final], cadea);
  }
}
/********************************************************************************/
/* Elimina todos os elementos do historial liberando a memoria apuntada polos punteiros*/
void borrar_historial(mem* historial){
  for (int i = 0; i <= historial->final; i++){
    free(historial->element[i]);
    historial->element[i] = NULL;
  }
  historial->final = -1;
}
/********************************************************************************/
/* Imprime o historial de mais antigo a mais recente. */
void mostrar_historial(mem historial){
  for (int i = 0; i <= historial.final; i++){
    printf("%s\n", *historial.element[i]);
  }
}
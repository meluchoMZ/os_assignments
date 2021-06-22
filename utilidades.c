/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#include "utilidades.h"

/* Imprime unha cadea ata que atope un caracter nulo ou de fin de linha. */
void print_cadea(char cadea[]){
  for(int i = 0; (cadea[i] != '\n') && (cadea[i] != '\0'); i++){
    printf("%c", cadea[i]);
  }
}
/* Imprime a data no formato predefinido da funcion ctime(). */
void imprimir_data(void){
  time_t now;
  time(&now);
  printf("%s",ctime(&now));
}
/******************************************************************************/
/* Imprime por terminal a hora en formato H:M:S. */
void imprimir_hora(void){
  time_t tempo;
  tempo = time(NULL);
  struct tm *punt_tempo = localtime(&tempo);
  char hora[10];
  strftime(hora,10,"%H:%M:%S",punt_tempo);
  printf("%s\n",hora);
}
/********************************************************************************/
unsigned long int convertir_a_int(char str[]) {
  int n = strlen(str), acumulador, ninterior = n-1;
  unsigned long int result = 0;
  for (int i = 0; i < n; i++) {
    switch (str[i]) {
      case '0' : acumulador = 0;
                 break;
      case '1' : acumulador = 1;
                 break;
      case '2' : acumulador = 2;
                 break;
      case '3' : acumulador = 3;
                 break;
      case '4' : acumulador = 4;
                 break;
      case '5' : acumulador = 5;
                 break;
      case '6' : acumulador = 6;
                 break;
      case '7' : acumulador = 7;
                 break;
      case '8' : acumulador = 8;
                 break;
      case '9' : acumulador = 9;
                 break;
      default : acumulador = -1;
    }
    if (acumulador == -1){
      return 0;
      break;
    } else {
        result = result + acumulador * power(10, ninterior);
        ninterior--;
    }
  }
  return result;
}
/******************************************************************************/
int power(int a, int b) {
  int acum = 1;
  for (int i = 0; i < b; i++) {
    acum = acum * a;
  }
  return acum;
}
/******************************************************************************/
void obter_ult_acc(char hora[]) {
	time_t now;
  time(&now);
  strcpy(hora, ctime(&now));
}
/******************************************************************************/
void pasar_a_array (elemento* e, char* argv[], int tipo) {
  elemento* aux = e;
  int i = 0;
  while (aux != NULL && i < TAM_ARR) {
    if ((argv[i] = malloc(256*sizeof(char))) == NULL) {perror("Imposible reservar memoria"); return;}
    strcpy(argv[i],*aux->datos);
    aux = aux->next;
    i++;
  } 
  if (tipo == PASAR_SPLANO) {
    free(argv[i - 1]);
    argv[i - 1] = NULL;
  }
}
/******************************************************************************/
void ultimo_acceso (time_t* tempo){
  struct tm *punt_tempo = localtime(tempo);
  char hora[30];
  strftime(hora,30,"%d %b %H:%M",punt_tempo);
  printf(" %s ",hora);
}
/******************************************************************************/
void limpar_buffer (char* argv[],int tam) {
  int i = 0;
  while (i < tam) {
    if (argv[i] != NULL) {free(argv[i]); argv[i] = NULL;}
    i++;
  }
}
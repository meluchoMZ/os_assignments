/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#include "fendido.h"
/* ESTE ANACO E A IMPLEMENTACION DA LISTA NECESARIA PARA FACER O INTERPRETE: */
void insertar(lista* l, string s){
  elemento* aux;
  if (l->inicio == NULL) { /* Caso: lista vacia. */
    aux = malloc(sizeof(elemento));
    if (aux != NULL) { /* Reservouse memoria correctamente */
      aux->datos = malloc(sizeof(string));
      strcpy(*aux->datos, s);
      aux->next = NULL;
      l->inicio = aux;
      l->final = aux;
      l->num_nodos++;
    } else { /* Fallo na reserva de memoria */
      perror("Non se puido reservar memoria: \n");
    }
  } else {
    aux = malloc(sizeof(elemento));
    if (aux != NULL){ /* Correcta reserva de memoria*/
      aux->datos = malloc(sizeof(string));
      strcpy(*aux->datos,s);
      aux->next = NULL;
      l->final->next= aux;
      l->final = aux;
      l->num_nodos++; 
    }
  }
}
/********************************************************************************/
void imprimir_lista(lista l){
  elemento* it = l.inicio;
  for (int i = 0; i < l.num_nodos; i++){
    printf("%s ", *it->datos);
    it = it->next;
  }
}
/********************************************************************************/
void borrar_lista(lista *l){
  elemento* it = l->inicio;
  while(l->num_nodos > 0){
    free(it->datos);
    it = it->next;
    free(l->inicio);
    l->inicio = it;
    l->num_nodos--;
  }
  l->final = NULL;
}
/********************************************************************************/
elemento* seguinte(elemento* posicion){
  elemento* nxt;
  if (posicion != NULL){
    nxt = posicion->next;
  } else nxt = NULL;
  return nxt;
}
/********************************************************************************/
elemento* primeiro(lista l){
  elemento* e;
  e = l.inicio;
  return e;
}
/********************************************************************************/
elemento* segundo(lista l){
  elemento* e;
  e = seguinte(primeiro(l));
  return e;
}
/********************************************************************************/
elemento* terceiro(lista l){
  elemento* e;
  e = seguinte(segundo(l));
  return e;
}
/********************************************************************************/
elemento* cuarto(lista l){
  elemento* e;
  e = seguinte(terceiro(l));
  return e;
}
/********************************************************************************/
void inicializar_lista(lista* l){
  l->inicio = NULL;
  l->final = NULL;
  l->num_nodos = 0;
}
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#ifndef __FENDIDO_H
#define __FENDIDO_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAX_TAM 256
#define MEM 4096

typedef char string[MAX_TAM];
typedef struct nodo{
  string *datos;
  struct nodo *next;
}elemento;
typedef struct DirLIsta{
  elemento* inicio;
  elemento* final;
  int num_nodos;
}lista;

void insertar(lista *l, string s);
void imprimir_lista(lista l);
void borrar_lista(lista *l);
elemento* seguinte(elemento* posicion);
elemento* primeiro(lista l);
elemento* segundo(lista l);
elemento* terceiro(lista l);
elemento* cuarto(lista l);
void inicializar_lista(lista* l);

#endif
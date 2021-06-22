/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#ifndef __HIST_H
#define __HIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define MAX_TAM 256
#define MEM 4096

typedef char string[MAX_TAM];
typedef struct {
  string* element[MEM];
  int final;
} mem;

/* Funcions do historial. */
void escribir_historial(mem* historial, char cadea[]);
void mostrar_historial(mem historial);
void borrar_historial(mem* historial);
#endif
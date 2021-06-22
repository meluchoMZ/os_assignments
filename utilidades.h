/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#ifndef __UTILIDADES_H
#define __UTILIDADES_H

#include "fendido.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define TAM_ARR 256
#define PASAR_NORMAL 0
#define PASAR_SPLANO 1
#define BUSCAR_SPLANO 1

void print_cadea (char cadea[]);
void imprimir_data(void);
void imprimir_hora(void);
int power (int a, int b);
unsigned long int convertir_a_int (char str[]);
void obter_ult_acc (char hora[]);
void ultimo_acceso (time_t* tempo);
void pasar_a_array (elemento* e, char* argv[], int tipo);
void limpar_buffer (char* argv[],int tam);

#endif
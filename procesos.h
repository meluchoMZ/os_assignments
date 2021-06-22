/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 
#ifndef __PROCESOS_H
#define __PROCESOS_H

#include "lista_proc.h"
#include "fendido.h"
#include "utilidades.h"
#include "signal.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

#define PPLANO 0
#define SPLANO 1
#define PROG 2


void Cmd_priority (elemento* aux, char cadea[]);
void Cmd_fork (elemento* aux, char cadea[]);
void Cmd_exec (elemento* aux, char cadea[]);
void Cmd_pplano (elemento* aux, char cadea[]);
void Cmd_splano (elemento* aux, char cadea[], plist* pl);
void Cmd_prog (elemento* aux, plist* pl, char cadea[]);
void Cmd_pplano_splano_prog (elemento* aux, plist* pl, char cadea[], int tipo);
void Cmd_listarprocs (elemento* aux, char cadea[], plist* pl);
void Cmd_proc (elemento* aux, char cadea[], plist* pl);
void Cmd_borrarprocs (elemento* aux, char cadea[], plist* pl);
#endif
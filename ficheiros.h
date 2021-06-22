/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 


#ifndef __FICHEIROS_H
#define __FICHEIROS_H
/* Estas son necesarias para o manexo do sistema de ficheiros do SO */
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <unistd.h>
#include "fendido.h"
#include "utilidades.h"

/* Funcions parte de ficheiros. */
void listar(lista l);
void listar_crear_borrar(elemento* e, int V);
char TipoFichero (mode_t m);
char * ConvierteModo2 (mode_t m);
void info (lista* l);
void listar_longo(elemento* e, int V);
void borrado_recursivo(elemento* e);
void recursivo_listar(elemento* e, int L, int V, int comprobador);
#endif
/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "memoria.h"

void* asignar_malloc(unsigned long int tam, list* mlist) {
	char hora[100];
  void* ptr = NULL;
  elem* e = malloc(sizeof(elem));
  if (e == NULL) {
    perror("Imposible reservar memoria");
    return NULL;
  } else {
    e->dir_malloc = malloc(tam);
    if (e->dir_malloc == NULL) {
      perror("Imposible reservar memoria");
      return NULL;
    } else {
      strcpy(e->fich_nome, "");
      e->key_fd = 0;
      e->tamano = tam;
      obter_ult_acc(hora);
      strcpy(e->ult_acc, hora);
      e->tipo = MALLOC;
      inserir_mlist(e, mlist);
      ptr = e->dir_malloc;
    }
  }
  free(e);
  return ptr;
}
/******************************************************************************/
void * ObtenerMemoriaShmget (key_t clave, size_t tam, list* mlist) {
	void * p;
	int aux,id,flags=0777;
	struct shmid_ds s;
  char hora[100];
  elem e;
	if (tam) /*si tam no es 0 la crea en modo exclusivo */
		flags=flags | IPC_CREAT | IPC_EXCL;
				/*si tam es 0 intenta acceder a una ya creada*/
	if (clave==IPC_PRIVATE)	/*no nos vale*/
			{errno=EINVAL; return NULL;}
	if ((id=shmget(clave, tam, flags))==-1)
			return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1){
		aux=errno;	/*si se ha creado y no se puede mapear*/
		if (tam)	/*se borra */
			shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	shmctl (id,IPC_STAT,&s);
  e.dir_malloc = p;
  strcpy(e.fich_nome, "");
  e.key_fd = clave;
  e.tamano = tam;
  e.tipo = SHARED;
  obter_ult_acc(hora);
  strcpy(e.ult_acc, hora);
  inserir_mlist(&e, mlist);
	return (p);
}
/******************************************************************************/
void Cmd_AsignarCreateShared (char *arg[], list* mlist) {
	key_t k;
	size_t tam=0;
	void *p;
	if (arg[0]==NULL || arg[1]==NULL) {
		imprimir_shared(*mlist);
		return;
	}
	k=(key_t) atoi(arg[0]);
	if (arg[1]!=NULL)
		tam=(size_t) atoll(arg[1]);
	if ((p=ObtenerMemoriaShmget(k,tam, mlist))==NULL)
		perror ("Imposible obtener memoria shmget");
	else
		printf ("Memoria de shmget de clave %d asignada en %p\n",k,p);
}
/******************************************************************************/
void * MmapFichero (char * fichero, int protection, list *mlist) {
	int df, map=MAP_PRIVATE,modo=O_RDONLY;
	struct stat s;
	void *p; 
  elem e;
  char hora[100];
	if (protection&PROT_WRITE)
		modo=O_RDWR;
	if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
		return NULL;
	if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
		return NULL;
	e.dir_malloc = p;
  strcpy(e.fich_nome, fichero);
  e.key_fd = df;
  e.tamano = s.st_size;
  obter_ult_acc(hora);
  strcpy(e.ult_acc, hora);
  e.tipo = MMAP;
  inserir_mlist(&e,mlist);
	return p;
}
/******************************************************************************/
void Cmd_AsignarMmap (char *arg[], list* mlist) {
	char *perm;
	void *p;
	int protection=0;
	if (arg[0]==NULL){
		imprimir_mmap(*mlist);
    return;
    }
	if ((perm=arg[1])!=NULL && strlen(perm)<4) {
		if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
		if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
		if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
	}
	if ((p=MmapFichero(arg[0],protection, mlist))==NULL)
		perror ("Imposible mapear fichero");
  else
		printf ("fichero %s mapeado en %p\n", arg[0], p);
}
/******************************************************************************/
ssize_t LeerFichero (char *fich, void *p, ssize_t n) {
	ssize_t nleidos,tam=n;
	int df, aux;
	struct stat s;
	/*n=-1 indica que se lea todo el fichero*/
	if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
		return ((ssize_t)-1);
	if (n==LEERCOMPLETO)
		tam=(ssize_t) s.st_size;
	if ((nleidos=read(df,p, tam))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return ((ssize_t)-1);
	}
	close (df);
	return (nleidos);
}
/******************************************************************************/
void Cmd_borrakey (char *args[]) {
	key_t clave;
	int id;
	char *key=args[0];
	if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
		printf ("rmkey clave_valida\n");
		return;
	}
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}
	if (shmctl(id,IPC_RMID,NULL)==-1)
		perror ("shmctl: imposible eliminar memoria compartida\n");
  else {printf("Memoria compartida de clave %s eliminada.\n",key);}
}
/******************************************************************************/
void doRecursiva (int n) {
  char automatico[2048];
  static char estatico[2048];

  printf("parametro n: %d en %p\n", n, &n);
  printf("array estatico en: %p \n", estatico);
  printf("array automatico en %p\n", automatico);

  n--;
  if (n > 0) {
    printf("\n");
    doRecursiva(n);
  }
}
/******************************************************************************/
void volcar (void* addr, int bytes) {
  char* i = (char*) addr + bytes;
  char* ch = (char*) addr;
  while (ch < i) {
    for (int i = 0; i < 25; i++) printf("%2c ", ch[i]);
    printf("\n");
    for (int i = 0; i < 25; i++) printf("%.2x ", ch[i]);
    printf("\n\n");
    ch += 25;
  }
}
/******************************************************************************/
void llenar (void* addr, int bytes, int byte) {
  char* i = (char*) addr + bytes;
  char* ch = (char*) addr;
  while (ch < i) {
    *ch = byte;
    ch++;
  }
}
/******************************************************************************/
void Cmd_mem (elemento* it, list mlist, char cadea[]) {
  int auto1, auto2, auto3;
  if (it == NULL) {
    printf("Variables locales:  %p,  %p,  %p\n", &auto1, &auto2, &auto3);
    printf("Variables globales: %p,  %p,  %p\n", &global1, &global2, &global3);
    printf("Funciones programa: %p,  %p,  %p\n", (void *) MmapFichero, (void *) asignar_malloc, (void *) Cmd_asignar);
  } else {
    if (it->next != NULL) {
      print_cadea(cadea); printf(" no encontrado\n");
    } else {
      if (strcmp(*it->datos, "-malloc") == 0) {if(imprimir_malloc(mlist) == 0) printf("Error: lista vacia.\n");}
      else if (strcmp(*it->datos, "-shared") == 0) {if(imprimir_shared(mlist) == 0) printf("Error: lista vacia.\n");}
      else if (strcmp(*it->datos, "-mmap") == 0) {if(imprimir_mmap(mlist) == 0) printf("Error: lista vacia.\n");}
      else if (strcmp(*it->datos, "-all") == 0) imprimir_mlist(mlist);
      else {print_cadea(cadea); printf(" no encontrado\n");}
    }
  }
}
/******************************************************************************/
void Cmd_asignar (elemento* it, list *mlist, char cadea[]) {
  unsigned long int conv;
  void* ptr;
  char* arg[2];
  if (it->next == NULL) {
    imprimir_mlist(*mlist);
  } else {
    it = it->next; /*segundo*/
    if (strcmp(*it->datos, "-malloc") == 0) {
      it = it->next;
      if (it ==  NULL) {
        if (imprimir_malloc(*mlist) == 0) {printf("Error: lista vacia.\n");}
      } else {
        if (it->next != NULL) {
          print_cadea(cadea); printf(" no encontrado\n");
        } else {
          conv = convertir_a_int(*it->datos);
          if ((ptr = asignar_malloc(conv, mlist)) != NULL) {printf("Asignados %ld bytes en: %p\n", conv, ptr);}
        }
      }
    } else if (strcmp(*it->datos, "-mmap") == 0) {
      it = it->next;
      if (it == NULL) {
        if (imprimir_mmap(*mlist) == 0) {printf("Error: lista vacia.\n");}
      } else {
        if (seguinte(seguinte(it)) != NULL) {
          print_cadea(cadea); printf(" no encontrado\n");
        } else {
          arg[0] = *it->datos;
          if (it->next ==  NULL) {arg[1] = NULL;}
          else {arg[1] = *it->next->datos;}
          Cmd_AsignarMmap(arg, mlist);
        }
      }
    } else if (strcmp(*it->datos, "-crearshared") == 0) {
      if (it != NULL) {
            it = it->next; /* terceiro */
            if (it == NULL) {
              if (imprimir_shared(*mlist) == 0) printf("Error: lista vacia.\n");
            } else {
              if ((it->next ==  NULL) || (seguinte(it->next) != NULL)) {
                print_cadea(cadea); printf(" no encontrado\n");
              } else {
                char * arg[2];
                arg[0] = *it->datos;
                arg[1] = *it->next->datos;
                Cmd_AsignarCreateShared(arg, mlist);
              }
            }
      }
    } else if (strcmp(*it->datos, "-shared") == 0) {
      it = it->next;
      if (it == NULL) {
        if (imprimir_shared(*mlist) == 0) printf("Error: lista vacia.\n");
      } else {
        if (it->next != NULL) {
          print_cadea(cadea); printf(" no encontrado\n");
        } else {
          void * ptr = ObtenerMemoriaShmget(convertir_a_int(*it->datos), 0, mlist);
          if (ptr == NULL) {perror ("Non se puido asignar a memoria shared");} 
          else {printf("Asignada memoria shared (key %ld) en %p\n", convertir_a_int(*it->datos), ptr);}
        }
      }
    } else {print_cadea(cadea); printf(" no encontrado\n");}
  }
}
/******************************************************************************/
void Cmd_desasignar (elemento* aux, list *mlist, char cadea[]) {
  aux = aux->next;
        if (aux == NULL) {
          imprimir_mlist(*mlist);
        } else if (strcmp(*aux->datos, "-malloc") == 0) {
            aux = aux->next;
            if (aux == NULL) {
              if (imprimir_malloc(*mlist) == 0) {printf("Error: Lista vacia.");}
            } else {
              if (aux->next != NULL) {print_cadea(cadea); printf(" no encontrado\n");}
              else {borrar_nodo_tam(mlist, convertir_a_int(*aux->datos));}
            }
        } else if (strcmp(*aux->datos, "-mmap") == 0) {
            aux = aux->next;
            if(aux == NULL) {if (imprimir_mmap(*mlist) == 0) printf("Error:lista vacia.\n");}
            else {borrar_nodo_mmap(mlist, *aux->datos);}
        } else if (strcmp(*aux->datos, "-shared") == 0) {
            aux = aux->next;
            if (aux == NULL) {
              imprimir_shared(*mlist);
            } else {
                if (aux->next != NULL) {print_cadea(cadea); printf(" no encontrado.\n");}
                else {
                  int i = buscar_shared(*mlist, convertir_a_int(*aux->datos));
                  if( i == -1) {printf("Non se puido desasignar shared: Non existe\n");} 
                  else {if (shmdt(mlist->datos[i]->dir_malloc) == 0) {borrar_nodo_shared(mlist, mlist->datos[i]->dir_malloc);}}
                }
            }
        } else { /* Implica o uso de desasignar addr. */
          if (aux->next == NULL) {
            borrar_nodo_addr(mlist, (void *) strtoull(*aux->datos, NULL, 16));
          } else {
            print_cadea(cadea); printf(" no encontrado\n");
          }
        }
}
/******************************************************************************/
void Cmd_recursiva_n (elemento* aux, char cadea[]) {
  aux = aux->next;
  if((aux == NULL) || aux->next != NULL) {
    print_cadea(cadea); printf(" no encontrado\n");
  } else {
    doRecursiva(convertir_a_int(*aux->datos));
  }
}
/******************************************************************************/
void Cmd_volcar (elemento*aux, char cadea[]) {
  aux = aux->next;
  if ((aux == NULL) || (seguinte(aux->next) != NULL)) {
    print_cadea(cadea); printf(" no encontrado\n");
  } else {
    if (aux->next == NULL) {
      volcar((void*) strtoull(*aux->datos, NULL, 16), 25);
    } else {
      volcar ((void*) strtoull(*aux->datos, NULL, 16), convertir_a_int(*aux->next->datos));
    }
  }
}
/******************************************************************************/
void Cmd_llenar (elemento* aux, char cadea[]) {
  aux = aux->next; /* segundo */
        if ((aux == NULL) || (seguinte(seguinte(aux->next)) != NULL)) {
          print_cadea(cadea); printf(" no encontrado\n");
        } else {
          if (aux->next == NULL) {
            llenar((void*) strtoull(*aux->datos, NULL, 16), 128, 0x41 - '\0');
          } else {
            if (seguinte(aux->next) == NULL) {
              llenar((void*) strtoull(*aux->datos, NULL, 16), convertir_a_int(*aux->next->datos),0x41 - '\0');
            } else {
              elemento* aux2 = seguinte(aux->next);
              if (aux2->next != NULL) {
                print_cadea(cadea); printf(" no encontrado\n");
              } else {
                string s; strcpy(s, *aux2->datos);
                unsigned long int ascii = (unsigned long int) strtoull(*aux2->datos, NULL, 0);
                if ( (int) s[0] != 39 && ascii <= 128)
                  llenar((void*) strtoull(*aux->datos, NULL, 16), convertir_a_int(*aux->next->datos),ascii);
                else
                if ((strlen(*aux2->datos) > 2) && (s[0] == '0' && s[1] == 'x')) 
                  llenar((void*) strtoull(*aux->datos, NULL, 16), convertir_a_int(*aux->next->datos),ascii);
                else 
                if (strlen(*aux2->datos) != 3 || (int) s[0] != 39 || (int) s[2] != 39) {
                    print_cadea(cadea); printf(" %c no encontrado\n", s[2]);
                } else 
                  llenar((void*) strtoull(*aux->datos, NULL, 16), convertir_a_int(*aux->next->datos),(int) s[1]);
              }
            }
          }
        }
}
/******************************************************************************/
void borrar_key (elemento* aux, char cadea []) {
  aux = aux->next;
  char* args[1];
  if ((aux == NULL) || (aux->next != NULL)) {print_cadea(cadea); printf(" no encontrado\n");}
  else {
    args[0] = *aux->datos;
    Cmd_borrakey(args);
  }
}
/******************************************************************************/
void Cmd_rfich (elemento* aux, char cadea[]) {
  aux = aux->next;
        if ((aux == NULL) || (seguinte(seguinte(aux->next))) != NULL) {
          print_cadea(cadea); printf(" no encontrado\n");
        } else {
          elemento* aux2 = aux->next;
          if (aux2 == NULL) {
            print_cadea(cadea); printf(" no encontrado\n");
          } else {
            if (aux2->next == NULL) {  /* Caso que non recibe cont. */
                int nlidos;
                if ((nlidos = LeerFichero(*aux->datos, (void *) strtoull(*aux2->datos, NULL, 16),LEERCOMPLETO)) == -1) {
                  perror("Imposible acceder a memoria");
                } else {
                  printf("Lidos %d bytes en %p\n", nlidos, (void *) strtoull(*aux2->datos, NULL, 16));
                }
            } else { /* Caso que recibe cont. */
              int nlidos;
              if ((nlidos = LeerFichero(*aux->datos, (void *) strtoull(*aux2->datos, NULL, 16),convertir_a_int(*aux2->next->datos))) == -1) {
                  perror("Imposible acceder a memoria");
                } else {
                  printf("Lidos %d bytes en %p\n", nlidos, (void *) strtoull(*aux2->datos, NULL, 16));
                }
            }
          }
        }
}
/******************************************************************************/
ssize_t Escribir_fichero (char* fich, void * p, ssize_t n, int flag) {
  ssize_t nesc = 0, tam = n;
  int fd, aux;
  struct stat s;
  if (flag == 0) { /* con sobreescritura */
    unlink(fich);
    if ((fd=open(fich, O_CREAT | O_WRONLY, S_IRWXU)) == -1 || stat (fich, &s) == -1 ) 
      {return ((ssize_t) -1);}
  } else { /* sen sobreescritura */
    if ((fd=open(fich, O_APPEND | O_CREAT | O_WRONLY, S_IRWXU)) == -1 || stat (fich, &s) == -1 ) 
      {return ((ssize_t) -1);}
  }

  if ((nesc = write(fd, p, tam)) == -1) {
    aux=errno;
    close(fd);
    errno=aux;
    return ((ssize_t) -1);
  }
  close(fd);
  return nesc;
}
/******************************************************************************/
void Cmd_wfich (elemento* aux, char cadea[]) {
  int nescritos = 0, flag = 1;
  /*segundo*/
  if ((aux == NULL) || (seguinte(aux) == NULL) || (seguinte(seguinte(aux)) == NULL))
    {print_cadea(cadea); printf(" no encontrado\n");}
  else {
    aux = aux->next;
    if (aux != NULL) {
      if (strcmp(*aux->datos, "-o") == 0) {aux = aux->next; flag = 0;}
      nescritos = Escribir_fichero(*aux->datos,(char *) strtoul(*aux->next->datos, NULL, 16), convertir_a_int(*aux->next->next->datos), flag);
      if (nescritos == -1) {perror("Non se puido escribir no ficheiro"); return;}
      printf("Escritos %d bytes en %s.\n", nescritos, *aux->next->datos);
    }
  }
}
/******************************************************************************/
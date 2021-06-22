/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "ficheiros.h"

/********************************************************************************/
/* Lista dependendo do codigo. */
void listar(lista l){
  elemento* aux = primeiro(l);
  aux = seguinte(aux);
  int comp = 0, L = 0, V = 0, R = 0;
  char direxec[ MAX_TAM]; 
  getcwd(direxec, MAX_TAM);
  while((aux != NULL) && (comp != 1)){
    if (strcmp(*aux->datos, "-l") == 0){
      L = 1;
    } else if (strcmp(*aux->datos, "-v") == 0){
      V = 1;
    } else if (strcmp(*aux->datos, "-r") == 0){
      R = 1;
    } else {
      comp = 1;
    }
    
    if (comp == 0){
      aux = seguinte(aux);
    }
  }
  if (aux == NULL){
    aux = malloc(sizeof(elemento));
    aux->datos = malloc(sizeof(string));
    strcpy(*aux->datos, "./");
    aux->next = NULL;
    if (R == 1){
      recursivo_listar(aux, L, V, 0);
    } else if (L == 0) {
      listar_crear_borrar(NULL, V);
    } else {
      listar_longo(aux, V);
    }
    free(*aux->datos);
    free(aux);
    aux = NULL;
  } else {
    if (R == 1) {
      while(aux != NULL){
        if(chdir(*aux->datos) == 0){
          chdir("..");
          chdir(*aux->datos);
          strcpy(*aux->datos, "./");
          recursivo_listar(aux, L, V, 0);
          chdir("..");
        } else {
          perror("Non se puido acceder o directorio: ");
        }
        aux = seguinte(aux);
      }
    } else if (L == 1){
        listar_longo(aux, V);
    } else {
      while (aux != NULL) {
        listar_crear_borrar(aux, V);
        aux = seguinte(aux);
      }
    }
  }
}

/********************************************************************************/
void listar_crear_borrar(elemento* e, int V){
  DIR* dir;
  struct dirent* rdir;
  struct stat* st = malloc(sizeof(struct stat));
  char d[MAX_TAM];
  char ad[MAX_TAM];
  char rd[MAX_TAM];
  getcwd(d,MAX_TAM);
  if (e == NULL){
    strcpy(ad,"./");
    dir = opendir("./");
  } else {
    strcpy(ad,*e->datos);
    dir = opendir(*e->datos);
  }
  if (chdir(ad) == 0){
    getcwd(rd, MAX_TAM);
    printf("********* %s \n", rd);
  if (dir == NULL){
    perror("Non se puido acceder o directorio: ");
  } else {
    rdir = readdir(dir);
    if (rdir == NULL){
      perror("Non se puido leer o contido do directorio: ");
    } else {
      if (V == 0){
        while (rdir != NULL) { 
         stat(rdir->d_name, st);
          printf("%s ", rdir->d_name);
          printf("%ld \n", st->st_size) ;
          rdir = readdir(dir);
       }
      } else {
        while (rdir != NULL) { 
          if (strncmp(rdir->d_name, "./", 1) != 0){
            stat(rdir->d_name, st);
            printf("%s ", rdir->d_name);
            printf("%ld \n", st->st_size) ;
          }
          rdir = readdir(dir);
       }
      }
       if (closedir(dir) != 0){
        perror("Non se puido pechar o directorio: ");
       }
    }
    }
  } else {
     printf("********* %s \n",*e->datos);
    perror("Non se puido acceder o directorio: ");
  }
  chdir(d);
  free(st);
}
/******************************************************************************/
void borrado_recursivo(elemento* e){
	char d[MAX_TAM];
	char rd[MAX_TAM];
	DIR* dir;
	struct dirent* rdir;
	elemento* nome;
	getcwd(d, MAX_TAM);
	while (e != NULL){
		if (remove(*e->datos) != 0){
			if (chdir(*e->datos) == 0){
				getcwd(rd, MAX_TAM);
        chdir(d);
				dir = opendir(*e->datos);
				if (dir != NULL){
          chdir(rd);
					rdir = readdir(dir);
						if(rdir != NULL){
							while (rdir != NULL){
								nome = malloc(sizeof(elemento));
								nome->datos = malloc(sizeof(string));
								strcpy(*nome->datos, rdir->d_name);
								nome->next = NULL;
								if ((strncmp(*nome->datos, ".\0",2) != 0) && (strncmp(*nome->datos, "..\0",3) != 0)){
									borrado_recursivo(nome);
								}
								rdir = readdir(dir);
                free(nome->datos);
								free(nome);
								chdir(rd);
							}
						}
				}
				chdir(d);
        if (remove(*e->datos) != 0){
          perror("Non se puido borrar: ");
        }
        closedir(dir);
			} else {
				perror("Non e puido borrar: ");
			}
		} 
		e = seguinte(e);
	}

}
/******************************************************************************/
char TipoFichero (mode_t m) {
  switch (m&S_IFMT){ /*and bit a bit con los bits de formato,0170000 */
    case S_IFSOCK:return 's'; /*socket */
    case S_IFLNK:return 'l';/*symbolic link*/
    case S_IFREG:return '-';/* fichero normal*/
    case S_IFBLK:return 'b';/*block device*/
    case S_IFDIR:return 'd';/*directorio */
    case S_IFCHR:return 'c';/*char device*/
    case S_IFIFO:return 'p';/*pipe*/
    default: return '?';/*desconocido, no deberia aparecer*/
  }

}
/******************************************************************************/
char * ConvierteModo2 (mode_t m) {
  static char permisos[12];
  strcpy (permisos,"---------- ");
  permisos[0]=TipoFichero(m);

  if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
  if (m&S_IWUSR) permisos[2]='w';
  if (m&S_IXUSR) permisos[3]='x';
  if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
  if (m&S_IWGRP) permisos[5]='w';
  if (m&S_IXGRP) permisos[6]='x';
  if (m&S_IROTH) permisos[7]='r'; /*resto*/
  if (m&S_IWOTH) permisos[8]='w';
  if (m&S_IXOTH) permisos[9]='x';
  if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
  if (m&S_ISGID) permisos[6]='s';
  if (m&S_ISVTX) permisos[9]='t';
  return (permisos);
}
/******************************************************************************/
void info (lista* l){
  elemento* aux = segundo(*l);
  struct stat* st = malloc(sizeof(struct stat));
  struct passwd* usr;
  struct group* gr;
  FILE* link;
  char linkname[MAX_TAM];
  char* d;
  FILE* file;
  if (aux == NULL){
            printf("ERROR: Faltan o/os arquivos/arquivos do cal quere informacion.\n");
        } else {
            while (aux != NULL){
              file = fopen(*aux->datos, "r");
              if (file != NULL){ 
	                  
                lstat(*aux->datos, st);
               if (st != NULL){
                d = ConvierteModo2(st->st_mode);
                  usr = getpwuid(st->st_uid);
                  gr = getgrgid(st->st_gid);
                if ((st != NULL) && (usr != NULL) && (gr != NULL)){
                  printf("%10ld %s %2ld  %10s %10s %10ld", st->st_ino, d, st->st_nlink ,usr->pw_name, gr->gr_name, st->st_size);
                  ultimo_acceso(&st->st_mtime);
                  if (d[0] == 'l') {
                    link = fopen(*aux->datos,"r");
                    readlink(*aux->datos,linkname, MAX_TAM);
                    printf(" %s->%s\n",  *aux->datos, linkname);
                    fclose(link);
                  } else {
                    printf(" %s\n",  *aux->datos);
                  }
                } else {
                  printf("Imposible acceder ficheiro '%s' : %s\n", *aux->datos, strerror(errno));
                }
                fclose(file);
               }
              } else {
	                  printf("Imposible acceder ficheiro '%s' : %s\n", *aux->datos, strerror(errno));
              }
              aux = seguinte(aux);
            }
            free(st);
        }
}
/******************************************************************************/
void listar_longo(elemento* e, int V){
  DIR* dir;
  struct dirent* rdir;
  struct stat* st = malloc(sizeof(struct stat));
  struct passwd* usr;
  struct group* gr;
  FILE* link;
  char* d;
  char linkname[MAX_TAM];
  char rd[MAX_TAM];
  char chd[MAX_TAM];
  getcwd(chd,MAX_TAM);
  while (e != NULL){
    dir = opendir(*e->datos);
    if (chdir(*e->datos) == 0){
    if (dir == NULL){
      printf("Imposible acceder ficheiro '%s' : %s\n", *e->datos, strerror(errno));
    } else {
      getcwd(rd, MAX_TAM);
      printf("********* %s \n", rd);
      rdir = readdir(dir);
      if (rdir ==  NULL){
         printf("Imposible acceder ficheiro '%s' : %s\n", *e->datos, strerror(errno));
      } else {
        if (V == 0){ /* Non se escolleu opcion -v.*/
          while (rdir != NULL){
            lstat(rdir->d_name, st);
               if (st != NULL){
                d = ConvierteModo2(st->st_mode);
                  usr = getpwuid(st->st_uid);
                  gr = getgrgid(st->st_gid);
            printf("%10ld %s %2ld  %10s %10s %10ld", st->st_ino, d, st->st_nlink ,usr->pw_name, gr->gr_name, st->st_size);
            ultimo_acceso(&st->st_mtime);
            if (d[0] == 'l') {
              link = fopen(rdir->d_name,"r");
              readlink(rdir->d_name,linkname, MAX_TAM);
              printf(" %s->%s\n",  rdir->d_name, linkname);
              fclose(link);
            } else {
              printf(" %s\n",  rdir->d_name);
            }
            
            rdir = readdir(dir);
            }
          }
        } else { /* Escolleuse opcion -v.*/
            while (rdir != NULL){
              if (strncmp(rdir->d_name, ".",1) != 0){
                lstat(rdir->d_name, st);
               if (st != NULL){
                d = ConvierteModo2(st->st_mode);
                  usr = getpwuid(st->st_uid);
                  gr = getgrgid(st->st_gid);
                  printf("%10ld %s %2ld  %10s %10s %10ld", st->st_ino, d, st->st_nlink ,usr->pw_name, gr->gr_name, st->st_size);
                ultimo_acceso(&st->st_mtime);
                if (d[0] == 'l') {
                    link = fopen(rdir->d_name,"r");
                    readlink(rdir->d_name,linkname, MAX_TAM);
                    printf(" %s->%s\n",  rdir->d_name, linkname);
                    fclose(link);
                  } else {
                    printf(" %s\n",  rdir->d_name);
                  }
              rdir = readdir(dir);
              }
            } else {
              rdir = readdir(dir);
            }
        }
      }
      closedir(dir);
    }
    e = seguinte(e);
   }
  } else {
    perror("Non se puido acceder a directorio: ");
    e = seguinte(e);
  }
  chdir(chd);
  }
  free(st);
}
/******************************************************************************/
void recursivo_listar(elemento* e, int L, int V, int comprobador){
  char d[MAX_TAM];
  char rd[MAX_TAM];
  DIR* dir;
  struct dirent* rdir;
  elemento* rec = malloc(sizeof(elemento));
  getcwd(d,MAX_TAM);
    if (chdir(*e->datos) != 0){
      if (comprobador == 0){/* Caso primeira iteracion */
        printf("Imposible acceder ficheiro '%s' : %s\n", *e->datos, strerror(errno));
      }
    } else {
        chdir(d);
        if (L == 0){
          chdir(d);
          listar_crear_borrar(e, V);
        } else {
          listar_longo(e, V);
        }
          dir = opendir(*e->datos);
          if (dir == NULL){
            printf("Imposible acceder directorio '%s' : %s\n", *e->datos, strerror(errno));
          } else {
            rdir = readdir(dir);
            chdir(*e->datos);
            while (rdir != NULL){
              getcwd(rd,MAX_TAM);
              if ((strncmp(rdir->d_name, ".\0", 2) != 0) && (strncmp(rdir->d_name, "..\0", 3) != 0)){
                strcat(rd,"/");
                strcat(rd, rdir->d_name);
                rec->datos = malloc(sizeof(string));
                strcpy(*rec->datos, rd);
                rec->next = NULL;
                if (chdir(rdir->d_name) == 0){
                  recursivo_listar(rec, L, V, 0);
                } 
                free(rec->datos);
                
              }
              chdir(d);
              rdir = readdir(dir);
              
            }
            
          }
         closedir(dir) ;
    }
  chdir(d);
  free(rec);
}

/******************************************************************************/

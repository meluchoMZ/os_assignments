all: list
	gcc -o shell shell.c utilidades.c hist.c fendido.c ficheiros.c list.c memoria.c lista_proc.c sinais.c procesos.c  -Wall -Wextra -g
list: list.c
	gcc -c list.c -Wall -Wextra

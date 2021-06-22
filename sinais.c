/*****************************************************************************/
/*
 * AUTORES:
 * Mario Rial Romero  (m.rial.romero). DNI: 58004451S
 * Miguel Blanco Godon  (miguel.blanco.godon). DNI: 78809752E
 */ 

#include "sinais.h"

struct SEN {
	char *nombre;
	int senal;
};

static struct SEN sigstrnum[] = {
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL},
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2},
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP},
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH},
	{"IO", SIGIO},
	{"SYS", SIGSYS},
	/*senales que no hay en todas partes*/
	#ifdef SIGPOLL
		{"POLL", SIGPOLL},
	#endif

	#ifdef SIGPWR
		{"PWR", SIGPWR},
	#endif

	#ifdef SIGEMT
		{"EMT", SIGEMT},
	#endif

	#ifdef SIGINFO
		{"INFO", SIGINFO},
	#endif

	#ifdef SIGSTKFLT
		{"STKFLT", SIGSTKFLT},
	#endif

	#ifdef SIGCLD
		{"CLD", SIGCLD},
	#endif

	#ifdef SIGLOST
		{"LOST", SIGLOST},
	#endif

	#ifdef SIGCANCEL
		{"CANCEL", SIGCANCEL},
	#endif

	#ifdef SIGTHAW
		{"THAW", SIGTHAW},
	#endif

	#ifdef SIGFREEZE
		{"FREEZE", SIGFREEZE},
	#endif

	#ifdef SIGLWP
		{"LWP", SIGLWP},
	#endif

	#ifdef SIGWAITING
		{"WAITING", SIGWAITING},
	#endif

	{NULL,-1},

};
/*fin array sigstrnum */

int Senal(char * sen) /*devuel el numero de senial a partir del nombre*/ {
	int i;
	for (i=0; sigstrnum[i].nombre!=NULL; i++)
	if (!strcmp(sen, sigstrnum[i].nombre))
	return sigstrnum[i].senal;
	return -1;
}
char *NombreSenal(int sen) /*devuelve el nombre senal a partir de la senal*/ {
/* para sitios donde no hay sig2str*/
	int i;
	for (i=0; sigstrnum[i].nombre!=NULL; i++)
	if (sen==sigstrnum[i].senal)
	return sigstrnum[i].nombre;
	return ("SIGUNKNOWN");
}
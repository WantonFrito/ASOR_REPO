#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

volatile int sigusr_flag, sigalarm_flag;

static void sigusr1_action(int arg) {
	sigusr_flag = 1;
}	

static void sigalarm_action(int arg) {
	sigalarm_flag = 1;
}

int main(int argc, char ** argv) { 
	if (argc != 2) {
		fprintf(stderr, "Uso: %s <secs>\n", argv[0]);
		return -1;
	}	
	
	sigset_t sig_set;
	struct sigaction sigalarm_struct, sigusr_struct;

	sigusr_flag = 0;
	sigalarm_flag = 0;
	sigfillset(&sig_set);
	sigdelset(&sig_set, SIGALRM);
	sigdelset(&sig_set, SIGUSR1);
	
	sigalarm_struct.sa_handler = sigalarm_action;
	sigalarm_struct.sa_mask = sig_set;
	sigalarm_struct.sa_flags = 0;
	sigusr_struct.sa_handler = sigusr1_action;
	sigusr_struct.sa_mask = sig_set;
	sigusr_struct.sa_flags = 0;

	sigaction(SIGALRM, &sigalarm_struct, NULL);
	sigaction(SIGUSR1, &sigusr_struct, NULL);

	alarm(atoi(argv[1]));
	printf("El ejecutable se borrará en %d segundos. Si quiere parar, envíe SIGUSR1 al proceso\n", atoi(argv[1]));
	
	sigsuspend(&sig_set);

	if (sigalarm_flag) {

		unlink(argv[0]);
		printf("Ejecutable borrado\n");

	}

	else if (sigusr_flag) {

		printf("Se ha cancelado el borrado del archivo\n");

	}

	else { 

		printf("Se ha producido un error\n");
		return -1;

	}
    
	return 0;
}
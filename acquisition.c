#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"


int main(int argc, char **argv) {

	int fd0[2];
	int fd1[2];
	int fd2[2];
	int fd3[2];

	if (pipe(fd0) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

	pid_t pid;
	pid_t pid2;
	char* mes_lu_auto;
	char* mes_lu_term;

	printf("acq: je vais creer un fils terminal\n");
	pid = fork();

//le fils - terminal
	if (pid == 0){
		printf("- je suis le fils terminal\n");

		close(fd0[1]);
		close(fd1[0]);

		char s_fd0[50], s_fd1[50];

		sprintf(s_fd0, "%d", fd0[0]);
		sprintf(s_fd1, "%d", fd1[1]);

		execl("./terminal","./terminal", s_fd0, s_fd1, NULL);
		printf("j'ai exec terminal.c\n");
	}
	else if(pid < 0){	//erreur
		perror("erreur fork1");
		exit(EXIT_FAILURE);
	}
	printf("acq: je vais creer un fils autorisation\n");
	pid2 = fork();


//fils - autorisation
	if(pid2 == 0){
		printf("- je suis le fils autorisation\n");

		close(fd2[1]);
		close(fd3[0]);
		
		char s_fd2[50], s_fd3[50];

		sprintf(s_fd2, "%d", fd2[0]);
		sprintf(s_fd3, "%d", fd3[1]);

		execl("./autorisation","./autorisation", s_fd2, s_fd3, NULL);
		printf("j'ai exec autorisation.c\n");
	}
	else if (pid2 < 0){
		perror("erreur fork2");
		exit(EXIT_FAILURE);
	}

//pere
	close(fd0[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd3[1]);

	//lit pipe sortant du terminal
	mes_lu_term = litLigne(fd1[0]);
	printf("acq: a lu mes_term\n");
	printf("acq: %s\n", mes_lu_term);

	//ecrit dans le pipe vers autorisation
	ecritLigne(fd2[1], mes_lu_term);
	
	//lit pipe sortant de autorisation
	mes_lu_auto = litLigne(fd3[0]);
	printf("acq: a lu mes_auto\n");
	printf("acq: %s\n", mes_lu_auto);
	//ecrit dans le pipe vers terminal
	ecritLigne(fd0[1], mes_lu_auto);
	

	return 0;
}
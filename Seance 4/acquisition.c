#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"


int main(int argc, char **argv) {

	// int fd0[2];
	// int fd1[2];
	int fd2[2];
	int fd3[2];

	// if (pipe(fd0) == -1) {
 //        perror("pipe");
 //        exit(EXIT_FAILURE);
 //    }
 //    if (pipe(fd1) == -1) {
 //        perror("pipe");
 //        exit(EXIT_FAILURE);
 //    }
    if (pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }


	// creation de pipe temporaire pour stackage dans tableau de pipe
	int fd0[2];
	int fd1[2];
	// creation du tableau de stockage de pipe
	int nb_term = 3;
	int tab[nb_term][4];

	for (int i =0; i < nb_term; i++) {

		// initialisation
		if (pipe(fd0) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		if (pipe(fd1) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		// mise en tableau
		tab[i][0] = fd0[0];
		tab[i][1] = fd0[1];
		tab[i][2] = fd1[0];
		tab[i][3] = fd1[1];

	}

	pid_t pid;
	pid_t pid2;
	char* mes_lu_auto;
	char* mes_lu_term;

	printf("acq: je vais creer un fils terminal\n");

// Creation des processus, tubes et recouvrement

//le fils - terminal
	for (int i = 0; i < nb_term; i++) {	
		pid = fork();
		
		if (pid == 0){
			printf("- je suis le fils terminal%d\n",i);

			// tab[i][0] = fd0[0];
			// tab[i][1] = fd0[1];
			// tab[i][2] = fd1[0];
			// tab[i][3] = fd1[1];

			close(tab[i][1]);
			close(tab[i][2]);

			char s_fd0[50], s_fd1[50];

			sprintf(s_fd0, "%d", tab[i][0]);
			sprintf(s_fd1, "%d", tab[i][3]);

			execl("/usr/bin/xterm","/usr/bin/xterm","-hold","-e","./terminal", s_fd0, s_fd1, NULL);
			// printf("j'ai exec terminal.c\n");
		}
		else if(pid < 0){	//erreur
			perror("erreur fork1");
			exit(EXIT_FAILURE);
		}
	}
// 

	printf("acq: je vais creer un fils autorisation\n");


//fils - autorisation
	pid2 = fork();

	if(pid2 == 0){
		printf("- je suis le fils autorisation\n");

		close(fd2[1]);
		close(fd3[0]);
		
		char s_fd2[50], s_fd3[50];

		sprintf(s_fd2, "%d", fd2[0]);
		sprintf(s_fd3, "%d", fd3[1]);

		execl("/usr/bin/xterm","/usr/bin/xterm","-hold","-e","./autorisation", s_fd2, s_fd3, NULL);
		// printf("j'ai exec autorisation.c\n");
	}
	else if (pid2 < 0){
		perror("erreur fork2");
		exit(EXIT_FAILURE);
	}
// 

// Lecture ecriture des messages

//pere
	close(fd0[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd3[1]);

	//lit pipe sortant du terminal
	while(1) {
		for (int i = 0; i < nb_term; i++) {	

			// tab[i][0] = fd0[0];
			// tab[i][1] = fd0[1];
			// tab[i][2] = fd1[0];
			// tab[i][3] = fd1[1];

			mes_lu_term = litLigne(tab[i][2]);
			// printf("acq: a lu mes_term\n");
			// printf("acq: %s\n", mes_lu_term);
			printf("transmet la demande\n");

			//ecrit dans le pipe vers autorisation
			ecritLigne(fd2[1], mes_lu_term);
			
			//lit pipe sortant de autorisation
			mes_lu_auto = litLigne(fd3[0]);
			// printf("acq: a lu mes_auto\n");
			// printf("acq: %s\n", mes_lu_auto);
			printf("transmet l'autorisation\n");
			//ecrit dans le pipe vers terminal
			ecritLigne(tab[i][1], mes_lu_auto);
		}
	}

	return 0;
}
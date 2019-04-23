#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lectureEcriture.h"
#include "message.h"
#include <semaphore.h>

int nb_term = 3;
int fd2[2];
int fd3[2];
sem_t semaphore;
sem_t semaphore2;
sem_t semaphore3;
int part = 0;

// ==================STRUCT==================
typedef struct args_threads {
		int fd_entree_auto;
		int fd_sortie_auto;
		int *fd_entree_term;
		int fd_sortie_term;
		char** tab_cb_struct;
		int* tab_pipe_struct;
}args_threads;
//

// ==================FONCTIONS==================
void *threading_term(void *input) {
	char* mes_lu_term;
	int thread_part = part++;
	printf("threading_term[%ld]: thread_part:%d\n", pthread_self(), thread_part);

	printf("threading_term[%ld]: je suis un thread\n", pthread_self());
	//lit pipe sortant du terminal
	printf("threading_term[%ld]: Rempli le tableau de cb et de pipe:\n", pthread_self());

	((struct args_threads*)input)->tab_pipe_struct[thread_part] = ((struct args_threads*)input)->fd_entree_term[thread_part];


	printf("tab_pipe_struct rempli avec: %d\n", ((struct args_threads*)input)->tab_pipe_struct[thread_part]);

	char cb[255], type[255], valeur[255];
	
	while(1) {

		printf("threading_term[%ld]: rentre dans le while de threading_term\n", pthread_self());
		mes_lu_term = litLigne(((struct args_threads*)input)->fd_sortie_term);
		printf("threading_term[%ld]: mes_lu_term[%s]\n",pthread_self(), mes_lu_term);

		decoupe(mes_lu_term, cb, type, valeur);
		((struct args_threads*)input)->tab_cb_struct[thread_part] = cb;

		printf("threading_term[%ld]: transmet la demande\n", pthread_self());
		//ecrit dans le pipe vers autorisation
		sem_wait(&semaphore3);
		ecritLigne(((struct args_threads*)input)->fd_entree_auto, mes_lu_term);
		sem_post(&semaphore3);
	}
	return 0;
}

void *threading_glob(void *input) {
	char* mes_lu_auto;
	
	printf("threading_glob: je suis un thread\n");

	//lit pipe sortant de autorisation
	while(1) {

		printf("rentre dans le while de threading_glob\n");
		mes_lu_auto = litLigne(((struct args_threads*)input)->fd_sortie_auto);
		printf("threading_glob: mes_lu_auto[%s]\n", mes_lu_auto);

		char cb[255], type[255], valeur[255];
		char* cb_;

		decoupe(mes_lu_auto, cb, type, valeur);
		cb_ = cb;
		printf("cb_ : %s\n", cb_);
		for (int i =0; i < nb_term; i++){

			printf("/!|############## threading_glob: rentre dans le for\n");
			printf("cb: %s\n", cb);
			char* cb_in_tab = ((struct args_threads*)input)->tab_cb_struct[i];
			printf("cb_in_tab: %s\n", cb_in_tab);

			int fd_term_tab_pipe = ((struct args_threads*)input)->tab_pipe_struct[i];
			printf("fd_term_tab_pipe: %d\n", fd_term_tab_pipe);
			
			if (cb == cb_in_tab) {
			// if (strcmp(*cb_in_tab, cb_) != NULL){

				printf("threading_glob: envoie de la reponse au pipe: %d\n", fd_term_tab_pipe);
				ecritLigne(fd_term_tab_pipe, mes_lu_auto);
			}
			else{
				printf("echec de la condition 'cb == cb_in_tab'\n");
			}
		}
	}
	return 0;
}
// 

// ==================MAIN==================
int main(int argc, char **argv) {

	// ==================CREATION DE PIPES==================
		printf("\n");
		printf("==================CREATION DE PIPES==================\n");
		printf("main: rentre dans la creation de pipes\n");
		printf("\n");

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
		printf("la creation des pipes est terminée\n");
	// 

	// ==================CREATION DES PROCESSUS, TUBES ET RECOUVREMENT==================
		printf("\n");
		printf("======CREATION DES PROCESSUS, TUBES ET RECOUVREMENT======\n");
		printf("main: rentre dans la creation de processus\n");
		printf("\n");

		pid_t pid;
		pid_t pid2;

		printf("acq: je vais creer un fils terminal\n");
		//le fils - terminal========================================
		for (int i = 0; i < nb_term; i++) {	
			pid = fork();
			
			if (pid == 0){
				printf("- je suis le fils terminal%d\n",i);
				printf("\n");

				close(tab[i][1]);
				close(tab[i][2]);

				char s_fd0[50], s_fd1[50];

				sprintf(s_fd0, "%d", tab[i][0]);
				sprintf(s_fd1, "%d", tab[i][3]);

				execl("/usr/bin/xterm","/usr/bin/xterm","-hold","-e","./terminal", s_fd0, s_fd1, NULL);
			}
			else if(pid < 0){	//erreur
				perror("erreur fork1");
				exit(EXIT_FAILURE);
			}
		}

		printf("acq: je vais creer un fils autorisation\n");
		//fils - autorisation=======================================
		pid2 = fork();

		if(pid2 == 0){
			printf("- je suis le fils autorisation\n");
			printf("\n");

			close(fd2[1]);
			close(fd3[0]);
			
			char s_fd2[50], s_fd3[50];

			sprintf(s_fd2, "%d", fd2[0]);
			sprintf(s_fd3, "%d", fd3[1]);

			execl("/usr/bin/xterm","/usr/bin/xterm","-hold","-e","./autorisation", s_fd2, s_fd3, NULL);
		}
		else if (pid2 < 0){ //erreur
			perror("erreur fork2");
			exit(EXIT_FAILURE);
		}
	//  

	// ==================CREATION DE THREADS==================
		printf("\n");
		printf("==================<CREATION DE THREADS>==================\n");
		printf("main: rentre dans la creation de threads\n");
		printf("\n");

		pthread_t tid;
		pthread_t tab_threads[nb_term];

		char **tab_cb = malloc(sizeof(char[255])*nb_term);
		int *tab_pipe = malloc(sizeof(int)*nb_term);

		struct args_threads *struct_thread = (args_threads *)malloc(sizeof(args_threads));
		struct_thread->fd_entree_term = malloc(sizeof(int)*nb_term);

		printf("main[%d]\n", getpid());
		sem_init(&semaphore, 0, 1);
		sem_init(&semaphore2, 0, 1);
		sem_init(&semaphore3, 0, 1);

		for (int i =0; i < nb_term; i++) {//CREATION DES THREADS_TERM

			// sem_wait(&semaphore);
			printf("main: rempli les struct avec ses params\n");
			printf("etape:%d; %d,%d,%d\n",i, fd2[1], tab[i][1], tab[i][2]);
			printf("\n");
			struct_thread->fd_entree_auto = fd2[1];//auto <---o
			struct_thread->fd_sortie_auto = fd3[0];//auto --->o
			struct_thread->fd_entree_term[i] = tab[i][1];//term <---o
			// struct_thread->fd_entree_term[i] = 2;
			struct_thread->fd_sortie_term = tab[i][2];//term --->o
			struct_thread->tab_cb_struct = tab_cb;
			struct_thread->tab_pipe_struct = tab_pipe;


			if (pthread_create(&tid, NULL, threading_term, (void *)struct_thread) != 0)
				return -1;
			printf("main: creation du thread_term[%ld] avec succes \n", tid);
			tab_threads[i] = tid;
			// sem_post(&semaphore);
		}

		if (pthread_create(&tid, NULL, threading_glob, (void *)struct_thread) != 0)
				return -1;
			printf("main: creation du thread_glob[%ld] avec succes \n", tid);

		
		for (int i =0; i < nb_term; i++) {
			//attendre fin des threads
			int i = pthread_join (tab_threads[i], NULL);
		}
		pthread_join (tid, NULL);

		printf("main: fin des threads\n");
	//

	return 0;
}
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include <ctype.h>
#include "message.h"

int main(int argc, char **argv) {
	

	
	printf("auto: autorisation.c est execute\n");
	int reception;
	int envoie;
	char *w_lower = "demande";
	char *w_upper = "Demande";

/* no de carte concernee, Type du message, Valeur associée au message */
    char cb[255], type[255], valeur[255];


	if (argc >= 2) {
		reception = atoi(argv[1]);	//entree
		envoie = atoi(argv[2]);		//sortie
	}
	else{
		perror("auto: parametre d'entree et sortie a preciser\n");
		exit(0);
	}
	int recu = 0;
	printf("auto: JE SUIS EN TRAIN D'ATTENDRE\n");
	char* entree = litLigne(reception);
	printf("auto: %s\n", entree);

	if (entree == NULL) {
		perror("auto: TestLectureEcriture - litLigne\n");
		exit(0);
	} 

	else {
		// printf("Ca ne devrait pas arriver\n");
		//https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
		
		if (strstr(entree, w_lower) != NULL || strstr(entree, w_upper) != NULL){
			decoupe(entree,cb, type, valeur);
			char* c = message(cb, "Reponse", "OK");
			ecritLigne(envoie, c);
			printf("auto: %s\n", c);
		}
		else {
			perror("auto: ceci n'est pas une demande: que voulez-vous faire\n");
			exit(0);
		}
		
	}

	return 0;
}
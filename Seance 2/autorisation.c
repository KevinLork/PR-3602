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

	printf("%s\n", "autorisation.c est execute");
	int reception;
	int envoie;
	char *w_lower = "demande";
	char *w_upper = "Demande";

/* no de carte concernee, Type du message, Valeur associée au message */
    char cb[255], type[255], valeur[255];


	if (argc >= 2) {
		reception = atoi(argv[0]);	//entree
		envoie = atoi(argv[1]);		//sortie
	}
	else{
		perror("parametre d'entree et sortie a preciser");
		exit(0);
	}
	int recu = 0;
	char* entree = litLigne(reception);

	if (entree == NULL) {
		perror("TestLectureEcriture - litLigne");
		exit(0);
	} 

	else {
		// printf("Ca ne devrait pas arriver\n");
		//https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
		
		if (strstr(entree, w_lower) != NULL || strstr(entree, w_upper) != NULL){
			decoupe(entree,cb, type, valeur);
			char* c = message(cb, "Reponse", "OK");
			ecritLigne(envoie, c);
		}
		else {
			perror("ceci n'est pas une demande: que voulez vous faire");
			exit(0);
		}
		
	}

return 0;
}
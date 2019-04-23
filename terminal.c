#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include "message.h"


int main(int argc, char **argv) {

// entry-fichier vers terminal
	// int entry = 0;
	// int sortie = 1;
	printf("term: terminal.c est execute\n");
	int entry;
	int sortie;
	if (argc >= 2) {

		entry = atoi(argv[1]);	//entry
		sortie = atoi(argv[2]);		//sortie
		//https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
	}
	else{
		perror("term: parametres d'entry et sortie a preciser\n");
		exit(0);
	}

	int recu = 0;
	char cb[255], type[255], valeur[255];
 	int decoupeOk;
	//               |0001000000000000|Demande|400|

	// char* entry = litLigne(entry);
	
	char *Demande1 = message("0001000000000000", "Demande", "100");
	char *Demande2 = message("0002000000000000", "Demande", "200");
	char *Demande3 = message("0003000000000000", "Demande", "300");
	// printf("Demande de paiement de %s euros\n",valeur);

	int i =0;
	char* tableau[] = {Demande1, Demande2, Demande3};


	for (i = 0; i < 3; i ++){
		decoupeOk = decoupe(tableau[i], cb, type, valeur);
	    if (!decoupeOk){
			perror("term: Demande inconnue\n");
			exit(0);
		}
		else {
				ecritLigne(sortie, tableau[i]);		
				printf("term: Demande envoyee\n");
		}
		//|0001000000000000|reponse|ok|
		// char* reponse = message("0001000000000000", "Reponse", valeur);
		

		char* reponse = litLigne(entry);
		decoupeOk = decoupe(reponse, cb, type, valeur);
	    if (!decoupeOk) {
			perror("term: Réponse invalide\n");
			exit(0);
		}
		else {
				printf("term: Réponse recue \n");
				// ecritLigne(sortie,reponse);		
			}
	}

	return 0;
}	
	

// terminal vers sortie-vers aquisition
// aquisition vers terminal
// terminal vers ecran

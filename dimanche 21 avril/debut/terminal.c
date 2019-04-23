#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include "message.h"
#include "annuaire.h"


int main(int argc, char **argv) {

	AnnuaireClients *an;
	// int entree = 0;
	// int sortie = 1;
	printf("term: terminal.c est execute\n");
	int entree;
	int sortie;
	if (argc >= 2) {

		entree = atoi(argv[1]);	//entree
		sortie = atoi(argv[2]);		//sortie
		//https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
	}
	else{
		perror("term: parametres d'entree et sortie a preciser\n");
		exit(0);
	}

	int recu = 0;
	char cb[255], type[255], valeur[255];
 	int decoupeOk;


// ===============================ANNUAIRE==========================================
	an = annuaire("annuaire.an");
	if(an == NULL){
		fprintf(stderr,"%s ne peut lire l'annuaire depuis le fichier%s\n", argv[0], "Annu");
		exit(0);
	}
// ===============================ANNUAIRE==========================================
	
	char * valeur_;
	valeur_ = malloc(sizeof(char) * TAILLEBUF);

	// for(i = 0; i < an->nbClients ; i++){
	for(int i = 0; i < 3 ; i++){
		// sprintf(ligne,"%s %s %d\n", an->donnees[i].CB, an->donnees[i].banque, an->donnees[i].solde);

		sprintf (valeur_ , "%d\n", (an->donnees[i].solde-100));
		char* Demande = message(an->donnees[i].CB, "Demande", valeur_);
		
		decoupeOk = decoupe(Demande, cb, type, valeur);
	    if (!decoupeOk){
			perror("term: Demande inconnue\n");
			exit(0);
		}
		else {
				ecritLigne(sortie, Demande);		
				printf("term: Demande envoyee\n");
		}


	}
		
	while(1){

		char* reponse = litLigne(entree);
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
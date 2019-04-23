#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include <ctype.h>
#include "message.h"
#include "annuaire.h"

int main(int argc, char **argv) {
	
	printf("auto: autorisation.c est execute\n");
	AnnuaireClients *an;
	Client *cl;

// ===============================ANNUAIRE==========================================
	an = annuaire("annuaire.an");
	if(an == NULL){
		fprintf(stderr,"%s ne peut lire l'annuaire depuis le fichier%s\n", argv[0], "Annu");
		exit(0);
	}
// ===============================ANNUAIRE==========================================


	while(1){
		
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
		// printf("auto: JE SUIS EN TRAIN D'ATTENDRE\n");
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
				decoupe(entree, cb, type, valeur);
				// Rechercher le client dont la carte est 0004564593939009
				cl = client(an,cb);

				// si ce client est dans l'annuaire
				if (cl != NULL){
					// On affiche ses informations
					printf("Le numero de carte du client recherche est %s\n", cl->CB);
					printf("Sa banque est %s\n", cl->banque);
					printf("Le solde de son compte est %d\n", cl->solde);
					
					char* c = message(cb, "Reponse", "1");
					ecritLigne(envoie, c);
					printf("auto: %s\n", c);
				}
				else {
				printf("Le client %s n'est pas répertorie dans l'annuaire\n", cb);
					char* c = message(cb, "Reponse", "0");
					ecritLigne(envoie, c);
					printf("auto: %s\n", c);
				}



			}
			else {
				perror("auto: ceci n'est pas une demande: que voulez-vous faire\n");
				exit(0);
			}
			
		}
	}

	return 0;
}
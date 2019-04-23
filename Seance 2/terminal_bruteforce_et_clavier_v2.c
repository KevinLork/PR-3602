/*#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include "message.h"
#include "alea.h"




int main(int argc, char **argv) {

	int clavier = 0;
	int fdmessage = 1;
	char emeteur[255], type[255], valeur[255];
 	int decoupeOk;
	aleainit();
	sprintf(valeur,"%d",alea(1,50000));
	char *msg = message("0001000000000000", "Demande", valeur);
	// Creation d'une demande de paiement pour un montant aleatoire

	printf("Demande de paiement de %s euros\n",valeur);

	decoupeOk = decoupe(msg, emeteur, type, valeur);
    if (!decoupeOk) 
	{
		perror("Demande inconnue");
		exit(0);
	}
	else
		{
			printf("Demande recue \n");
			ecritLigne(fdmessage,msg);		
		}
	
	char* reponse = message("0001000000000000", "Reponse", valeur);
	decoupeOk = decoupe(msg, emeteur, type, valeur);
    if (!decoupeOk) 
	{
		perror("Réponse invalide");
		exit(0);
	}
	else
		{
			printf("Réponse donnée \n");
			ecritLigne(fdmessage,reponse);		
		}
	return 0;
}	*/


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include "alea.h"
#include "message.h"


int main(int argc, char **argv) {
// clavier-fichier vers terminal
	// int clavier = 0;
	// int shell = 1;
	int clavier;
	int shell;
	if (argc >= 2) {

		clavier = atoi(argv[0]);	//entree
		shell = atoi(argv[1]);		//sortie
		//https://stackoverflow.com/questions/2797813/how-to-convert-a-command-line-argument-to-int
	}
	else{
		perror("parametre d'entree et sortie a preciser");
		exit(0);
	}

	int recu = 0;
	char cb[255], type[255], valeur[255];
 	int decoupeOk;
	//               |0001000000000000|Demande|400|
	aleainit();
	sprintf(valeur,"%d",alea(1,50000));

	char* entree = litLigne(clavier);
	
	// char *entree = message("0001000000000000", "Demande", valeur);
	// Creation d'une demande de paiement pour un montant aleatoire

	printf("Demande de paiement de %s euros\n",valeur);

	decoupeOk = decoupe(entree, cb, type, valeur);
    if (!decoupeOk) 
	{
		perror("Demande inconnue");
		exit(0);
	}
	else
		{
			printf("Demande envoyee \n");
			ecritLigne(shell,entree);		
		}
	//|0001000000000000|reponse|ok|
	// char* reponse = message("0001000000000000", "Reponse", valeur);
	char* reponse = litLigne(clavier);
	decoupeOk = decoupe(reponse, cb, type, valeur);
    if (!decoupeOk) 
	{
		perror("Réponse invalide");
		exit(0);
	}
	else
		{
			printf("Réponse donnée \n");
			ecritLigne(shell,reponse);		
		}
	return 0;
}	
	

// terminal vers shell-vers aquisition
// aquisition vers terminal
// terminal vers ecran

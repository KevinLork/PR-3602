#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lectureEcriture.h"
#include "message.h"



int main(int argc, char **argv) {

	int clavier = 0;
	int fdmessage = 1;
	char* demande = message(1111222233334444,"Demande",400);

	if (Decoupe(demande)= 0)
	{
		perror("Demande inconnue");
		exit(0);
	}
	else
		{
			printf("Demande recue \n");
			ecritLigne(fdmessage,demande);		
		}
	
	char* reponse = litLigne(clavier);
	if (reponse == NULL)
	{
		perror("Aucune réponse");
		exit(0);
	}
	else
		{
			printf("Réponse donnée \n");
			ecritLigne(fdmessage,reponse);		
		}
	return 0;
}	




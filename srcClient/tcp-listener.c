#include "../include/Api.h"

int main(int argc,char *argv[]){
	TCPConnexion interface;
	int tailleMessage;
	Configuration config;
	char *msg,*msgCopy;
	
	//Chargement de la configuration
	if(argc != 2){
		perror("Erreur utilisation du programme -> passez en argument le chemin du fichier de configuration");
		exit(EXIT_FAILURE);
	}
	
	config = ParseConfig(GetConfigDoc(argv[1]));
		
	//Initialisation de la connexion UDP
	msg = calloc(1,config.tailleMessage*sizeof(char));
	if(msg == NULL){
		perror("Erreur allocation mémoire pour les messages \n");
		exit(EXIT_FAILURE);

	}
	
	tailleMessage = config.tailleMessage;
	interface = ConnexionServer(config);
	FreeConfiguration(&config);
	
	/*Debut des messages*/
	printf("Test -> Entrez un message de moins de %d caractères : ",tailleMessage);
	fgets(msg,tailleMessage+1,stdin); 
	
	while (strcmp(msg,".")) {
		if (strlen(msg) > 0) {
			// Envoi de la ligne au serveur
			if (!EnvoiMessageServer(interface,msg)) {
				perror("Erreur lors de l'envoi du message \n");
				free(msg);
				FermetureConnexion(interface,EXIT_FAILURE);
			}

			//Traitement du message
			if (AttenteMessage(interface)) {
				// Lecture de la ligne modifiée par le serveur.
				memset(msg, 0, sizeof(msg)); // Mise à zéro du tampon
				if (!ReceptionMessageServer(interface,msg)) {
					perror("Erreur lors de la réception du message \n");
					free(msg);
					FermetureConnexion(interface,EXIT_FAILURE);
				}
				printf("Message traité : %s\n", msg);
			}
			
			else 
				printf("Pas de réponse dans la seconde. \n");			
		}
		//Nouveau message
		printf("Entrez un message de moins de %d caractères : ",tailleMessage);
		fgets(msg,tailleMessage+1,stdin);
	}
	//Fin connexion
	free(msg);
	FermetureConnexion(interface,EXIT_SUCCESS);
}

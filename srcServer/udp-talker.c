#include "../include/Api.h"

int main()
{
	UDPConnexion interface;
	int port;
	char msg[MAX_MSG + 1],ipv4[IPv4_LENGTH];

	/*Paramètres*/
	printf("Entrez le nom du serveur ou son adresse IP : ");
	fgets(ipv4,IPv4_LENGTH,stdin);

	printf("Test -> Entrez un message de moins de %d caractères : ",MAX_MSG);
	fgets(msg,MAX_MSG+1,stdin);

	//Initialisation de la connexion UDP
	interface = ConnexionServer(ipv4,8080,MAX_MSG+1);

	// Arrêt lorsque l'utilisateur saisit une ligne ne contenant qu'un point
	while (strcmp(msg, ".")) {
		if (strlen(msg) > 0) {
			// Envoi de la ligne au serveur
			if (!EnvoiMessageServer(interface,msg)) {
				perror("Erreur lors de l'envoi du message \n");
				FermetureConnexion(interface,EXIT_FAILURE);
			}

			//Traitement du message
			if (AttenteMessage(interface)) {
				// Lecture de la ligne modifiée par le serveur.
				memset(msg, 0, sizeof msg); // Mise à zéro du tampon
				if (!ReceptionMessageServer(interface,msg)) {
					perror("Erreur lors de la réception du message \n");
					FermetureConnexion(interface,EXIT_FAILURE);
				}
				printf("Message traité : %s\n", msg);
			}
			else 
				printf("Pas de réponse dans la seconde. \n");			
		}
		//Nouveau message
		printf("Entrez un message de moins de %d caractères : ",MAX_MSG);
		fgets(msg,MAX_MSG+1,stdin);
	}
	//Fin connexion
	FermetureConnexion(interface,EXIT_SUCCESS);
}

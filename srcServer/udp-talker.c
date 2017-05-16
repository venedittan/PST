#include "../include/Api.h"

int main()
{
	UDPConnexion interface;
	int i;
	struct sockaddr_in adresseClient;

	char msg[MAX_MSG + 1];
	
	/*Initialisation de la connexion UDP*/
	interface = ConnexionClient(8080,MAX_MSG + 1);

	// Libération de la mémoire occupée par les enregistrements
	printf("Attente de requête sur le port %s\n",interface.port);

	while (1) {
		// Mise à zéro du tampon de façon à connaître le délimiteur
		// de fin de chaîne.
		memset(msg, 0, sizeof msg);

		if (!ReceptionMessageClient(interface,&adresseClient,msg)) {
			perror("Erreur lors de la réception du message");
			FermetureConnexion(interface,EXIT_FAILURE);
		}
		
		if (strlen(msg) > 0) {
			// Affichage de l'adresse IP du client.
			printf(">> depuis %s", inet_ntoa(adresseClient.sin_addr));

			// Affichage du numéro de port du client.
			printf(":%hu\n", ntohs(adresseClient.sin_port));
			
			// Affichage de la ligne reçue
			printf(" Message reçu : %s\n", msg);
			
			// Conversion de cette ligne en majuscules.
			for (i = 0; i < strlen(msg); i++)
				msg[i] = toupper(msg[i]);

			// Renvoi de la ligne convertie au client.
			if (!EnvoiMessageClient(interface,&adresseClient,msg)) {
				perror("Erreur lors de l'envoi du message");
				FermetureConnexion(interface,EXIT_FAILURE);
			}
		}
	}
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_MSG 80
#define MAX_PORT 5
#define IPv4_LENGTH 35 //32 caractères plus les 3 pts

/*Structure : Interface pour la réception des messages*/
typedef struct UDPConnexion{
	char port[MAX_PORT + 1];
	int statut;
	unsigned int longueurAdresseClient;
	int lienConnexion;
	struct sockaddr_in adresseClient;
	unsigned short int tailleMessage;
	struct addrinfo hints;
	struct addrinfo *infoServeur;
	char ipv4[IPv4_LENGTH];
}UDPConnexion;



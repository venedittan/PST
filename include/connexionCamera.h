#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*Structure : Interface pour la réception des messages*/
typedef struct UDPConnexion{
	char *port;
	int statut;
	unsigned int longueurAdresseClient;
	int lienConnexion;
	struct sockaddr_in adresseClient;
	unsigned short int tailleMessage;
	struct addrinfo hints;
	struct addrinfo *infoServeur;
	char *ipv4;
}UDPConnexion;



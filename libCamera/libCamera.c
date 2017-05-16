#include "../include/libCamera.h"

/*Fonction de gestion pour l'erreur*/
const char* ErreurBind(int erreur){
	switch(erreur){
		case EACCES : 
			return "L'adresse est protégée et l'utilisateur n'est pas le superutilisateur";

		case EADDRINUSE : 
			return "L'adresse fournie est déja utilisée";

		case EBADF : 
			return "sockfd n'est pas un descripteur valide";

		case ENOTSOCK : 
			return "sockd est un descripteur de fichier , pas d'un socket";

		case EADDRNOTAVAIL : 
			return "Une interface inexistance a été demandée ou bien l'adresse demandée n'est pas locale";

		case EFAULT : 
			return "addr pointe en dehors de l'espace d'adresse accessible";

		case EINVAL : 
			return "La longueur addrlen est fausse , ou la socket n'est pas de la famille AF_UNIX";

		case ELOOP : 
			return "addr contient des références circulaires";

		default:
			return "Erreur Autre";
	}
}

/*Fonction de connexion*/
UDPConnexion ConnexionServer(char* ipv4,int port,unsigned short int tailleMessage){

	/*Nouvelle connexion*/
	UDPConnexion interface;
	char portStr[MAX_PORT + 1];

	/*Initialisation de la nouvelle connexion*/
	strcpy(interface.ipv4,ipv4);
	memset(&(interface.hints), 0, sizeof(interface.hints));
	interface.tailleMessage = tailleMessage;
	interface.hints.ai_family = AF_INET; // IPv4
	interface.hints.ai_socktype = SOCK_DGRAM; // UDP

	interface.longueurAdresseClient = sizeof(struct sockaddr_in);
	interface.lienConnexion = 0;
	
	interface.infoServeur = calloc(1,sizeof(struct addrinfo));

	/*Vérification du port d'écoute*/
	if(port >= 1500 && port <= 65000){
		sprintf(portStr,"%d",port);
		memset(interface.port,0,sizeof(interface.port));
		strcpy(interface.port,portStr);
	}

	else{
		perror("Le port d'écoute n'est pas compris entre 1500 et 65000");
		exit(EXIT_FAILURE);
	}

	/*Vérification du statut*/
	interface.statut = getaddrinfo(interface.ipv4,interface.port,&(interface.hints),&(interface.infoServeur));
	if(interface.statut){
		fprintf(stderr,"Erreur de connexion sur le port %s -> statut : %s \n",interface.port,gai_strerror(interface.statut));
		exit(EXIT_FAILURE);
	}

	/*On se crée la connexion*/
	interface.lienConnexion = socket(interface.infoServeur->ai_family,interface.infoServeur->ai_socktype,interface.infoServeur->ai_protocol);
	if(interface.lienConnexion == -1){
		fprintf(stderr,"Erreur lors de la création de la connexion au port %s \n",interface.port);
		exit(EXIT_FAILURE);
	}

	/*On lie la connexion*/
	if(connect(interface.lienConnexion,interface.infoServeur->ai_addr,interface.infoServeur->ai_addrlen) == -1){
		fprintf(stderr,"Erreur lors de la liaison au port %s -> Erreur : %s \n",interface.port,ErreurBind(errno));
		exit(EXIT_FAILURE);
	}
	
	/*Renvoi de la connexion*/
	return interface; 
}

/*Fonction de connexion*/
UDPConnexion ConnexionClient(int port,unsigned short int tailleMessage){

	/*Nouvelle connexion*/
	UDPConnexion interface;
	char portStr[MAX_PORT + 1];

	/*Initialisation de la nouvelle connexion*/
	memset(&(interface.hints), 0, sizeof(interface.hints));
	interface.hints.ai_family = AF_INET; // IPv4
	interface.hints.ai_socktype = SOCK_DGRAM; // UDP
	interface.hints.ai_flags = AI_PASSIVE; // Toutes les adresses disponibles
	interface.longueurAdresseClient = sizeof(struct sockaddr_in);
	interface.lienConnexion = 0;
	interface.infoServeur = NULL;
	interface.tailleMessage = tailleMessage;

	/*Vérification du port d'écoute*/
	if(port >= 1500 && port <= 65000){
		sprintf(portStr,"%d",port);
		strcpy(interface.port,portStr);
	}
	else{
		perror("Le port d'écoute n'est pas compris entre 1500 et 65000");
		exit(EXIT_FAILURE);
	}

	/*Vérification du statut*/
	interface.statut = getaddrinfo(NULL,interface.port,&(interface.hints),&(interface.infoServeur));
	if(interface.statut){
		fprintf(stderr,"Erreur de connexion sur le port %s -> statut : %s \n",interface.port,gai_strerror(interface.statut));
		exit(EXIT_FAILURE);
	}

	/*On se crée la connexion*/
	interface.lienConnexion = socket(interface.infoServeur->ai_family,interface.infoServeur->ai_socktype,interface.infoServeur->ai_protocol);
	if(interface.lienConnexion == -1){
		fprintf(stderr,"Erreur lors de la création de la connexion au port %s \n",interface.port);
		exit(EXIT_FAILURE);
	}

	/*On lie la connexion*/
	if(bind(interface.lienConnexion,interface.infoServeur->ai_addr,interface.infoServeur->ai_addrlen) == -1){
		close(interface.lienConnexion);
		fprintf(stderr,"Erreur lors de la liaison au port %s -> Erreur : %s \n",interface.port,ErreurBind(errno));
		exit(EXIT_FAILURE);
	}

	/*Libération des informations complémentaires*/
	freeaddrinfo(interface.infoServeur);

	/*Renvoi de la connexion*/
	return interface; 
}

/*Reception Message Client*/
int ReceptionMessageClient(UDPConnexion interface,struct sockaddr_in* adresseClient,char *msg){
	return recvfrom(interface.lienConnexion,msg,sizeof(msg),0,(struct sockaddr *) adresseClient,&(interface.longueurAdresseClient)) != -1;
}

/*Envoi Message Client*/
int EnvoiMessageClient(UDPConnexion interface,struct sockaddr_in* adresseClient,char *msg){
	return sendto(interface.lienConnexion,msg,sizeof(msg),0,(struct sockaddr *) adresseClient,interface.longueurAdresseClient) != -1;
}

/*Reception Message Serveur*/
int ReceptionMessageServer(UDPConnexion interface,char *msg){
	return recv(interface.lienConnexion,msg,sizeof(msg),0) != -1;
}

/*Envoi Message Serveur*/
int EnvoiMessageServer(UDPConnexion interface,char *msg){
	return sendto(interface.lienConnexion,msg,sizeof(msg),0,interface.infoServeur->ai_addr,interface.infoServeur->ai_addrlen) != -1;
}

/*Attente Message 1s*/
int AttenteMessage(UDPConnexion interface){
	struct timeval timeVal;
	fd_set readSet;

	// Attente de la réponse pendant une seconde.
	FD_ZERO(&readSet);
	FD_SET(interface.lienConnexion, &readSet);
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	return select(interface.lienConnexion+1, &readSet, NULL, NULL, &timeVal);
}

/*Fonction de fermeture de connexion*/
void FermetureConnexion(UDPConnexion interface,int Erreur){
	close(interface.lienConnexion);
	exit((Erreur == EXIT_FAILURE) ? EXIT_FAILURE : EXIT_SUCCESS);
}




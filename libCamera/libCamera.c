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
			return "l'addresse pointe en dehors de l'espace d'adresse accessible";

		case EINVAL : 
			return "La longueur addrlen est fausse , ou la socket n'est pas de la famille AF_UNIX";

		case ELOOP : 
			return "l'addresse contient des références circulaires";

        case ENAMETOOLONG : 
            return "Adresse trop longue";
        
        case ENOMEM : 
            return "Espace mémoire RAM insuffisant";
        	
        case EROFS : 
            return "Le socket est un répertoire ou fichier en lecture seule";

		default:
			return "Erreur Autre";
	}
}

/*Fonction de connexion*/
TCPConnexion ConnexionServer(Configuration config){

	/*Nouvelle connexion*/
	TCPConnexion interface;
	char *portStr;
	
	/*Initialisation de la nouvelle connexion*/
	memset(&(interface.hints), 0, sizeof(interface.hints));
	interface.lienConnexion = 0;
	interface.tailleMessage = config.tailleMessage;
	interface.hints.ai_family = AF_INET; // IPv4
	interface.hints.ai_socktype = SOCK_DGRAM; // TCP
	interface.longueurAdresseClient = sizeof(struct sockaddr_in);
	interface.infoServeur = calloc(1,sizeof(struct addrinfo));
	interface.ipv4 = calloc(1,config.tailleAdresse);
	interface.port = calloc(1,config.taillePort+1);
	portStr = calloc(1,config.taillePort+1);
	strcpy(interface.ipv4,config.adresseIp);
	
	/*Vérification du port d'écoute*/
	if(config.port >= 1500 && config.port <= 65000){
		sprintf(portStr,"%d",config.port);
		strcpy(interface.port,portStr);
		free(portStr);
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
	if(bind(interface.lienConnexion,interface.infoServeur->ai_addr,interface.infoServeur->ai_addrlen) == -1){
		close(interface.lienConnexion);
		fprintf(stderr,"Erreur lors de la liaison au port %s -> Erreur : %s \n",interface.port,ErreurBind(errno));
		exit(EXIT_FAILURE);
	}

	/*On ecoute sur le port*/
	if(listen(interface.lienConnexion,60) == -1){
		close(interface.lienConnexion);
		fprintf(stderr,"Impossible d'écouter sur le port %s -> Erreur : %s \n",interface.port,ErreurBind(errno));
	}
	
	/*Renvoi de la connexion*/
	return interface; 
}

/*Fonction de connexion*/
TCPConnexion ConnexionClient(Configuration config){

	/*Nouvelle connexion*/
	TCPConnexion interface;
	char *portStr;

	/*Initialisation de la nouvelle connexion*/
	memset(&(interface.hints), 0, sizeof(interface.hints));
	interface.lienConnexion = 0;
	interface.infoServeur = NULL;
	interface.ipv4 = NULL;
	interface.hints.ai_family = AF_INET; // IPv4
	interface.hints.ai_socktype = SOCK_DGRAM; // TCP
	interface.hints.ai_flags = AI_PASSIVE; // Toutes les adresses disponibles
	interface.longueurAdresseClient = sizeof(struct sockaddr_in);
	interface.tailleMessage = config.tailleMessage;
	interface.port = calloc(1,config.taillePort+1);
	portStr = calloc(1,config.taillePort+1);
	

	/*Vérification du port d'écoute*/
	if(config.port >= 1500 && config.port <= 65000){
		sprintf(portStr,"%d",config.port);
		strcpy(interface.port,portStr);
		free(portStr);
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
int ReceptionMessageClient(TCPConnexion interface,struct sockaddr_in* adresseClient,char *msg){
	return recvfrom(interface.lienConnexion,msg,sizeof(msg),0,(struct sockaddr *) adresseClient,&(interface.longueurAdresseClient)) != -1;
}

/*Envoi Message Client*/
int EnvoiMessageClient(TCPConnexion interface,struct sockaddr_in* adresseClient,char *msg){
	return sendto(interface.lienConnexion,msg,sizeof(msg),0,(struct sockaddr *) adresseClient,interface.longueurAdresseClient) != -1;
}

/*Reception Message Serveur*/
int ReceptionMessageServer(TCPConnexion interface,char *msg){
	return recv(interface.lienConnexion,msg,sizeof(msg),0) != -1;
}

/*Envoi Message Serveur*/
int EnvoiMessageServer(TCPConnexion interface,char *msg){
	return sendto(interface.lienConnexion,msg,sizeof(msg),0,interface.infoServeur->ai_addr,interface.infoServeur->ai_addrlen) != -1;
}

/*Attente Message 1s*/
int AttenteMessage(TCPConnexion interface){
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
void FermetureConnexion(TCPConnexion interface,int Erreur){
	close(interface.lienConnexion);
	free(interface.port);
	if(interface.ipv4 != NULL)
		free(interface.ipv4);
		
	exit((Erreur > 256) ? 1 : Erreur);
}

/*Fonction pour récupérer une chaine alloué dynamiquement*/
char* GetDynamicStringFromConsole(int maxTailleChaine){
	int i,c;
	char *string;
	
	string = malloc(sizeof(char));
	string[0] = '\0';
	
	for(i=0;i<maxTailleChaine && (c=getchar()) != '\n' && c != EOF ;i++){
		string = realloc(string,(i+2)*sizeof(char));
		string[i] = (char)c;
		string[i+1] = '\0';
	}
	
	return string;
}




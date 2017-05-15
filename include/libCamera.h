#include "connexionCamera.h"

/*Définition des fonctions de l'API*/
UDPConnexion ConnexionClient(int,unsigned short int);
UDPConnexion ConnexionServer(char*,int,unsigned short int);
int ReceptionMessageClient(UDPConnexion,struct sockaddr_in*,char*);
int EnvoiMessageClient(UDPConnexion,struct sockaddr_in*,char*);
int ReceptionMessageServer(UDPConnexion,char*);
int EnvoiMessageServer(UDPConnexion,char*);
int AttenteMessage(UDPConnexion);
void FermetureConnexion(UDPConnexion,int);
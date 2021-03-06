#include "connexionCamera.h"
#include "../libConfig/ConfigConnectXml.h"

/*Définition des fonctions de l'API*/
TCPConnexion ConnexionClient(Configuration);
TCPConnexion ConnexionServer(Configuration);
int ReceptionMessageClient(TCPConnexion,struct sockaddr_in*,char*);
int EnvoiMessageClient(TCPConnexion,struct sockaddr_in*,char*);
int ReceptionMessageServer(TCPConnexion,char*);
int EnvoiMessageServer(TCPConnexion,char*);
int AttenteMessage(TCPConnexion);
void FermetureConnexion(TCPConnexion,int);
const char* ErreurBind(int);
char* GetDynamicStringFromConsole(int);

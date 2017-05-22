#include "connexionCamera.h"
#include "../libConfig/ConfigConnectXml.h"

/*Définition des fonctions de l'API*/
UDPConnexion ConnexionClient(Configuration);
UDPConnexion ConnexionServer(Configuration);
int ReceptionMessageClient(UDPConnexion,struct sockaddr_in*,char*);
int EnvoiMessageClient(UDPConnexion,struct sockaddr_in*,char*);
int ReceptionMessageServer(UDPConnexion,char*);
int EnvoiMessageServer(UDPConnexion,char*);
int AttenteMessage(UDPConnexion);
void FermetureConnexion(UDPConnexion,int);
const char* ErreurBind(int);
char* GetDynamicStringFromConsole(int);

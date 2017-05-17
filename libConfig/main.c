#include "main.h"

int main(){
    Configuration config;
    config = ParseConfig(GetConfigDoc("/home/wakidou/Documents/PST/Config.xml"));

    /*Affichage Config*/
    printf("Taille des messages : %d \n",config.tailleMessage);
    printf("Taille de l'adresse Ip : %d \n",config.tailleAdresse);
    printf("Adresse IP : %s \n",config.adresseIp);
    printf("Port d'écoute : %d \n",config.port);
    printf("Debit : %d %s/%s \n",config.debit,config.uniteDebit,config.tempsDebit);

    return EXIT_SUCCESS;
}

#include "ConfigConnectXml.h"

xmlDoc* GetConfigDoc(char* file){
    xmlDoc* document = xmlReadFile(file,"utf-8",0);
    if(document != NULL)
        return document;
    else{
        fprintf(stderr,"Erreur : le fichier n'a pas pu etre ouvert et décodé");
        exit(EXIT_FAILURE);
    }
}

char* GetAttribute(xmlNode* noeud,const char* nom){
    xmlChar* nomXml = xmlCharStrdup(nom);
    xmlChar* valueAttr = xmlGetProp(noeud,nomXml);
    if(valueAttr == NULL){
        fprintf(stderr,"Impossible de récupérer l'attribut %s pour le noeud %s",nom,noeud->name);
        exit(EXIT_FAILURE);
    }

    return (char *)valueAttr;
}

char* GetValue(xmlNode* noeud){
    return (char*)xmlNodeGetContent(noeud);
}

int CheckRoot(xmlNode* root){
    return !(strcmp((char*)root->name,"ConfigCamera"));
}

Configuration ParseConfig(xmlDoc* document){
    xmlNode *root,*first_child,*node;
    Configuration config;
    int test = 0;
    char *uniteDebit,*tempsDebit;

    /*Init root et config*/
    root = xmlDocGetRootElement(document);
    if(!CheckRoot(root)){
        fprintf(stderr,"Erreur : le document de configuration n'est pas conforme -> le noeud racine doit etre ConfigCamera \n");
        exit(EXIT_FAILURE);
    }

    memset(&config,0,sizeof(config));

    //Init taille message
    config.tailleMessage = atoi(GetAttribute(root,"tailleMessage"));

    //Init des autres données
    first_child = root->children;
    for(node = first_child;node != NULL;node = node->next){
        test = (!strcmp((char*)node->name,"Port")+2*(!strcmp((char*)node->name,"AdresseIP"))+3*(!strcmp((char*)node->name,"Debit")));
        switch(test){
            case 1 :
                config.taillePort = atoi(GetAttribute(node,"taillePort"));
                config.port = atoi(GetValue(node));
                break;
            case 2 :
                config.tailleAdresse = atoi(GetAttribute(node,"tailleAdresse"));
                config.adresseIp = calloc(1,config.tailleAdresse);
                strcpy(config.adresseIp,GetValue(node));
                break;
            case 3 :
                uniteDebit = GetAttribute(node,"unit");
                tempsDebit = GetAttribute(node,"temps");

                config.uniteDebit = calloc(1,sizeof(uniteDebit));
                config.tempsDebit = calloc(1,sizeof(tempsDebit));
                config.debit = atoi(GetValue(node));

                strcpy(config.uniteDebit,uniteDebit);
                strcpy(config.tempsDebit,tempsDebit);
                break;
            default:
                continue;
        }
    }

    xmlFreeNodeList(first_child);
    xmlFreeDoc(document);

    /*Retour Config*/
    return config;
}

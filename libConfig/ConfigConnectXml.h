#ifndef CONFIGCONNECTXML_H_INCLUDED
#define CONFIGCONNECTXML_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

typedef struct Configuration{
    int taillePort;
    int tailleAdresse;
    int tailleMessage;
    char* adresseIp;
    int port;
    int debit;
    char* uniteDebit;
    char* tempsDebit;
}Configuration;

xmlDoc* GetConfigDoc(char*);
int CheckRoot(xmlNode*);
Configuration ParseConfig(xmlDoc*);
char* GetAttribute(xmlNode*,const char*);
char* GetValue(xmlNode*);



#endif // CONFINGCONNECTXML_H_INCLUDED

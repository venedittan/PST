#Makefile pour compilation du projet : PST
.PHONY: clean

#Compilateur
CC = gcc

#Nom de l'Executable
EXEC = ConfigReader

#Executable
all: $(EXEC)

#Construction de l'éxecutable
$(EXEC): ConfigConnectXml.o main.o
			$(CC) -o $(EXEC) ConfigConnectXml.o main.o -lxml2   

#Construction des fichiers objets
ConfigConnectXml.o : ConfigConnectXml.c 
				$(CC) -o ConfigConnectXml.o -c ConfigConnectXml.c

main.o : main.c
			$(CC) -o main.o -c main.c

#Pour effacer les fichiers objets : appeler make clean
clean : 
		rm *.o

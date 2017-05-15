#!/bin/bash

if [ -z "$1" ]
	then 
		echo "Erreur . Spécifiez un arguement : -client ou -server"
else
	case "$1" in 
		"-client") 
			make -f MakefileClient
			make clean -f MakefileClient
			;;

		"-server")
			make -f MakefileServer
			make clean -f MakefileServer
			;;

		*)
			echo "Erreur de paramètres : tapez -client ou -server"
			;;
	esac
fi
 
#!/bin/sh

case $# in
	3);;
	*)echo "errore nel numero di parametri"
	exit 1 ;;
esac


case $1 in
	/*) if test ! -d $1 -o ! -x $1
		then
			echo "Errore il primo parametro non è una cartella o non è scrivibile"
			exit 3
		fi ;;
	*) echo "Errore il primo parametro non è un nome assoluto"
		exit 2;;
esac


PATH=$PATH:`pwd`
export PATH


FCR.sh $1 $*

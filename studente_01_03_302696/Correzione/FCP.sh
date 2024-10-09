#!/bin/sh

case $# in
0|1|2) 	echo Errore: numero parametri è $# quindi pochi parametri.
	exit 1;;
*) 	#DEBUG-OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia una directory
case $1 in
/*) 	if test ! -d $1 -o ! -x $1
	then
		echo $1 non directory
		exit 2
	fi;;
*) 	echo $1 non nome assoluto
	exit 3;;
esac

G=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)

#quindi ora possiamo usare il comando shift
shift

#ora in $* abbiamo i nomi relativi/semplici dei file 
for i 
do
	case $i in
	*/*) echo "$i non è un nome relativo semplice"  
	     exit 4;;
	*)   #echo OK $i stringa corretta  
		;;
	esac
done

#controlli sui parametri finiti 
PATH=`pwd`:$PATH
export PATH


#invochiamo
FCR.sh $G $*			#il primo parametro è la Gerarchia principale G il secondo parametro è il percorso attuale  il terzo parametro è il percorso attuale per arrivare alla gerarchia principale


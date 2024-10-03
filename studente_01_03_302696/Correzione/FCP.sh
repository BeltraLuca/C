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
	*/*) echo in una striga che sara\' usata come estensione non ci puo\' essere uno slash 
	     exit 4;;
	*)   #echo OK $i stringa corretta  
		;;
	esac
done

#controlli sui parametri finiti 
PATH=`pwd`:$PATH
export PATH

#invochiamo  
FCR.sh $G $*	#in alternativa invece che $* si poteva scrivere $1 e $2

echo "Ricerca conclusa"

files=""

for i
do
	cat /tmp/nomiRelativiSemplici | grep -w $i >> /dev/null
	if test $? -eq 0
	then
		echo "_____________$i verrà usato______________"
		files="$files $i"
	else
		echo "_____________$i non verrà usato_______________"
	fi
done

rm /tmp/nomiRelativiSemplici

echo "\nInizio FILE C\n"
maino $files

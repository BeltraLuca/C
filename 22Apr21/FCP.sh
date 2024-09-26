#!/bin/sh

case $# in
	1|2|3)echo "ERRORE numero parametri insufficiente"
		exit 1;;
	*);;
esac

case $1 in
	*[!0-9]*)echo "il primo parametro non è un numero"
		exit 2;;
	*) if test $1 -gt 0
		then
			W=$1
		else
			echo "il primo parametro non è strettamente positivo"
		fi;;
esac

S=$2

shift
shift

for G in $*
do
	case $G in
		/*)echo "$G è un nome assoluto";;
		*)echo "$G non è un nome assoluto"
			exit 4;;
	esac
done

PATH=`pwd`:$PATH
export PATH
file=/tmp/nomiAssoluti
>$file
for G in $*
do
	if test ! -d $G -o ! -x $G
        then
                echo "$G non è una cartella o non è eseguibile"
                exit 3
        fi
	FCR.sh $S $G $file
done

X=`cat $file | wc -l`
echo "sono state trovate $X  gerarchie"

if test $X -ge $W
then
	echo "Luca inserisci un numero compreso tra 1 e $W"
	X=0
	while test $X -gt $W -o $X -le 0
	do
		read X
	done
fi

cat $file
echo "\n";
cat $file | head -$X | tail -1



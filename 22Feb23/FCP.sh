#!/bin/sh

case $# in
	0|1|2) echo "Errore numero di parametri basso"
		exit 1;;
	*);;
esac

X=$1
shift
case $X in
	*[!0-9]*)echo "il primo parametro non e un numero o non e positivo"
		exit 2;;
	*);;
esac

if test $X -gt 500
then
	echo "errore il primo parametro non e strettamente minore di 500"
	exit 3
fi


for G in $*
do
	case $G in
		/*)
			if test ! -x $G -o ! -d $G
			then
				echo "Errore il parametro $G non e una catella o non e eseguibile"
				exit 5
			fi;;
		*) echo "errore il parametro $G non e un nome assoluto"
			exit 4;;
	esac
done

PATH=$PATH:`pwd`
export PATH

Q=1
for G in $*
do
	cat /dev/null > /tmp/fase$Q
	echo "sto entrando in FCR"
	FCR.sh $X /tmp/fase$Q $G
	if test `wc -l < /tmp/fase$Q` -ge 2
	then
		echo "fase 1\n\n`cat /tmp/fase$Q`"
		main `cat /tmp/fase$Q`
	fi
	Q=`expr $Q + 1`
done

rm /tmp/fase*

#!/bin/sh

case $# in
	1|2|3) echo "errore nell'inserimento di parametri"
		exit 1;;
	*);;
esac

case $1 in
	*[!1-9]*)echo "ERRRORE primo parametro passato non è un numero"
		exit 2;;
	*)H=$1;;
esac


case $2 in
	*[!1-9]*) echo "ERRORE secondo parametro passato non è un numero"
		exit 2;;
	*)M=$1;;
esac

shift
shift

for G in $*
do
	case $G in
		/*)echo "$G è un nome assoluto";;
		*)echo "$G non è un nome assoluto"
			exit 3;;
	esac
done

PATH=`pwd`:$PATH
export PATH
>/tmp/nomi
for G
do
	if test -d $G -a -x $G
	then
		FCR.sh $H $M $G
	fi
done

cat /tmp/nomi
main `cat /tmp/nomi`



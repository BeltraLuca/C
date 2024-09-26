#!/bin/sh

case $# in
	1|2|3)echo numero parametri insufficient
	exit 1;;
	*);;
esac

case $1 in
	/*)echo "$1 è un nome assoluto"
		if test -d $1 -a -x $1
		then
			G=$1
		else
			echo "$1 non è una directory o non è eseguibile"
			exit 5
		fi;;
	*)echo "$1 non è un nome assoluto"
	exit 2;;
esac


case $2 in
	*[!0-9]*)echo "$2 non è un numero"
		exit 3;;
	*) if test $2 -le 0
		then
			echo "$2 è un numero ma non è strettamente positivo"
			exit 3
		fi
		L=$2;;
esac

shift
shift

for Q in $*
do
	case $Q in
		?)echo "$Q è un carattere";;
		*)echo "$Q non è un carattere"
		exit 4;;
	esac
done

PATH=`pwd`:$PATH
export PATH
file=/tmp/nomiAssoluti
>$file
FCR.sh $G $L $file $*

echo "sono stati trovati `wc -l <$file` file"

for F in `cat $file`
do
	main $F $L $*
done

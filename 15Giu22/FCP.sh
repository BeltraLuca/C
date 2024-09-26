#!/bin/sh

case $# in 
	0|1|2)echo numero di parametri insufficiente
		exit -3;;
	*)echo numero di parametri giusto;;
esac 
Q=$#

for G in $*
do
	case $G in
		/*)echo $G è un nome assoluto
			if test ! -d $G -a ! -x $G
			then
				echo $G non è una directory
				exit -1
			fi;;

		*)echo $G non è un nome assoluto
		exit -2;;
	esac

done


to='/tmp/file'
>$to
PATH=`pwd`:$PATH
export PATH

for G in $*
do
	FCR.sh $G $to
	echo $G ha trovato `wc -l < $to` file
done

echo sono stati trovati in totale `wc -l < $to` file con caratteri tutti minuscoli

main `cat $to`		#richiamo l'eseguibile in c e gli passo i file trovati

`rm $to`		#cancello il file temporaneo
exit 0

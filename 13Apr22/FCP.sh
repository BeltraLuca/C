#!/bin/sh

case $# in
	0|1|2)echo numero parametri insufficienti
		exit -1;;
	*)echo si lavorerà con ` expr $# - 1 ` parametri;;
esac
Q=` expr $# - 2 `

case $1 in
	*[!1-9]*)echo il primo parametro non è n numero o non è strettamente positivo
	exit -2;;
	*)X=$1;;
esac

i=0


PATH=`pwd`:$PATH
export PATH
shift

for G in $*
do
	case $G in
		/*)
		 	if test ! -d $G -o ! -x $G 
			then
				echo $G non è una directory o non è attraversabile
				exit -4
			fi
			> /tmp/nomiAssoluti$$-$i
			FCR.sh $X $G /tmp/nomiAssoluti$$-$i
			echo il numero dei file trovati in questa gerarchia è di `wc -l </tmp/nomiAssoluti$$-$i`
			;;
		*) echo il parametro  $i non è un nome assoluto
			exit -5;;
	esac
	i=` expr $i + 1 `
done

for file1 in `cat /tmp/nomiAssoluti$$-0`
do
	
	n=0
	for i in *
	do

		if test $n -gt 0
		then
			for file2 in `cat /tmp/nomiAssoluti$$-$n`
			do
				if diff $file1 $file2 > /dev/null 2>&1
				then
					echo i file $file1 e $file2 sono uguali
				fi
			done
		fi
	done
	n=` expr $n + 1 `
done

n=0
for G
do
	#eliminiamo il file temporaneo n-esimo
	rm /tmp/nomiAssoluti$$-$n
	n=`expr $n + 1`
done
exit 0

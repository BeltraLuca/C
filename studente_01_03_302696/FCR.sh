#!/bin/sh

#instanzio la gerarchia
D=$1
CONT=0
tutti=

cd $D
#sposto in avanti i parametri
shift

H=`expr $# / 2 `
#file nella cartella
for File in *
do
	if test -f $File
	then
		#file parametri
		for f in $*
		do
			if test "$File" = "$f"
			then
				tutti="$tutti $f"
				CONT=` expr $CONT + 1 `
				break
			fi
		done
	fi
done

if test $CONT -ge $H
then
	echo "nella directory corrente $PWD sono stati trovati $CONT file"
	main $tutti
fi

for Dir in *
do
	if test -d $Dir -a -x $Dir
	then
		FCR.sh $Dir $*
	fi 
done
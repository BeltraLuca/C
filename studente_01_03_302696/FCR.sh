#!/bin/sh


#instanzio la gerarchia
G=$1
cont=0
tutti=
temp="/tmp/nomiRelativiSemplici"

cd $G
echo "----------------------------------------------------------------------------------------------------------------------------"
echo "\nRicerca nella directory `pwd`\n"
#sposto in avanti i parametri
shift

M=`expr $# / 2 `
#file nella cartella
for F in *
do
	if test -f $F
	then
		#file parametri
		for f in $*
		do
			if test "$F" = "$f"
			then
				tutti="$tutti $f"
				cont=` expr $cont + 1 `
				break
			fi
		done
	fi
done

echo "**********Dati sulla directory************************************ "
if test $cont -ge $M
then

	
	echo "# Nella directory `pwd` sono stati trovati $cont file, le occorrenze di nomi sono quindi maggiori e/o uguali a $M                                  "
	echo $tutti >> $temp
	echo "# I file trovati nella seguente directory sono $tutti                                                                        "

else
	echo "# Non sono state trovate occorenze o sono inferiori alla metà dei nomi relativi aggiunti"
fi
echo "----------------------------------------------------------------------------------------------------------------------------\n"

for Dir in *
do

	if test -d $Dir -a -x $Dir
	then
		echo "RICERCA SUCCESSIVA......"
		FCR.sh $Dir $*
	fi 
done

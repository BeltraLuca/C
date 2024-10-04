#!/bin/sh

#instanzio la gerarchia
G=$1
cont=0				#contatore di occorrenze
tutti=""			#variabile contenente i parametri trovati
P=$2
orig=$3
cd $G				#entro nella gerarchia

#sposto in avanti i parametri
shift
shift
shift

n=$#
resto=$((n%2))
M=`expr $# / 2 + $resto `
#file nella cartella
for F in *
do
	if test -f $F
	then
		#file parametri
		for f in $*
		do
			#verifica dei nomi relativi semplici
			if test "$P$F" = "$f"				#verifico che il file attuale sia tra i nomi specificati come parametro dall'utente
			then
				tutti="$tutti $f"			#aggiungo il parametro in una variabile $tutti che verrà poi mandata dopo opportuna verifica al main
				cont=` expr $cont + 1 `			#conto quanti parametri sono già stati trovati
			fi
		done
	fi
done


for Dir in *
do

        if test -d $Dir -a -x $Dir
        then
                if test $P=""
                then
                        P="$Dir/"
                else
                        P=$P/"$Dir"
                fi
                FCR.sh $Dir $P $orig $*
        fi
done


if test $cont -ge $M							#verifico che il numero di parametri trovati $cont sia maggiore o uguale alla metà dei parametri $M (punto del testo)
then

	echo "# Nella directory `pwd` sono stati trovati $cont file "			#stampo il nome assoluto della directory (punto del testo)
	cd $orig
	main $tutti									#faccio partire il main passando come parametri i parametri del file comandi trovati all'interno della directory (punto del testo) 
fi


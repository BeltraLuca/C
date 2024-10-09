#!/bin/sh

#instanzio la gerarchia
cont=0				#contatore di occorrenze
tutti=""			#variabile contenente i parametri trovati
G=$1				#memorizza il percorso principale della gerarchia mandata come parametro quando è stato richiamato FCP.sh

cd $G			#entro nella cartella

#sposto in avanti i parametri
shift

n=$#						#ho una variabile n perchè ad estrarre il modulo $# mi dava errore
resto=$((n%2))					#usato nel caso di parametri dispari, la metà si approssima per eccesso
M=`expr $# / 2 + $resto `			#calcolo la metà, in caso di numero $# pari il $resto non cambierà l'operazione in caso di numero dispari $# la metà avrà sommato 1 

#verifica dei file nella cartella
for F in *
do
	if test -f "$F"
	then
		#file parametri
		for f in $*
		do
			#verifica dei nomi relativi semplici
			if test "$F" = "$f"				#verifico che il file attuale sia tra i nomi specificati come parametro dall'utente
			then
				tutti="$tutti $f"			#aggiungo il parametro in una variabile $tutti che verrà poi mandata dopo opportuna verifica al main
				cont=` expr $cont + 1 `			#conto quanti parametri sono già stati trovati
			fi
		done
	fi
done


#verifica delle cartelle
for Dir in *
do

        if test -d "$Dir" -a -x "$Dir"
        then
                FCR.sh "$G/$Dir" $*
	fi
done


if test $cont -ge $M							#verifico che il numero di parametri trovati $cont sia maggiore o uguale alla metà dei parametri $M (punto del testo)
then

	echo "# Nella directory `pwd` sono stati trovati $cont file "			#stampo il nome assoluto della directory (punto del testo)
	cd $G										#prima di far partire il main rientro nella directory principale, in modo che  il main possa usare i nomi relativi semplici e trovare i file
	main $tutti									#faccio partire il main passando come parametri i parametri del file comandi trovati all'interno della directory (punto del testo) 
fi


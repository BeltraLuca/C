#!/bin/sh

cd $1

for file in *
do

	if test -f $file -a -r $F
	then
		if test `wc -l < $file` -gt 0
		then
			if `cat < $file | grep [A-Z] >/dev/null`
			then
				echo $file non è tutto minuscolo
			else
				echo file trovato! $file
				echo `pwd`/$file >> $2
			fi
		else 
			echo $file il file è vuoto
		fi
	fi

	if test -d $file -a -x $file
	then
		echo $file è una directory
		FCR.sh `pwd`/$file $2
	fi
done

exit 0

#!/bin/sh

cd $3
X=$1
Q=$2

echo "sono in `pwd`"
for F in *
do
	if test -f $F -a -r $F
	then
		if test `wc -c < $F` -lt $X
		then
			echo "il file `pwd`/$F ha `wc -c < $F` caratteri ovvero minori di $X"
			echo `pwd`/$F >> $Q
		fi
	fi
done

for D in *
do
	if test -d $D -a -x $D
	then
		FCR.sh $X $Q $D 
	fi
done




#!/bin/sh

echo $*
cd $1
shift
shift
cont1=0
cont2=0
tot=""

for F in *
do
	if test -f $F
	then
		case $F in
		*.$2)
			if test -r $F
			then
				cont1=` expr $cont1 + 1 `
				tot=$tot $F
			fi;;
		*.$3)
			if test -r $F -a -w $F
			then
				cont2=` expr $cont2 + 1 `
				tot=$tot $F
			fi;;
		*);;
		esac
	fi
done
	echo "TEST $cont1 $cont2"
	if test $cont1 -ge 1 -a $cont2 -ge 1
	then
		echo `pwd`
		#main $tot
	fi

for G in *
do
	if test -d $G -a -x $G
	then
		echo "entro dentro a $G"
		FCR.sh $G $1 $2
	fi
done

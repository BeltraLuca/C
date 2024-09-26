#!/bin/sh

cd $2

count=0
for F in *
do
	if test -f $F
	then
		case $F in
			*.$1)echo "trovato, il file si chiama $F"
				count=1;;
			*);;
		esac
	fi
done

if test $count -eq 1
then
	echo `pwd`>>$3
fi
for G in *
do
	if test -d $G -a -x $G
	then
		FCR.sh $1 $2 `pwd`/$G
	fi
done

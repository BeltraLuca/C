#!/bin/sh

cd $3

count=0
for G in *
do
	if test -f $G -a -r $G
	then
		if test `echo $G | wc -c` -eq 2 -a `cat $G | wc -l` -lt $1
		then
			count=`expr $count + 1`
			echo conteggio $count
		fi
	fi

done

if test $count -ge 2 -a $count -lt $2
then
	echo $3 >>/tmp/nomi
fi

for G in *
do
	if test -d $G -a -x $G
	then
		echo "entriamo dentro $G"
		FCR.sh $1 $2 $G
	fi
done

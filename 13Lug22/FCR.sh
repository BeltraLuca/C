#!/bin/sh

cd $1
L=$2
file=$3

shift
shift
shift

var=1
for F in *
do
	if test -f $F -a -r $F
	then
		if test `wc -l <$F` -eq $L
		then
			for C in $*
			do
				if ! `cat $F | grep $C >/dev/null`
				then
					var=0
				fi
			done
			
			if test $var -eq 1
			then
				echo "trovato `pwd`/$G"
				echo `pwd`/$F >>$file
			fi
			var=1
		fi
	fi
done

for G in *
do
	if test -d $G -a -x $G
	then
		FCR.sh `pwd`/$G $L $file $*
	fi
done


#!/bin/bash

if [ $# -ne 2 ] || [ ! -f $1 ] || [ ! -f $2 ]; then
	echo "Usage: Two arguments needed!"
	exit 1
fi

rows1=$(grep "$1" $1 | wc -l)
rows2=$(grep "$2" $2 | wc -l)

if [ ${rows1} -gt ${rows2} ]; then
	cat $1 | grep -o "\"[[:print:]]\+\"[[:print:]]\+$" | sort -d >> ./$1.songs
elif [ ${rows1} -lt ${rows2} ]; then
	cat $2 | grep -o "\"[[:print:]]\+\"[[:print:]]\+$" | sort -d >> ./$2.songs
else 
	echo "Same number of rows in $1 and $2!"
fi

exit 0

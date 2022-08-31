#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Usage: Three arguments needed!"
	exit 1
fi

while read line; do
	#echo "${line}"
	if ( cat $1 | grep "$3=" | cut -d'=' -f2 | grep -q "$line" ); then
		sed -E -i "s/($3=[[:print:]]*)$line([[:print:]]*)/\1\2/g" $1
		sed -E -i "s/[[:space:]]+/ /g" $1
		sed -E -i "s/=[[:space:]]\+/=/g" $1
	fi
done < <(cat $1 | grep "$2=" | cut -d'=' -f2 | tr ' ' '\n')	

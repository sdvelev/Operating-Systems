#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: Two parameters needed!"
	exit 1
fi

if [ ! -d $1 -o ! -d $2 ]; then
	echo "Arguments must be directories!"
	exit 2
fi

CH=$(find $2 -mindepth 1 -maxdepth 1 | wc -l)

if [ ${CH} -ne 0 ]; then
	echo "Directory $2 must be emprty!"
	exit 3
fi	


find $1 -mindepth 1 -maxdepth 1 ! -type d | xargs -n 1 cp -t $2
find $1 -mindepth 1 -type d | xargs -n 1 cp -r -t $2

while read line; do
	
	S=$(basename ${line})
	SU=$(basename ${line} | egrep "^\.(.)+\.swp" | wc -l)


	if [ ${SU} -ne 1 ]; then
		continue
	fi	

	N=$(echo ${line} | rev | cut -d'/' -f2- | rev)

	Q=$(echo ${S} | cut -c 2- | rev | cut -c5- | rev)

	RES=$(echo "${N}/${Q}")

	if [ -f ${RES} ]; then
		rm ${line}
	fi	

done < <(find $2 -mindepth 1 -type f)	


exit 0

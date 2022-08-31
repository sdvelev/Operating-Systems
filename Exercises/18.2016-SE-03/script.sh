#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Script needs two arguments"
	exit 1
fi

ln=${1}
rn=${2}

mkdir a b c

find . -mindepth 1 -maxdepth 1 -type f | while read line; do
	if [ ${line} = ${0} ]; then
		continue
	fi	

	rs=$(wc -l ${line} | cut -d' ' -f1)
	if [ ${rs} -lt ${ln} ]; then
		mv ${line} ./a
	elif [ ${rs} -gt ${ln} -a ${rs} -lt ${rn} -o ${rs} -lt ${ln} -a ${rs} -gt ${rn} ]; then
		mv ${line} ./b
	else 
		mv ${line} ./c
	fi	
done	

#!/bin/bash

if [ $(id -un) != "root" ]; then
	echo "Usage: root needed"
	exit 1
fi

tmp1=$(mktemp)

cat /etc/passwd | cut -d ':' -f 1,6 > ${tmp1}

while read line; do
	usr=$(echo ${line} | cut -d ':' -f 1)
	hmd=$(echo ${line} | cut -d ':' -f 2)

	if [ -z ${hmd} -o ! -d ${hmd} ]; then
		echo "Does not exist home directory for user ${usr}"
		continue
	fi	

	if [ #ssss -u ${urd} [ ! -w ${hmd} ] ]; then
		echo "Does not have rights to write in home directory for ${usr}"
	fi	

done < ${tmp1}	

rm ${tmp1}

#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: One parameter needed!"
	exit 1   
fi

#[ $(id -u) -eq 0 ] || exit 2

PROCESSES_FOO=$(ps -u $1 | wc -l)

while read line; do
	if [ $(echo "${line}" | awk '{print $1}') -gt ${PROCESSES_FOO} ]; then
		echo "${line}" | awk '{print $2}'
	fi	
	done < <(ps -e -o user= | sort | uniq -c)

	AVERAGE_TIME=$(ps -e -o times= | awk 'BEGIN {total=0} {total = total+$1} END{print (total/NR)}')	
	echo "Average: ${AVERAGE_TIME}"

	while read line; do
		flag=$(echo "$line" | awk -v avg=${AVERAGE_TIME} 'avg<$3 {print 1}')
		if [ ${flag} -eq 1 ]; then
			#kill $(echo "${line}" | awk '{print $1}')
			#sleep 2
			#kill -9 $(echo "${line}" | awk '{print $1}')
			echo "KILL process of user: $(echo "${line}" | awk '{print $1}') pid: $(echo "${line}" | awk '{print $2}')"
		fi
	done < <(ps -u ${1} -o user=,pid=,times=) 

exit 0

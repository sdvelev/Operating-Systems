#!/bin/bash

if [ $(id -un) != "s62537" ]; then
	echo "Script must be executed by root!"
	exit 1
fi

OUTPUT=$(mktemp)

ps -e -o uid=,pid=,rss= > "${OUTPUT}"

while read USR; do
		
	NUMPR=$(cat "${OUTPUT}" | awk -v usr="${USR}" 'BEGIN {COUNT = 0} $1 == usr {COUNT++} END {print COUNT}')

	if [ "${NUMPR}" -eq 0 ]; then
		continue
	fi	

	TOTALRSS=$(cat "${OUTPUT}" | awk -v usr="${USR}" '$1 == usr {RSS += $3} END {print RSS}')

	echo "User "${USR}" has "${NUMPR}" processes with total RSS "${TOTALRSS}.""

	AVG=$(echo "${TOTALRSS}" / "${NUMPR}" | bc)

	PIDM=$(cat "${OUTPUT}" | tr -s ' ' | cut -c 2- | egrep "^"${USR}".+$" | sort -n -r -t' ' -k 3,3 | head -n 1 | awk 'print $2')

	RSSM=$(cat "${OUTPUT}" | tr -s ' ' | cut -c 2- | egrep "^"${USR}".+$" | sort -n -r -t' ' -k 3,3 | head -n 1 | awk 'print $3')

	AVGD=$(("${AVG}" + "${AVG}"))

	echo "${PIDM}"
	continue

	if [ "${RSSM}" -gt "${AVGD}" ]; then
		#kill "${PIDM}"
		#sleep 2
		#kill -s SIGKILL "${PIDM}"
		echo "It will be killed."
	fi

done< <(cat /etc/passwd | cut -d':' -f3 )	


rm "${OUTPUT}"

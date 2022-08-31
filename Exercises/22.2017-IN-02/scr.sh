#!/bin/bash

file=$(mktemp)

ps -e -o user=,pid=,time= > "${file}"

user_p=$(cat ${file} | egrep "${1}" | wc -l

echo "num processes: ${user_p}")

while read br name; do
	
	if [ "${br}" -gt ${user_p} ]; then
		echo $name
	fi	

done < <(cat ${file} | awk '{print $1}' | sort | uniq -c | awk '{print $1" "$2}') 

cat "${file}" | awk '{print $1}' | sort | uniq -c | awk -v user_p=${user_p} '$1 > user_p { print $2 }'
#######################
res=$(cat "${file}" | tr : ' ' | awk '{total += $5 + $4*60 + $3 * 3600 } END {print total / NR }')

echo "${res}"

###############

egrep "${1}" "${file}" | tr : ' ' | awk -v res="${res}" '$5 + $4 * 60 + $3 * 3600 > 2 * res { print $2 } ' | xargs kill


#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: 1 argument"
	exit 1
fi

if [ $(id -un) != "root" ]; then
	echo "Usage: root user needed"
	exit 2
fi

tmp1=$(mktemp)
ps -e -o user= | sort | uniq > ${tmp1}

tmp2=$(mktemp)
ps -e -o user=,pid=,rss= > ${tmp2}

tmp3=0

while read line; do
	tmp3=$(awk -v line=${line} '$1 == line {sumr+=$3} END {printf "%d\n", sumr}' ${tmp2})
	echo "${line} ${tmp3}"

	if [ ${tmp3} -gt ${1} ]; then
		tokillPid=$(ps -u ${line} -o pid=,rss= | sort -k2,2rn | head -n 1 | awk '{print $1}')
		#echo ${tokillPid}
		kill ${tokillPid}
		sleep 2
		kill -9 ${tokillPid}
	fi	

done < <(cat ${tmp1})

rm -r ${tmp1} ${tmp2}

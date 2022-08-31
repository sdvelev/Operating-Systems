#!/bin/bash

RES=$(mktemp)

while read line; do
	USR=$(echo ${line} | cut -d':' -f1)
	HDR=$(echo ${line} | cut -d':' -f2)

	find ${HDR} -type f -printf "%T@ ${USR} %p" 2> /dev/null | sort -nr | head -n1 >> ${RES}

done < <(awk -F':' '{printf "%s:%s\n", $1, $6}' /etc/passwd)

cat ${RES} | sort -nr | head -n 1 | cut -d' ' -f2-

rm ${RES}

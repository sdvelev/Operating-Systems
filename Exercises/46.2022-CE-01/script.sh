#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Usage: Three parameters are needed!"
	exit 1
fi

PREFIX=$(echo "prefix.csv")

RES1=$(cat ${PREFIX} | egrep "^([a-zA-Z])+,${2},(.)+$" | cut -d ',' -f3)

RES2=$(echo ${1} \* ${RES1} | bc)

BASE=$(echo "base.csv")

UNIT=$(cat ${BASE} | egrep "^([a-zA-Z])+,${3},(.)+$" | cut -d ',' -f1)
MEASURE=$(cat ${BASE} | egrep "^([a-zA-Z])+,${3},(.)+$" | cut -d ',' -f3)

echo "${RES2} ${3} (${MEASURE}, ${UNIT})"

exit 0

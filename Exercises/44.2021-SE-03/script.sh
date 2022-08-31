#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: Two arguments needed!"
	exit 1
fi

echo "#include<uintstd.h>" > ${2}

echo -e "\n" >> ${2}

echo "int main() {" >> ${2}
echo -e "\tuint32_t arrN = NN;" >> ${2}

echo -e "\tconst uint16_t arr[] = {" >> ${2}

CNTI=0

while read line; do
	
	CNTI=$((${CNTI} + 1))

done < <(xxd ${1} | awk '{printf "%s %s %s %s %s %s %s %s\n", $2, $3, $4, $5, $6, $7, $8, $9}' | tr ' ' '\n')	

while read line; do
	#line = 1234
	CNT=$((${CNT} + 1))
	ONE=$(echo ${line} | cut -c 1-2)
	TWO=$(echo ${line} | cut -c 3-4)

	if [ ${CNT} -eq ${CNTI} ]; then
		echo -e "\t\t 0x${TWO}${ONE}" >> ${2}
		break
	fi	

	echo -e "\t\t 0x${TWO}${ONE}," >> ${2}

done < <(xxd ${1} | awk '{printf "%s %s %s %s %s %s %s %s\n", $2, $3, $4, $5, $6, $7, $8, $9}' | tr ' ' '\n')	

sed -i "s/NN/${CNT}/" ${2}

echo -e "\t};" >> ${2}
echo "}" >> ${2}
exit 0


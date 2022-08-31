#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: Two parameters needed!"
	exit 1
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > ${1}


while read line; do
	FN=$(basename ${line})
	
	HOSTNAME=$(echo ${FN} | rev | cut -c5- | rev )

	UNI=$(grep -A 6  "Maximum Physical Interfaces" ${line} | awk -F':' '{print $2}' | cut -c 2- | tr '\n' ',')

	LICENSE=$(grep "This platform has a" ${line} | cut -d' ' -f 5- | rev | cut -d' ' -f 2- | rev) 
	
	SN=$(grep "Serial Number:" ${line} |cut -d':' -f2 | tr -d ' ')	
	KEY=$(grep "Running Activation Key:" ${line} | cut -d':' -f2 | tr -d ' ')	

	echo "${HOSTNAME},${UNI}${LICENSE},${SN},${KEY}" >> ${1}

done < <(find ${2} -mindepth 1)	

exit 0

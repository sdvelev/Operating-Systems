#!/bin/bash

if [ $(whoami) != "s62537" ]; then
	echo "Error"
	exit 1
fi

while read line; do
	
	USR-$(echo $line | cut -d ':' -f1)
	HOMEDIR=$(echo $line | cut -d ':' -f 6)

	if [ ! -d $HOMEDIR ]; then
		echo "${USR}"
		continue
	fi

	PERMS=$(stat -c '%A' "$HOMEDIR")	
	
	if [ $(echo $PERMS | cut -c 3) == "w" ]; then
		echo "The owner has no wr"
		if [ "$(stat -c '%U' ${HOMEDIR})" == ${USR} ]; then
			continue
		fi
	fi
		FGROUP=$(stat -c '%G' ${HOMEDIR} )

		UGROUPS=$(id -nG "${USR}")

	if [ $(echo $PERMS | cut -c 6) == "w" ]; then 
		if echo "${UGROUPS}" | grep -q "\<${FGROUP}\>" ; then
			continue
		fi	
	fi

	if [ $(echo $PERMS | cut -c 9) == "w" ]; then
		continue	
	fi

	echo "${USR}"

done < <(cat /etc/passwd)


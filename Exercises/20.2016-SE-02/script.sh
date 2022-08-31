#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: 1 parameter"
	exit 1
fi

cat $1 | cut -d' ' -f4- | egrep -n "^" | sed -E 's/^([[:digit:]]+):/\1. /' | sort -t' ' -k2

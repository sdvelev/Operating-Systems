#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: 1 argument"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "Not directory"
	exit 2
fi	

#First solution
#find -L $1 -mindepth 1 -type l

#Second solution
#find $1 -mindepth 1 -type l -printf "%Y %p\n" 2> /dev/null | egrep "^[LN]" | cut -c3-

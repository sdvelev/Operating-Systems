#!/bin/bash

if [ $# != 2 ]; then
	echo "Usage: Two arguments needed!"
	exit 1
fi	

if [ ! -d $1 ]; then
	echo "First directory $1 does not exist!"
	exit 2
fi	

REQPATH=$(echo "$2/images")

mkdir -p "${REQPATH}"

while read "FILENAME"; do


	BASEFILENAME=$(basename "${FILENAME}")


	FILEWITHOUTEXT=$(echo "${BASEFILENAME}" | rev | cut -c 5- | rev)

	CLEANFILEWITHOUTEXT=$(echo "${FILEWITHOUTEXT}" | tr -s ' ' | awk '$1=$1')

	TITLE=$(echo "${CLEANFILEWITHOUTEXT}" | sed -E 's/\(([^\)])*\)//g') 

	if  echo "${CLEANFILEWITHOUTEXT}" | grep -q "("; then
		ALBUM=$(echo "${CLEANFILEWITHOUTEXT}" | rev | cut -d')' -f2 | cut -d'(' -f1 | rev)
	else
		ALBUM=$(echo "misc")
	fi

	DATEMOD=$(stat -c'%y' "${FILENAME}" | cut -d' ' -f1)

	HASH=$(sha256sum "${FILENAME}" | cut -c1-16)

	#echo "${HASH}"

#	REQPATH=$(echo "$2/images")

#	mkdir -p "${REQPATH}"

	cp "${FILENAME}" "${REQPATH}"

	NM=$(echo ""${REQPATH}"/"${BASEFILENAME}"") 
	mv "${NM}" ""${REQPATH}"/"${HASH}".jpg"

	PATTERN1=$(echo "$2/by-date/"${DATEMOD}"/by-album/"${ALBUM}"/by-title")
	PATTERN2=$(echo "$2/by-date/"${DATEMOD}"/by-title")
	PATTERN3=$(echo "$2/by-album/"${ALBUM}"/by-date/"${DATEMOD}"/by-title")
	PATTERN4=$(echo "$2/by-album/"${ALBUM}"/by-title")
	PATTERN5=$(echo "$2/by-title")

	mkdir -p "${PATTERN1}"
	mkdir -p "${PATTERN2}"
	mkdir -p "${PATTERN3}"
	mkdir -p "${PATTERN4}"
	mkdir -p "${PATTERN5}"

	ln -s ../../../../../images/""${HASH}".jpg" ""${PATTERN1}"/"${TITLE}".jpg" 	
	ln -s ../../../images/""${HASH}".jpg" ""${PATTERN2}"/"${TITLE}".jpg" 
	ln -s ../../../../../images/""${HASH}".jpg" ""${PATTERN3}"/"${TITLE}".jpg" 
	ln -s ../../../images/""${HASH}".jpg" ""${PATTERN4}"/"${TITLE}".jpg" 
	ln -s ../images/""${HASH}".jpg" ""${PATTERN5}"/"${TITLE}".jpg" 

done < <(find $1 -type f -name *.jpg)

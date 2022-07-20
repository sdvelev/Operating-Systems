#!/bin/bash

#Check for enough parameters
if [ $# -ne 4 ]; then
	echo "The script needs four parameters!"
	exit 1
fi

fiArchive=${1}
seSuccess=${2}
thFailed=${3}
foLogFile=${4}

inputArc=$(file -b "./${fiArchive}" | cut -d' ' -f1)

#Check for zip archive as first parameter
if [ "${inputArc}" != "Zip" ]; then
	echo "The first parameter must be Zip archive!"
	exit 2
fi	

#Check for non-existense of second, third and fourth parameters as directories and file
if [ -d ./${seSuccess} ]; then
	echo "The second parameter already exists as directory!"
	exit 3
fi

if [ -d ./${thFailed} ]; then
	echo "The third parameter already exists as directory!"
	exit 4
fi

if [ -f ./${foLogFile} ]; then
	echo "The fourth parameter already exists as file!"
	exit 5
fi	

#Function to move files in failed directory
mvFunction () {
	mv ${1} ${3}
	mv ${3}/$(basename ${1}) ${3}/${2}
}	

#Function to declare error in unarchiving
errUnarchiving () {
	echo "Error in unarchiving!"
}

#Create directories and file according to parameters
mkdir -p ${seSuccess} > /dev/null 2>&1
mkdir -p ${thFailed} > /dev/null 2>&1
mkdir -p $(dirname $(echo ${foLogFile})) > /dev/null 2>&1
touch ${foLogFile} > /dev/null 2>&1

#Check for successful creation
if [ ! -d ${seSuccess} -o ! -d ${thFailed} -o ! -f ${foLogFile} ]; then
	echo "Directories or files could not be created!"
	rm -r ${seSuccess} ${thFailed} ${foLogFile} > /dev/null 2>&1
	exit 6	
fi

#Create temporary directory to store results from unarchiving the zip archive
tmpdir=$(mktemp -d tmpdirXXX)

unzip -d  ./${tmpdir} "${fiArchive}" > /dev/null 2>&1

#Check for error in unarchiving 
if [ $? -ne 0 ]; then
	errUnarchiving
	rm -r ${seSuccess} ${thFailed} ${foLogFile} > /dev/null 2>&1
	rm -r ./${tmpdir} > /dev/null 2>&1
	exit 7
fi

#Declare variable to store correct faculty number
fn=0

#Iterate though each faculty number directory
find ./${tmpdir} -mindepth 1 -maxdepth 1 -type d 2> /dev/null | while read line; do

	fn=$(echo ${line} | cut -d'-' -f1 | cut -d'/' -f3)

	#Check for existence of more than a file or directory - without count of hidden files 
	if [ $(find ${line} -mindepth 1 -maxdepth 1 | egrep -v '^\./\.(.)*$' | wc -l) -ne 1 ]; then
	mvFunction ${line} ${fn} ${thFailed}
	continue
	fi

	#Find name of file
	filename=$(find ${line} -mindepth 1 -maxdepth 1 | egrep -v '^\./\.(.)*$')

	#Declare variables for storing formal errors
	errOne=0
	errTwo=0
	errThree=0
	errFour=0


	#Check for error 1 - whether the name is correct
	if [ ! -f ${line}/${fn}\.tar\.xz ]; then
		errOne=1
	fi	

	#Declare variable to store faculty number of the handed file - it might differ from the correct one 
	fnDir=$(basename ${filename}| cut -d'.' -f1)

	#Declare variables to store correct file format of the handed file
	filenameType=$(file -b ${filename} | cut -d' ' -f1)
	filenameTypeA=$(file -b ${filename} | cut -d' ' -f2)

	#Declare variables to store boolean value of correct format
	fxz=0
	fgzip=0
	fbzip2=0
	frar=0
	fzip=0
	ftar=0

	#Check for correct file format
	if [ "${filenameType}" = "XZ" ]; then
		errTwo=0
		fxz=1
	elif [ "${filenameType}" = "gzip" ]; then
		errTwo=1
 		fgzip=1
	elif [ "${filenameType}" = "bzip2" ]; then
		errTwo=1
 		fbzip2=1
	elif [ "${filenameType}" = "RAR" ]; then
		errTwo=1
 		frar=1
	elif [ "${filenameType}" = "Zip" ]; then
		errTwo=1
 		fzip=1
	elif [ "${filenameType}" = "POSIX" -a "${filenameTypeA}" = "tar" ]; then
		errTwo=1
		ftar=1
	else
		mvFunction ${line} ${fn} ${3}
		continue
	fi

	#Check for actions to be undertaken according to correct format
	if [ ${fxz} -eq 1 ]; then
		filenameAdd=$(mktemp tmpfileXXX)
		#Decompressing in new different file
		xz -d < ${filename} > ./${filenameAdd} 2> /dev/null

		if [ $? -ne 0 ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi	

		#Delete archive file
		rm -r ${filename}

		#Update correct file format of received file
		filenameType=$(file -b ./${filenameAdd} | cut -d' ' -f2)

		#Check for being tar archive - otherwise move in failed directory
		if [ ${filenameType} != "tar" ]; then
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Unarchive tar file in tmpdir directory
		tar xf ./${filenameAdd} --directory ${line} > /dev/null 2>&1

		if [ $? -ne 0 ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi
		
		#Delete tar archive
		rm -r ./${filenameAdd}
	
	elif [ ${fgzip} -eq 1 ]; then

		tmpf=$(mktemp tmpfileXXX)

		#Update variable with the correct filename of the archive
 		filename=$(find ${line} -mindepth 1 -maxdepth 1)
	
		#Decompressing in new different file
		gzip -d < ${filename} > ./${tmpf} 2> /dev/null

		if [ $? -ne 0  ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Update variable to store format of decompressed archive
 		filenameType=$(file -b ${tmpf} | cut -d' ' -f2)

		if [ ${filenameType} != "tar" ]; then
			rm -r ./${tmpf} > /dev/null 2>&1
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Delete original gz archive
		rm ${filename} > /dev/null 2>&1

		#Unarchive tar file in the same directory
		tar xf ./${tmpf} --directory ${line} > /dev/null 2>&1

		if [ $? -ne 0 ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${3}
			continue
		fi

		#Delete tar archive
		rm -r ./${tmpf} > /dev/null 2>&1

	elif [ ${fbzip2} -eq 1 ]; then

		tmpf=$(mktemp tmpfileXXX)

		#Update variable with the correct filename of the archive
 		filename=$(find ${line} -mindepth 1 -maxdepth 1)
	
		#Decompressing in new different file
		bzip2 -d < ${filename} > ./${tmpf} 2> /dev/null

		if [ $? -ne 0  ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Update variable to store format of decompressed archive
 		filenameType=$(file -b ./${tmpf} | cut -d' ' -f2)

		if [ ${filenameType} != "tar" ]; then
			rm -r ./${tmpf} > /dev/null 2>&1
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Delete original bz2 archive
		rm ${filename} > /dev/null 2>&1

		#Unarchive tar file in the same directory
		tar xf ./${tmpf} --directory ${line} > /dev/null 2>&1

		if [ $? -ne 0 ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${3}
			continue
		fi
		
		#Delete tar archive
		rm -r ./${tmpf} > /dev/null 2>&1

	elif [ ${fzip} -eq 1 ]; then

		fnDirAdd=$(mktemp -d tmpdirXXX)

		#Decompress in different new directory
		unzip -q ${filename} -d ./${fnDirAdd}

		if [ $? -ne 0  ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi
		
		#Delete initial zip archive
		rm -r ${filename} > /dev/null 2>&1

		#Move everything from the new directory
		mv ./${fnDirAdd}/* ${line} > /dev/null 2>&1
	
		#Delete newly created directory
		rm -r ./${fnDirAdd} > /dev/null 2>&1

	elif [ ${frar} -eq 1 ]; then

		fnDirAdd=$(mktemp -d tmpdirXXX)
		
		#Decompress rar archive in created directory since directories in rar are not stored
		unrar x ${filename}  ./${fnDirAdd} > /dev/null 2>&1

		if [ $? -ne 0  ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi

		#Delete initial rar achive
		rm -r ${filename} > /dev/null 2>&1

		#Move everything from the new directory
		mv ./${fnDirAdd}/* ${line} > /dev/null 2>&1

		#Delete newly created directory
		rm -r ./${fnDirAdd} > /dev/null 2>&1

	elif [ ${ftar} -eq 1 ]; then
		
		filenameAdd=$(mktemp -d tmpfileXXX)

		#Unarchive tar file in tmpdir directory
		tar xf ${filename} --directory ./${filenameAdd} > /dev/null 2>&1

		if [ $? -ne 0 ]; then
			errUnarchiving
			mvFunction ${line} ${fn} ${thFailed}
			continue
		fi
		
		#Delete tar archive
		rm -r ${filename} > /dev/null 2>&1

		#Move everything from the new directory
		mv ./${filenameAdd}/* ${line} > /dev/null 2>&1
		
		#Delete newly created temporary directory
		rm -r ./${filenameAdd} > /dev/null 2>&1
	fi	

	#Delete each directory which starts with __MACOSX (often contain hidden files for FS)
	find ${line} -mindepth 1 -maxdepth 1 -iname "__MACOSX*" -type d -exec rm -r {} \;

	#Check if there exists a directory with the correct name
	if [ ! -d ${line}/${fn} ]; then
		errFour=1
	fi	

	#Check if there is not only one directory on current level - if so, it marks it as wrong 
	if [ $(find ${line} -mindepth 1 -maxdepth 1 -type d | wc -l) -ne 1 ]; then
	
	#Another logic which can be applied to the example zip archive - it can replace the above "if" so that the flags become as in the example "flags.txt":
	#if [ $(find ${line} -mindepth 1 -type d|wc -l) -ne 1 -a ${errFour} -ne 0 -o ${errFour} -eq 0 -a $(find ${line} -mindepth 1 -type d| wc -l) -gt 2 ]; then

	errThree=1
	fi		

	#Check if either non-existing directory or wrong directory name
	if [ ${errThree} -eq 1 -o ${errFour} -eq 1 ]; then
		#Create in ${seSuccess} a new directory with the faculty number as it is not provided by the student
		mkdir ${seSuccess}/${fn} > /dev/null 2>&1

		numDirs=$(find ${line} -mindepth 1 -maxdepth 1 -type d | wc -l)
		dirs=$(find ${line} -mindepth 1 -maxdepth 1 -type d)
		if [ ${numDirs} -eq 1 ]; then
			mv ${dirs}/* ${seSuccess}/${fn} > /dev/null 2>&1
		else
			mv ${line}/* ${seSuccess}/${fn} > /dev/null 2>&1
		fi

	else
		#Move directory to the ${seSuccess} directory with the successful files
		mv ${line}/${fn} ${seSuccess} > /dev/null 2>&1
	fi

	#Attach the flags to the ${foLogFile} file by adding a new record as new line
	echo "${fn} ${errOne} ${errTwo} ${errThree} ${errFour}" >> ${foLogFile}
done

#Create temporary file to sort ${foLogFile}
tmpcp=$(mktemp tmpfileXXX)

#Sort ${foLogFile} by faculty number and store the result in the new file
sort -n -k 1,1 -t' ' ${foLogFile} > ${tmpcp} 2>/dev/null

#Delete the unsorted ${foLogFile} file with the flags
rm -r ${foLogFile} > /dev/null 2>&1

#Rename the new temporary file as ${foLogFile}
mv ${tmpcp} ${foLogFile} > /dev/null 2>&1

#Delete temporary diretory for storing ${fiArchive} after decomressing
rm -r ./${tmpdir} > /dev/null 2>&1

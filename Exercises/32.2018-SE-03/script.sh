#!/bin/bash

if [ $# -ne 2 -o ! -f $1 ];then
    echo "Usage: Two argument files needed!"
    exit 1
fi

touch $2

while read line
do
    if ( cat $2 | grep -q "^[[:digit:]]\+,$(echo $line | cut -d ',' -f2-)$" );then
        continue
    fi

    if [ $(cat $1 | grep "^[[:digit:]]\+,$(echo $line | cut -d ',' -f2-)$" | wc -l) -gt 1 ];then
        cat $1 | grep "^[[:digit:]]\+,$(echo $line | cut -d ',' -f2-)$" | sort -n -t ',' -k1 | head -n 1 >> $2
    else
        echo $line >> $2
    fi
done < <(cat $1)

exit 0

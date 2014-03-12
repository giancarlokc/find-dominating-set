#!/bin/bash

# Check arguments
if [ $# -lt 2 ]
then
	echo "Usage: ./run.sh [Input file directory] [Algorithm 1, Algorithm 2, ... , Algorithm N]"
	exit
fi

# Check if input directory exists
if [ ! -d $1 ]
then
	echo "Invalid input directory!"
	exit
fi

# Remove previous output files
rm -rf $1/*-out
rm -rf $1/*-checked

# For each file in the input directory
for f in $1*
do
	echo "################################# - $f"
	echo ""
	j=0
	# For each algorithm
	for i in ${@:2}
	do
		# Check if the algorithm file exists
		if [ -f $i ]
		then
			echo "Algorithm: $i - Input: $f"
			newname="$f-out$j"
			time ./"$i" 0 < $f > $newname
			./bin/checkDomSet < $newname
			rm -rf tmpTime
			echo "-- FINISH --"
			echo ""
		else
			echo "File '$i' not found!"
			echo ""
		fi
		j=`expr $j + 1`
	done
done

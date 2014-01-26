#!/bin/bash

rm fullerenes/*-test

i=0
for f in fullerenes/c*
do
	echo "Entry: $f"
	# do something on $f
	newname="$f-test"
	time ./findDomSet < $f > $newname
	echo "-- FINISH --"
	echo ""
done

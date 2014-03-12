#!/bin/bash

make

rm -rf fullerenes/*-test
rm -rf fullerenes/*-checked

i=0
for f in fullerenes/*
do
	echo "Entry: $f"
	# do something on $f
	newname="$f-test"
	time ./findDomSet 0 < $f > $newname
	./checkDom < $newname > "$newname-checked"
	echo "-- FINISH --"
	echo ""
done

#!/bin/bash

for thread in {0..31} 
do
	for run in {0..15}
	do
		./run.sh 100000 $thread 0 >> result.csv;
		printf "," >> result.csv;
	done
	echo "" >> result.csv;
done	



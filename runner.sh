#!/bin/bash

gcc main.c -lpthread -lrt -g -o main.out; 

for thread in {0..16} 
do
	for run in {0..15}
	do
		./main.out 100000 $thread 0 1 >> result.csv;
		printf "," >> result.csv;
	done
	echo "" >> result.csv;
done	



#!/bin/bash

for run in {0..15}
do
        ./run.sh 100000 16 100000 1 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv

for run in {0..15}
do
	./run.sh 100000 16 50000 >> result_min_size.csv;
	printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 100000 16 25000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 100000 16 12500 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 100000 16 6250 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 100000 16 3125 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;






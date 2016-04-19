#!/bin/bash

for run in {0..15}
do
        ./run.sh 10000000 16 10000000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv

for run in {0..15}
do
	./run.sh 10000000 16 5000000 >> result_min_size.csv;
	printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 10000000 16 2500000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 10000000 16 1250000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 10000000 16 625000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 10000000 16 312500 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;






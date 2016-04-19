#!/bin/bash

for run in {0..15}
do
        ./run.sh 1000000 16 1000000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv

for run in {0..15}
do
	./run.sh 1000000 16 500000 >> result_min_size.csv;
	printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 1000000 16 250000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15} 
do
        ./run.sh 1000000 16 125000 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 1000000 16 62500 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;

for run in {0..15}
do
        ./run.sh 1000000 16 31250 >> result_min_size.csv;
        printf "," >> result_min_size.csv;
done
echo "" >> result_min_size.csv;






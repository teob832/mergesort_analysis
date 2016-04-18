#!/bin/bash

for j in {1..25}
do
    for i in {0..8} 
    do
        ./run.sh 10000000 $i 1000000 >> 10mil.out;
    done
    echo "" >> output.txt;
done

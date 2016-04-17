#!/bin/bash

for i in {0..32}
do
    ./run.sh 10000000 $i 0 >> output.txt;
    sleep 1;
done



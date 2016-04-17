#!/bin/bash
gcc main.c -lpthread -lrt -g -o main.out; 
./main.out $1 $2 $3 $4;



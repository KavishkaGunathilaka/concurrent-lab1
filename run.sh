#!/bin/bash

mkdir output

gcc -g -Wall -o serial serial.c -lpthread
gcc -g -Wall -o parallel_rw_lock  parallel_rw_lock.c  -lpthread
gcc -g -Wall -o parallel_one_mutex parallel_one_mutex.c -lpthread

./serial 385 && ./parallel_one_mutex 385 && ./parallel_rw_lock 385
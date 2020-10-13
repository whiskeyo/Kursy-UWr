#!/bin/bash

echo "#id   time   IPC" > testsIPC3.dat

for ((i = 1; i <= 10; i++)); do   
  resultsv1=($( ./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v 1 -p ipc | awk '/Time/{print $(NF-1); next} /cycle/{print $NF}'))
  echo "$i ${resultsv1[0]} ${resultsv1[1]}" >> testsIPC3.dat
done

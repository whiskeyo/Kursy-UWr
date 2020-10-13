#!/bin/bash

#echo "" > tests.dat # clear contents of the file
echo "#id   v0time    v0l1    v0l2    v0l3      v0tlb     v1time    v1l1    v1l2    v1l3    v1tlb" > tests.dat
echo "| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|" > table.md
echo "|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|" >> table.md

for ((i = 1; i <= 50; i++)); do   
  resultsv0=($( ./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v 0 -p memory | awk '/Time/{print $(NF-1); next} /L1/{print $NF; next} /L2/{print $NF; next} /L3/{print $NF; next} /TLB/{print $NF}'))
  resultsv1=($( ./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v 1 -p memory | awk '/Time/{print $(NF-1); next} /L1/{print $NF; next} /L2/{print $NF; next} /L3/{print $NF; next} /TLB/{print $NF}'))
  echo "$i ${resultsv0[0]} ${resultsv0[1]} ${resultsv0[2]} ${resultsv0[3]} ${resultsv0[4]} ${resultsv1[0]} ${resultsv1[1]} ${resultsv1[2]} ${resultsv1[3]} ${resultsv1[4]}" >> tests.dat
  echo "|${resultsv0[0]}|${resultsv0[1]}|${resultsv0[2]}|${resultsv0[3]}|${resultsv0[4]}|${resultsv1[0]}|${resultsv1[1]}|${resultsv1[2]}|${resultsv1[3]}|${resultsv1[4]}|" >> table.md
done

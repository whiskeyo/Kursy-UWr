#!/bin/bash

#echo "" > tests.dat # clear contents of the file
echo "time2 ipc2" > tests.dat

for ((i = 1; i <= 30; i++)); do   
  #resultsv0=($( ./texture -S 0xdeadc0de -t 200000 -v 0 -p ipc | awk '/Time/{print $(NF-1); next} /cycle/{print $NF;}'))
  resultsv1=($( ./texture -S 0xdeadc0de -t 200000 -v 1 -p ipc | awk '/Time/{print $(NF-1); next} /cycle/{print $NF;}'))
  echo "$i ${resultsv1[0]} ${resultsv1[1]}" >> tests.dat
done

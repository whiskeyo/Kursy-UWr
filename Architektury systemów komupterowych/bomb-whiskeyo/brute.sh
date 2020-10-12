#!/bin/bash
> brute.txt

test() {
  out=$(./bomb psol.txt <<< "$1 $2") 
  echo $out
}

for ((i=0; i<1000;i++))
do
    for ((j=0; j<1000;j++))
    do
        echo "i: $i j: $j $(test $i $j)" >> brute.txt
    done
done

#!/bin/bash
# write(){
#     #mis0=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 0 -p branch | grep "Branch" | grep -o -E '[0-9]+.[0-9]')
#     #te0=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 0 -p branch | grep "Time" | grep -o -E '[0-9.]+')
#     #mis1=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 1 -p branch | grep "Branch" | grep -o -E '[0-9]+.[0-9]')
#     #te1=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 1 -p branch | grep "Time" | grep -o -E '[0-9.]+')
#     #echo "$mis0     $te0        $mis1   $te1"

#     n04=$(./randwalk -S 0x321 -n 4 -s 16 -t 14 -S 0x123 -v 0 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n05=$(./randwalk -S 0x321 -n 5 -s 16 -t 14 -S 0x123 -v 0 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n06=$(./randwalk -S 0x321 -n 6 -s 16 -t 14 -S 0x123 -v 0 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n07=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 0 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n08=$(./randwalk -S 0x321 -n 8 -s 16 -t 14 -S 0x123 -v 0 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n14=$(./randwalk -S 0x321 -n 4 -s 16 -t 14 -S 0x123 -v 1 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n15=$(./randwalk -S 0x321 -n 5 -s 16 -t 14 -S 0x123 -v 1 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n16=$(./randwalk -S 0x321 -n 6 -s 16 -t 14 -S 0x123 -v 1 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n17=$(./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123 -v 1 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     n18=$(./randwalk -S 0x321 -n 8 -s 16 -t 14 -S 0x123 -v 1 | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     echo "$n04  $n05    $n06    $n07    $n08    $n14  $n15    $n16    $n17    $n18"
# }

# for ((i = 1; i <= 10; i++)); do
#     #write >> tests.dat
#     write >> times.dat
# done

# write() {
#     progout=$(./randwalk -S 0xea3495cc76b34acc  -n 7 -s 12 -t 10 -v 1 -p ipc)
#     o1=$(echo "$progout" | grep "Time" | grep -o -E '[0-9]+.[0-9]+')
#     o2=$(echo "$progout" | grep "cycle" | grep -o -E '[0-9]+.[0-9]+')
#     o3=$(echo "$progout" | grep "Total" | grep -o -E '[0-9]+.[0-9]+')
#     echo "$1    $o1   $o2     $o3"
# }

# for ((i = 1; i <= 1000; i++)); do
#     write $i >> times22.dat
# done

myarr=($(./randwalk -S 0xea3495cc76b34acc  -n 7 -s 12 -t 10 -v 1 -p ipc | awk '/Time/{print $(NF-1);next} /cycle/{print $NF;next} /Total/{print $NF}' ))
echo ${myarr[0]} ${myarr[1]} ${myarr[2]}
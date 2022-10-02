#!/bin/bash
# ./ptxt3.sh -a -s -sd -d -asd -d -asd2 2 

argArray=( "$@" )
arrayLen="${#argArray[@]}"


for (( i=0; i < arrayLen-1 ; i++ ))
do
    flag=0
    for (( j=0; j<arrayLen-i-1; j++ ))
    do
        # if (( ${argArray[$j]} \> ${argArray[$j+1]} )); then
        if [ ${argArray[$j]} \> ${argArray[$j+1]} ]; then
            temp=${argArray[$j]}
            argArray[$j]=${argArray[$j+1]}
            argArray[$j+1]=$temp
        fi
    done
done

for (( l=0; l<arrayLen; l++))
do
    echo "${argArray[$l]} "
done
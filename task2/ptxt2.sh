#!/bin/bash
# ./ptxt2.sh -n 2 -t 0.2 -- "asd" 

while getopts n:t: arg
do
    case $arg in 
    n )
        n_var=${OPTARG}
        ;;
    t )
        t_var=$OPTARG
        ;;
    * )
        echo "Usage $0 -n . -t ."
        exit 1
        ;;
    esac
done
shift $(expr $OPTIND - 1)

if [[ -z "$n_var" ]]
then
    echo "Params -n is Null"
    exit 1
fi

for t in $(seq $n_var)
do
    if [[ -n "$t_var" ]]
    then
        echo $1
        sleep $t_var
    else
        echo "Params -t is Null"
        exit 1
    fi
done
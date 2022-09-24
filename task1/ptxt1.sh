#!/bin/bash
# ./ptxt1.sh -n 2 -t 0.22 -- he 


while [ -n "$1" ];
do
    case $1 in 
    "-n" )
        n_var=$2
        shift 2
        ;;
    "-t" )
        t_var=$2
        shift 2
        ;;
    --  )
        text=$2
        break
        ;;
    *)
        echo "Usage $0 -n . -t ."
        exit 1
        ;;
    esac
done

if [[ -z "$n_var" ]]
then
    echo "Params -n is Null"
    exit 1
fi

for t in $(seq $n_var)
do
if [[ -n "$t_var" ]]
    then
        echo $text
        sleep $t_var
    else
        echo "Params -t is Null"
        exit 1
    fi
done
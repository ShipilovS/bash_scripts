#!/bin/bash

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

for t in $(seq $n_var)
do
    echo $text
    sleep $t_var
done
#!/bin/bash
# 3 лабораторная - скачать  [log file](https://drive.google.com/file/d/1kSDVeCrxjWaGFnI9F-wPjdeeVmuX3WaF/view?usp=sharing), в котором необходимо иметь возможность фильтрации по дате и времени как в find, к примеру:
# * `-d +2015-07-29` - отображать только сткроки позже заданного числа
# * `-d 2015-07-29` - строго в этот день
# * `-d -2015-07-29` - раньше заданного числа  
# Необходимо добавить возможность выставления уровня логгирования (к примеру, -l <log_level>).  
# В случае, если указанного уровня не существует (можно массив задать), необходимо вывести сообщение об ошибке.  
# Для чтения из файла использовать `exec` и `read`.  
# Также необходимо сообщать о каких-либо ошибках, связанных с чтением файла (файл не существует, нет доступа и т.д.)  
# Синтаксис команды должен выглядеть следующим образом: `script.sh -d <date> -t <time> -l <log_level> <file_name>`.  
# При запуске обязательно должен указываться хотя бы один из фильтров.  
# Вывод отфильтрованных строчек должен производиться в стандартный вывод (std_out).  
# Вывод ошибок - в поток ошибок (std_err).

# ./task_3.sh -l INFO -d "+2015-07-29" -t "19:25:07" Zookeeper_2k.log
# ./task_3.sh -l INFO -d "2015-07-29" -t "17:41:44" Zookeeper_2k.log
# ./task_3.sh -l WARN -d "-2017-07-30" Zookeeper_2k.log

echo "Starting task 3"
arr=()
func_parse_date () {
    text=$1
    text_from_args=$d_var
    if [[ ! -z "$d_var" ]]; then
        if [[ ${d_var:0:1} == '+' ]]; then
            text_slice=${d_var:1:11}
            if [[ $text > $text_slice ]]; then
                echo $@
                # exit 1
            fi 
        elif [[ ${d_var:0:1} == '-' ]]; then
            text_slice=${d_var:1:11}
            if [[ $text < $text_slice ]]; then
                echo $@
                # exit 1
            fi 
        else 
            if [[ $text == $text_from_args ]]; then
                echo $@
            fi
        fi
    fi
}


func_parse_time () {
    time_from_file=$2
    time_from_file_slise=${time_from_file:0:8}
    if [[ ! -z "$t_var" ]]; then
        if [[ $time_from_file_slise == $t_var ]]; then
            echo $@
        fi
    fi
}

func_parse_log_level () {
    if [[ ! -z "$l_var" ]]; then
        text=$4
        text_from_args=$l_var
        if [[ $text == $text_from_args ]]; then
            echo $@
        fi
    fi
}


func_wrapper(){
    text=$@
    result=$text
    if [[ ! -z "$t_var" ]]; then
        result="$(func_parse_time $result)"
    fi

    if [[ ! -z "$l_var" ]]; then
        result="$(func_parse_log_level $result)"
    fi

    if [[ ! -z "$d_var" ]]; then
        result="$(func_parse_date $result)"
    fi 

    if [[ ! -z $result ]]; then
        echo "$result"
    fi

    # result="$(func_parse_log_level $text)"
    # result="$(func_parse_date $result)"
    # # result="$(func_parse_time $result)"
    # echo "$result"
}


while getopts l:d:t: arg
do
    case $arg in 
    l )
        l_var=$OPTARG
        ;;
    d )
        d_var=$OPTARG
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

filename=$1

exec 3<&0 
exec 3<&-
exec < $filename

echo "Start"

while read p ; do
    func_wrapper $p
    # func_parse_time $p
    # func_parse_date $p  
done > Zookeeper_2k_result.log

exec 0<&3                 
exec 3<&-
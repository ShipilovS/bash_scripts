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
# ./task_3.sh -d "+2015-07-30" -d "2015-08-20"  Zookeeper_2k.log
# ./task_3.sh -d "+2015-07-30" -d "-2015-08-30"  -l INFO Zookeeper_2k.log
# ./task_3.sh -t "+17:41:43" -t "-18:12:00" Zookeeper_2k.log
# ./task_3.sh -t "+19:21:42" -t "-19:50:42" -d "+2015-07-28" -d "-2015-08-12" -l INFO Zookeeper_2k.log

echo "Starting task 3"
arr_d=()
arr_l=()
arr_t=()
func_parse_date () {
    text=$1
    text_from_args=$d_var
    if [[ ! -z "$d_var" ]]; then # если параметр один и не нулевой
        if [[ ${#arr_d[@]} -le "1" ]]; then # -le - <=
            if [[ ${d_var:0:1} == '+' ]]; then # если первый символ = '+'
                text_slice=${d_var:1:11} # берем текст с 1 позиции до конца
                if [[ $text > $text_slice ]]; then
                    echo $@ # выводим все
                fi 
            elif [[ ${d_var:0:1} == '-' ]]; then # если первый символ = '-'
                text_slice=${d_var:1:11}
                if [[ $text < $text_slice ]]; then
                    echo $@
                fi 
            else 
                if [[ $text == $text_from_args ]]; then
                    echo $@
                fi
            fi
        else # если параметра 2 (не более)
            for elem in ${arr_d[@]};
            do
                if [[ ${elem:0:1} == '+' ]]; then
                    text_slice_more=${elem:1:11} 
                elif [[ ${elem:0:1} == '-' ]]; then
                    text_slice_less=${elem:1:11}
                else 
                    if [[ $text == $elem ]]; then
                        echo $@
                    fi
                fi
            done
            if [[ ! -z $text_slice_more ]] && [[ ! -z $text_slice_less ]]; then # если текст больше даты(more) и меньше даты(less) ненулевые
                if [[ $text > $text_slice_more ]] && [[ $text < $text_slice_less ]]; then # && если датa more больше даты И(вместе считаем) меньше даты(less)
                    # echo " $text > $text_slice_more ---  $text < $text_slice_less"
                    echo $@
                fi 
            fi
        fi
    fi
}


func_parse_time () {
    text=$2
    text=${text:0:8}
    text_from_args=$t_var
    if [[ ! -z "$t_var" ]]; then # если параметр один
        if [[ ${#arr_t[@]} -le "1" ]]; then # -le - <=
            if [[ ${t_var:0:1} == '+' ]]; then
                text_slice=${t_var:1:8}
                if [[ $text > $text_slice ]]; then
                    echo $@
                fi 
            elif [[ ${t_var:0:1} == '-' ]]; then
                text_slice=${t_var:1:8}
                if [[ $text < $text_slice ]]; then
                    echo $@
                fi 
            else 
                if [[ $text == $text_from_args ]]; then
                    echo $@
                fi
            fi
        else # если параметра 2 (не более)
            for elem in ${arr_t[@]};
            do
                if [[ ${elem:0:1} == '+' ]]; then
                    text_slice_more=${elem:1:8} 
                elif [[ ${elem:0:1} == '-' ]]; then
                    text_slice_less=${elem:1:8}
                else 
                    if [[ $text == $elem ]]; then
                        echo $@
                    fi
                fi
            done
            if [[ ! -z $text_slice_more ]] && [[ ! -z $text_slice_less ]]; then
                if [[ $text > $text_slice_more ]] && [[ $text < $text_slice_less ]]; then # &&
                    echo $@
                fi 
            fi
        fi
    fi
}

func_parse_log_level () {
    if [[ ! -z "$l_var" ]]; then
        text=$4
        text_from_args=$l_var
        for elem in ${arr_l[@]};
        do
            if [[ $text == $elem ]]; then
                echo $@
            fi
        done
    fi
}


func_wrapper(){
    text=$@
    result=$text
    if [[ ! -z "$t_var" ]]; then # если параметры входные не нулевые, то добавлять к получившемуся result запросы
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
        arr_l+=($l_var)
        ;;
    d )
        d_var=$OPTARG
        arr_d+=($d_var)
        ;;
    t )
        t_var=$OPTARG
        arr_t+=($t_var)
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

# exec 0<&3                 
# exec 3<&-



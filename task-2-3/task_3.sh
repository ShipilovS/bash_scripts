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

# ./task_3.sh -l INFO -d "+2015-07-29" -t "17:41:44" Zookeeper_2k.log


echo "Starting task 3"
arr=()

func_parse_date () {
    text=$4
    text_from_args=$l_var
    if [[ $text == $text_from_args ]]; then
        echo $@
    fi
}


func_parse_time () {
  func_result="some result"
  text = 
}

func_parse_log_level () {
  func_result="some result"
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
# exec 4<$filename

# func_parse_date "WARN" $l_var

echo "Start"

while read  p ; do
  func_parse_date $p $l_var
done < $filename

echo "end"
# n=1
# while read line; do
#   echo "Line No. $n : $line"
#   n=$((n+1))
# done < $filename

# echo $arr

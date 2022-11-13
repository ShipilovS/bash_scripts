#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>


// ./task4 -n 11 -t 3 -- Hello

#define LEN 50


int main(int argc, char *argv[])
{
    int opt;
    int nsecs;
    double tsecs = -1;
    char *text;

    struct option longopts[] = {
        {
        .name = "number",
        .has_arg = required_argument,
        .flag = NULL,
        .val = 'n'
        },
        {
        .name = "timeout",
        .has_arg = required_argument,
        .flag = NULL,
        .val = 't'
        }
    };

    while ((opt = getopt_long(argc, argv, "n:t:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'n':
            nsecs = atoi(optarg);
            break;
        case 't':
            tsecs = atof(optarg);
            break;
        default:
            fprintf(stderr, "(usage: prntxt -n|--number <N> [-t|--timeout [<T>]] -- <TEXT>)");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "--") == 0)
        {
            text = argv[i+1]; 
        }
    }

    if(text != NULL){
        for(int i = 0; i < nsecs; i++){
            printf("%s\n", text);
            if(tsecs > 0){
                sleep(tsecs);
            }
            else{
                sleep(1);
            }
        }
    }
    else
    {
        fprintf(stderr, "(usage: prntxt -n|--number <N> [-t|--timeout [<T>]] -- <TEXT>)");
        exit(EXIT_FAILURE);
    }
}
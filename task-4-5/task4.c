
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// ./task4 -n 11 -t 3 -- Hello

#define LEN 50


int main(int argc, char *argv[])
{
    int flags, opt;
    int nsecs, tfnd;
    double tsecs;
    char *text;

    nsecs = 0;
    tfnd = 0;
    flags = 0;
    while ((opt = getopt(argc, argv, "n:t:")) != -1) {
        switch (opt) {
        case 'n':
            flags = 1;
            nsecs = atoi(optarg);
            break;
        case 't':
            tsecs = atof(optarg);
            tfnd = 1;
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
            sleep(tsecs);
        }
    }
    else
    {
        fprintf(stderr, "(usage: prntxt -n|--number <N> [-t|--timeout [<T>]] -- <TEXT>)");
        exit(EXIT_FAILURE);
    }
}
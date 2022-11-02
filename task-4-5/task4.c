
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int flags, opt;
    int nsecs, tfnd;
    double tsecs;

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
        default: /* '?' */
            fprintf(stderr, "(usage: prntxt -n|--number <N> [-t|--timeout [<T>]] -- <TEXT>)");
            exit(EXIT_FAILURE);
        }
    }

    printf("flags=%d; tfnd=%d; nsecs=%d; tsecs=%f; optind=%d\n",
            flags, tfnd, nsecs, tsecs, optind);


    /* Other code omitted */

    exit(EXIT_SUCCESS);
}
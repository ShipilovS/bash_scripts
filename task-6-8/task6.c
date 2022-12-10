// Создает дочерний процесс, который каждые 3 секунды посылает сигнал SIGUSR1 родительскому процессу  - есть
// Создает в текущем каталоге именованный канал с именем requests и открывает его для чтения - есть 
// Создает сигнальные дескрипторы для обработки сигналов SIGINT и SIGUSR1 вроде есть
// Используя poll ожидает в цикле на дескрипторах сигналов и канала есть
// При получении данных из именованного канала выводит данные на экран как текст есть
// При получении сигнала SIGUSR1 выводит имя полученного сигнала на экран есть
// При получении сигнала SIGINT завершает цикл ожидания есть
// После завершения цикла ожидания посылает сигнал SIGTERM дочернему процессу
// Ждет завершение дочернего процесса и выводит на экран его код завершения
// Удаляет ранее созданный именованный канал reqests и завершается

#include <signal.h> // sigemptyset, sigaddset, sigprocmask
#include <sys/signalfd.h> // signalfd
#include <sys/wait.h> // signalfd
#include <unistd.h> // alarm, read
#include <stdio.h> // printf
#include <poll.h> // poll
#include <string.h> // strsignal
#include <fcntl.h> // open
#include <stdlib.h>
volatile sig_atomic_t stop = 0;

pid_t pid;
int status = 0;

void sigint_handler(int sig){
  printf("Catched SIGINT\n");
  stop = 1;
}

void sigusr1_handler(int sig){
  printf("\nCatched SIGUSR1 \n");
}

void sigterm_handler(int sig){
  printf("SIGTERM handler\n");
  stop = 1;
}

int main()
{
    pid = fork();
    if(pid == 0){
        signal(SIGUSR1, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, sigterm_handler);

        while(!stop) {
            kill(0, SIGUSR1);
            sleep(3);
        }
        exit(0);
    }

    // создаем сигнальный файловый дескриптор для SIGALRM
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);
    sigaddset(&sig, SIGINT);
    sigaddset(&sig, SIGTERM);
    sigprocmask(SIG_BLOCK, &sig, NULL);
    int sigfd = signalfd(-1, &sig, 0);
    int pipefd = open("request", O_RDONLY);

    struct pollfd pfd[] =
    {
        {.fd = sigfd, .events = POLLIN },
        {.fd = pipefd, .events = POLLIN }
    };


    while (!stop)
    {
        poll(pfd, 2, -1);
        if ((pfd[0].revents & POLLIN) != 0)
        {
            struct signalfd_siginfo siginfo = {};
            read(sigfd, &siginfo, sizeof(siginfo));
            if(siginfo.ssi_signo == SIGINT) {
                printf("\nCatch SIGINT with\n");
                stop=1;
            } 
            else {
                printf("Catch with pid = %d - '%s'\n", pid, strsignal(siginfo.ssi_signo));
            }
        }
        if ((pfd[1].revents & POLLIN) != 0)
        {
            char buf[16];
            int n = read(pipefd, buf, sizeof(buf));
            if (n != 0)
            {
                printf("Text: %.*s", n, buf);
            }
        }
    }

    // close(pipefd);
    kill(0, SIGTERM);
    wait(&status);
    unlink("request");
    printf("Child  %d exit code: %d\n", getpid(), status);

    exit(0);
}
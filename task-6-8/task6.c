// Создает дочерний процесс, который каждые 3 секунды посылает сигнал SIGUSR1 родительскому процессу  - есть
// Создает в текущем каталоге именованный канал с именем requests и открывает его для чтения - есть 
// Создает сигнальные дескрипторы для обработки сигналов SIGINT и SIGUSR1 вроде есть
// Используя poll ожидает в цикле на дескрипторах сигналов и канала
// При получении данных из именованного канала выводит данные на экран как текст
// При получении сигнала SIGUSR1 выводит имя полученного сигнала на экран
// При получении сигнала SIGINT завершает цикл ожидания
// После завершения цикла ожидания посылает сигнал C дочернему процессу
// Ждет завершение дочернего процесса и выводит на экран его код завершения
// Удаляет ранее созданный именованный канал reqests и завершается

#include <signal.h> // sigemptyset, sigaddset, sigprocmask
#include <sys/signalfd.h> // signalfd
#include <unistd.h> // alarm, read
#include <stdio.h> // printf
#include <poll.h> // poll
#include <string.h> // strsignal
#include <fcntl.h> // open
#include <stdlib.h>

void sig_handler(int signo)
{
  if (signo == SIGINT){
    printf("\nSIGINT!!!\n");
    // exit(1);
    raise(SIGUSR1);
    signal(signo, SIG_DFL);
  }

  else if (signo == SIGUSR1){
    printf("\nSignal \"%s\" has been catched\n", strsignal(signo)); // выводим название сигнала
    // printf("Child : Received a signal from parent \n");
    // sleep(1);
    // kill(getppid(), SIGUSR1);
    // exit(1);
    signal(signo, SIG_DFL);
  }

}

int main()
{

    // создаем сигнальный файловый дескриптор для SIGALRM
    sigset_t sig;
    sigemptyset(&sig);
    signal(SIGINT, sig_handler);
    signal(SIGUSR1, sig_handler);
    sigaddset(&sig, SIGALRM);
    sigprocmask(SIG_BLOCK, &sig, NULL);
    int sigfd = signalfd(-1, &sig, 0);

    sigset_t bs, os, b2;
    sigemptyset(&bs);
    sigaddset(&bs, SIGUSR1);
    sigaddset(&bs, SIGUSR2);
    sigemptyset(&b2);
    sigaddset(&b2, SIGINT);
    sigaddset(&b2, SIGTERM);
    sigprocmask(0, NULL, &os);

    // открываем именованный канал с именем “pipe”
    // должен быть создан заранее в текущем каталоге программы командой mkfifo pipe
    int pipefd = open("request", O_RDONLY);

    alarm(3); // сигнал SIGALRM будет отправлен через 3 секунды

    // подготавливаем структуру для ожидания на файловых дескрипторах
    struct pollfd pfd[] =
    {
        {.fd = sigfd, .events = POLLIN },
        {.fd = pipefd, .events = POLLIN }
    };

    while (1)
    {
        // ждем данные в файловых дескрипторах
        poll(pfd, 2, -1);

        // получен сигнал
        if ((pfd[0].revents & POLLIN) != 0)
        {
            // считываем параметры сигнала
            struct signalfd_siginfo siginfo = {};
            read(sigfd, &siginfo, sizeof(siginfo));
            // выводим имя сигнала
            printf("Catched signal %s\n", strsignal(siginfo.ssi_signo));

            // новый сигнал SIGALRM будет отправлен через 3 секунды
            alarm(3);
        }
        // есть данные в именованном канале
        if ((pfd[1].revents & POLLIN) != 0)
        {

        // читаем данные из канала блоками до 16 байт и выводим на экран
            char buf[16];
            int n = read(pipefd, buf, sizeof(buf));
            if (n != 0)
            {
                printf("%.*s", n, buf);
            }
        }
    }

    return 0;
}
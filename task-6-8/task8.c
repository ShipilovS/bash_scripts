#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/signalfd.h>
#include <poll.h>
#include <sys/msg.h>
#include <time.h>

#define NO_MSG_BEBUG1

const int MESSAGE_SIZE = sizeof(int) * 2;
const int COUNT = 5;

// Структура данных для сообщения.
struct messageRequest {
    long mtype;
    int id;
    int newState;
};

// Id очереди сообщений.
int messageQueueId;

// Id семафора.
int semaphoreId;

// Возвращает название состояния.
const char* getStateString(int state) {
    switch (state) {
    case 0:
        return "думает";

    case 1:
        return "голодает";

    case 2:
        return "ест";
           
    default:
        return "unknown-state";
    }
}

// Возвращает рандомное время.
int getRandomTime() {
    const int upper = 10;
    const int lower = 3;
    return (rand() % (upper - lower + 1)) + lower;
}

// Инициализирует семафоры.
void initializeSemaphores(int create) {
    key_t key = ftok("sem666", 1);
    if(create){
        semaphoreId = semget(key, COUNT, IPC_CREAT | 0600);
        unsigned short values[] = { 1, 1, 1 , 1, 1};
        semctl(semaphoreId, 0, SETALL, values);
    } else {
        semaphoreId = semget(key, 0, 0);
    }
}

// Инициализирует очередь сообщений.
void initializeMessageQueue(int create) {
    key_t msgkey = ftok("msg666", 1);
    if(create) {
        messageQueueId = msgget(msgkey, IPC_CREAT | 0660);
    } else {
        messageQueueId = msgget(msgkey, 0);
    }
}

// Захватывает семафоры.
void lockSemaphores(int id) {
    int left = id - 1;
    int right = id % COUNT;

    struct sembuf ops[] = {
        {.sem_num = left, .sem_op = -1 },
        {.sem_num = right, .sem_op = -1 }
    };
    semop(semaphoreId, ops, 2);
}

// Освобождает семафоры.
void unlockSemaphores(int id) {
    int left = id - 1;
    int right = id % COUNT;

    struct sembuf ops[] = {
        {.sem_num = left, .sem_op = 1 },
        {.sem_num = right, .sem_op = 1 }
    };
    semop(semaphoreId, ops, 2);
}

// Отправляет сообщение в очередь.
void sendMessage(int id, int newState) {
#ifndef NO_MSG_BEBUG
    struct messageRequest request = {.mtype=1, .id=id, .newState=newState };
    msgsnd(messageQueueId, &request, MESSAGE_SIZE, 0);
#else
    printf("[Дебаг] Философ %d теперь %s\n", id, getStateString(newState));
#endif
}

// Устанавливает состояние.
void setState(int id, int state) {
    sendMessage(id, state);
}

// Процесс мышления фелософа.
void subprocessThinking(int id) {
    setState(id, 0);
    int delay = getRandomTime() * 1000;
    clock_t startTime = clock();
    while(clock() - startTime < delay) { }
}

// Процесс еды фелософа.
void subprocessEat(int id) {
    setState(id, 2);
    int delay = getRandomTime();
    sleep(delay);
}

// Процесс голодания и еды.
void subprocessTryEat(int id) {
    setState(id, 1);
    lockSemaphores(id);
    subprocessEat(id);
    unlockSemaphores(id);
}

// Процесс фелософа.
void subprocess(int id) {
    srand(id);

    //initializeSemaphores(0);
    //initializeMessageQueue(0);
    //printf("[%ld] subproc\n", id);

    int currentState = 0;
    while(1) {
        subprocessThinking(id);
        subprocessTryEat(id);
    }   
}

// Процесс супервизор.
void mainprocess() {
    struct messageRequest request;
    while(1) {
#ifndef NO_MSG_BEBUG
        int n = msgrcv(messageQueueId, &request, MESSAGE_SIZE, 1, 0);
        if(n == -1) {
            perror("msgrcv");
        } else if(n > 0) {
            int senderId = request.id;
            int newState = request.newState;
            printf("Философ %d теперь %s\n", senderId, getStateString(newState));
        }
#endif
    }

    wait(NULL);
    printf("Finished\n");
}

int main() {
    initializeSemaphores(1);
    initializeMessageQueue(1);

    for(int id = 1; id <= 5; id++) {
        if(fork() == 0) {
            subprocess(id);
            exit(0);
        }
    }

    mainprocess();
}
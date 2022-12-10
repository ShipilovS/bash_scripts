#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>

struct sembuf acquire_sem = { .sem_num = 0, .sem_op = -1 };
struct sembuf release_sem = { .sem_num = 0, .sem_op = 1 };

int main()
{
  key_t key = ftok("sem11", 1);
  int semid = semget(key, 1, IPC_CREAT | 0600);
  semctl(semid, 0, SETVAL, 1); 

	pid_t pid = fork();
	if (pid == -1)
	{
	  perror("fork");
	  return 1;
	}
	else if (!pid) {
    semid = semget(ftok("sem11", 1), 0, 0);

	  while (1)
	  {
      semop(semid, &acquire_sem, 1);
	    for (int i = 0; i < 10; i++)
	    {
	      putchar('C'); fflush(stdout);
	   	  usleep(10000 /* us */); 
	    }
	    putchar('\n');
      semop(semid, &release_sem, 1);
	  }
	}
	
  semid = semget(ftok("sem11", 1), 0, 0);

  while (1)
  {
    semop(semid, &acquire_sem, 1);
    for (int i = 0; i < 10; i++)
    {
      putchar('P'); fflush(stdout);
   	  usleep(10000 /* us */); 
    }
    putchar('\n');
    semop(semid, &release_sem, 1);
  }
  
  return 0;
}
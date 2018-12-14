#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#define KEY 0xDEADBEEF
#define MAX_LINE 256


int main() {
  int semd, shmid, file;
  int * len_of_last;
  semd = semget(KEY, 1, 0);

  //DOWNING
  struct sembuf sb;
  struct stat   fs;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);

  printf("got the semaphore!\n");

  if ((shmid = shmget(KEY, MAX_LINE, 0644)) == -1) {
      perror("shmget");
      exit(1);
  }
  printf("obtained shmid: %i\n", shmid);
  len_of_last = shmat(shmid, NULL, 0); //should get len on last line
  if(len_of_last == (void *)-1)
      perror("shmat");
  if (file = open("story", O_RDWR | O_APPEND) == -1) {
      perror("open");
      exit(1);
  }
	
  //BROKEN BOI probably the lseek
  if(*len_of_last == 0) {
      printf("first line\n");
  }
  else {
      if(fstat(file, &fs))
          perror("fstat");
	  if(lseek(file, fs.st_size - *len_of_last, SEEK_SET) == -1)
          perror("lseek");
	  char line[MAX_LINE];
	  if (read(file, line, MAX_LINE) == -1) {
		  perror("read");
		  exit(1);
	  }
	  printf("Last line:\n %s", line);
  }
  
  printf("\nEnter next line:\n");
  char input[MAX_LINE];
  fgets(input, MAX_LINE, stdin);

  //updating shared mem
  *len_of_last = strlen(input);

  //writing to file
  if(write(file, input, *len_of_last) == -1) {
      perror("write");
      exit(1);
  }

  //releasing semaphore
  //UPPING
  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}


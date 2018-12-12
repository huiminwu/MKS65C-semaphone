#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 0xDEADBEEF

 union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO */
 };

int main(int argc, char *argv[]) {

  int semd;
  int r;
  int v;

  if(argc > 1) {
    if(!strcmp(argv[1], "-c") {
        semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
          if (semd == -1) {
            printf("error %d: %s\n", errno, strerror(errno));
            semd = semget(KEY, 1, 0);
            v = semctl(semd, 0, GETVAL, 0);
            printf("semctl returned: %d\n", v);
          }

    } else if (!strcmp(argv[1], "-r") {
        //remove
    } else if (!strcmp(argv[1], "-v") {
        //view
    }   
    else {
    union semun us;
    us.val = 3;
    r = semctl(semd, 0, SETVAL, us);
    printf("semctl returned: %d\n", r);
  }

  return 0;
}

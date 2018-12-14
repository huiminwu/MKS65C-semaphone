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

 union semun {
     int              val;    /* Value for SETVAL */
     struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
     unsigned short  *array;  /* Array for GETALL, SETALL */
     struct seminfo  *__buf;  /* Buffer for IPC_INFO */
 };

int main(int argc, char *argv[]) {
    char story_buff[2000];
    int semd, shmid, file;
    int r;
    int v;

    if(argc > 1) {
        if(!strcmp(argv[1], "-c")) {
            semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
            if (semd == -1) {
                printf("error %d: %s\n", errno, strerror(errno));
                semd = semget(KEY, 1, 0);
                v = semctl(semd, 0, GETVAL, 0);
                printf("existing semval: %d\n", v);
                union semun us;
                us.val = 3;
                printf("setting semval to 3\n");
                r = semctl(semd, 0, SETVAL, us);
                printf("semctl returned: %d\n", r);
            } 
            else {
                printf("creating new sem with semval 3\n");
                union semun us;
                us.val = 3;
                r = semctl(semd, 0, SETVAL, us);
                printf("semctl returned: %d\n", r);
            }
            if((shmid = shmget(KEY, MAX_LINE, IPC_CREAT | 0644 )) == -1) {
                perror("shmget");
                exit(1);
            }
            printf("Created shared memory with shmid %i\n", shmid);
            
            if((file = open("story", O_TRUNC | O_RDONLY | O_CREAT, 0666)) == -1) {
                perror("open");
                exit(1);
            }
            printf("Created story file (at fd %i)\n", file);
            if(close(file) == -1)
                perror("close");
        } else if (!strcmp(argv[1], "-r")) {
            //remove
            char story_buff[2000];
            if((file = open("story", O_RDONLY) == -1)) {
                perror("open");
                exit(1);
            }
            else {
                printf("Story:\n");
                read(file, story_buff, 1999); //cause last is null
                printf("%s\n", story_buff);
                close(file);
            }
            if((shmid = shmget(0, MAX_LINE, 0644 )) == -1) {
                perror("shmget");
            }
            else { 
                shmctl(shmid, IPC_RMID, NULL);
            }
            if(remove("story") == -1) {
                perror("rm");
            }
            if((semd = semget(KEY, 1, 0644)) == -1) {
                perror("semget");
            }
            else {
                semctl(semd, 0, IPC_RMID);
            }
        } else if (!strcmp(argv[1], "-v")) {
            //view
            if((file = open("story", O_RDONLY, 0644) == -1)) {
                perror("open");
                exit(1);
            }
            else {
                printf("Story:\n");
                if(read(file, story_buff, 2000) == -1) {
                    perror("read");
                    exit(1);
                }
                else {
                    printf("%s\n", story_buff);
                }
            }
        } else {
            printf("wrong command line argument!\nuse -c, -r, or -v!\n");
        }
    }

    return 0;
}

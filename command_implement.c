/* Implement command   "  ps -el|grep pts | wc -l  " 
 * using linux pipe ipc mechanism and exce systtem call 
 * */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) 
    {
        perror("pipe");
        exit(0);
    }

    if (fork() == 0) 
    {
       	close(pipe1[0]);
        dup2(pipe1[1], 1);
       
        execlp("ps", "ps", "-el", NULL);
    }

    if (fork() == 0) 
    {  
	close(pipe1[1]);
        dup2(pipe1[0], 0);

	close(pipe2[0]);
        dup2(pipe2[1], 1);

        execlp("grep", "grep", "pts", NULL);
    }

    close(pipe1[0]);
    close(pipe1[1]);

    if (fork() == 0) 
    {
	close(pipe2[1]);
        dup2(pipe2[0],0);
        execlp("wc", "wc", "-l", NULL);
    }
    close(pipe2[0]);
    close(pipe2[1]);

    pid_t p;
    int status;
    while(p=wait(&status) > 0);
    return 0;
}

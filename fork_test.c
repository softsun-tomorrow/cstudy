#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int count = 0;
    int pid;
    pid = fork();
    if(pid < 0){
        printf("error in fork!");
        exit(1);
    }
    else if(pid == 0){
        printf("I am the child process, the count is %d, my process ID %d\n", count, getpid());
    }
    else{
        printf("I am the parent process, the count is %d, my process ID %d\n", ++count, getpid());
        return 0;
    }
}

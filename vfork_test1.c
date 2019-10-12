#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int count = 0;
    int child;
    printf("Before create son, the father's count is:%d\n", count);
    child = vfork();
    if(child < 0){
        printf("error in vfork!");
        exit(1);
    }
    else if(child == 0){
        printf("This is son, his pid is:%d and the count is:%d\n", getpid(), ++count);
        exit(1);
    }
    else{
        printf("After son, This is father, his pid is:%d and the count is:%d and the child is:%d\n", getpid(), count, child);
        return 0;
    }
}

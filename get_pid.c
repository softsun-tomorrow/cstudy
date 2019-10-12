#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

main()
{
    printf("The current process ID is %d\n", getpid());
}

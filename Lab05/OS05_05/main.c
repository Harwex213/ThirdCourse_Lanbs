#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(int argc, char **argv)
{
    for(int i = 0; i < 100000; i++)
    {
        printf("pid: %d priority: %d\n", getpid(), getpriority(PRIO_PROCESS, 0));
        sleep(1);
    }
}
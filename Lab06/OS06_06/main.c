#include <stdio.h>
#include <unistd.h>
#define COUNT 10000000

int main(int argc, char **argv)
{
    for(int i = 0; i < COUNT; i++)
    {
        printf("pid: %d \n", getpid());
        sleep(1);
    }
}

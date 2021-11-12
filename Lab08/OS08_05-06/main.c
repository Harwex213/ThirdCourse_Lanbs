#include <stdio.h>
#include <unistd.h>
#include <time.h>

void printTime()
{
    time_t t = 0;
    t = time(&t);
    struct tm *tmptr;
    tmptr = localtime(&t);
    printf("%d.%d.%d %d:%d:%d\n", tmptr->tm_mday, tmptr->tm_mon+1, tmptr->tm_year+1900, tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
}

int main()
{
    printTime();

    int i = 0;
    clock_t cl1 = clock();
    clock_t cl2;
    while(1)
    {
        i++;
        cl2 = clock();
        if((cl2 - cl1)/CLOCKS_PER_SEC == 2)
            break;
    }

    printf("end after 2 sec: %d\n", i);
}

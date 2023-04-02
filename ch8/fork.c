#include "csapp.h"
#include "csapp.c"
int main()
{
    pid_t  pid;
    int x=1;
    pid=Fork();
    if(pid==0){
        printf("child: x=%d",++x);
        exit(0);
    }
    printf("parent: x=%d\n",--x);
    exit(0);
}
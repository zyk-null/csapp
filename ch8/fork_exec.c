#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int pid;
	pid=fork();
	if(pid<0){
		fprintf(stderr,"fork failed!");
	}else if(pid==0){
		execlp("/bin/ls","ls",NULL);
	}else{
		wait(NULL);
		printf("child completed!");
		exit(0);
	}
}

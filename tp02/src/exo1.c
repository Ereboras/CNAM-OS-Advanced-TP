#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	int f;
	int status;
	f = fork();

	if(f==0){

		int pid = getpid();
		int ppid = getppid();
		printf("pid fils (depuis fils): %d\n", pid);
		printf("pid père (depuis fils): %d\n", ppid);

		int reste = pid % 10;
		exit(reste);
	}
	else{

		int pid = getpid();
		printf("pid père (depuis père): %d\n", pid);

		wait(&status);

		int test = WEXITSTATUS(status);
		printf("code retour fils (depuis père): %d\n", test);
	}


	pid_t getpid(void);
	pid_t getppid(void);

}

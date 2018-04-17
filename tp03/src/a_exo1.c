#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int i;

// On SIGINT signal (Ctrl + C)
void event() {
	i++;
	printf("Compteur de kill: %d\n", i);
	if(i == 10) {
		printf("You're killing it boy !\n");
	}
}

// On SIGTERM signal (kill 2563 process-pid)
void exitEvent() {
	printf("That's all for today.");
	exit(1);
}

int main(){
	signal(SIGINT,event);
	signal(SIGTERM,exitEvent);
	while(1) {}
}

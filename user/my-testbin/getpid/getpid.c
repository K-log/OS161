#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid = getpid();
	printf("My pid is: %d\n", pid);
	return 0; 
}



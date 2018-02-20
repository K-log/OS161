/*
 * fork.c - fork stub system call test 
 */
#include <unistd.h>
#include <stdio.h>

 int
 main()
{
	pid_t pid;
   printf("Test call for fork stub\n");
   pid = fork();
   printf("pid: %d\n", pid);

	printf("Test call for get_pid stub\n");
	pid = getpid();
	printf("pid: %d\n", pid);

	printf("Test call for wait_pid stub\n");
	pid = waitpid(pid, 0, 0);
	printf("pid: %d\n", pid);

   return 0; /* avoid compiler warnings */
}

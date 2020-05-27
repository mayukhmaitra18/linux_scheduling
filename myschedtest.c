// User Space Program
// myschedtest.c
//

#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
int main(){
	const struct sched_param param = {
		.sched_priority = 0,
	};
	sched_setscheduler(getpid(), 7 /*policy*/, &param);

	int i;
	for(i = 0; i < 10; i++)
		if (fork() == 0){
			sleep(0);
			printf("child pid: %d\n", getpid());
			return 0;
		}
	printf("parent pid: %d\n", getpid());
	for(i = 0; i < 10; i++)
		wait(NULL);
	
	return 0;
}

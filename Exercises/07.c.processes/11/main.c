#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>

int main(int argc, char * argv[]){


	int counter_correct = 0;
	int counter_incorrect = 0;

	for(int i = 1; i < argc; i++){
		
		int pid = fork();

		if (pid == 0){
			
			if(execl(argv[i], argv[i], (char *) NULL) == -1){
				err(1, "Error in %s!", argv[i]);
			}
		}

		int status;
		wait(&status);


		if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
			++counter_correct;
		}
		else {
			++counter_incorrect;		
		}
	}
	
	dprintf("Successfully executed: %d\n", counter_correct);
	dprintf("Unsuccessfully executed: %d\n", counter_incorrect);

	exit(0);
}

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>

int main(int argc, char * argv[]){

	if (argc != 3){
		errx(1, "Two arguments needed!");
	}


	int pid = fork();

	if (pid == 0){
			
		if(execl(argv[1], argv[1], (char *) NULL) == -1){
			err(1, "Error in %s!", argv[1]);
		}
	}	
	
	int status;
	wait(&status);


	if (WIFEXITED(status) && WEXITSTATUS(status) == 0){

		int pid2 = fork();

		if (pid2 == 0){

			if(execl(argv[2], argv[2], (char *) NULL) == -1){
				err(1, "Error in %s!", argv[2]);
			}

		}

	}
	else {
		exit(42);
	}

	exit(0);
}

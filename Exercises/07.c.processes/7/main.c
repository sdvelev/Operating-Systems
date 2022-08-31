#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>

int main(int argc, char * argv[]){

	if (argc != 4){
		errx(1, "Three arguments needed!");
	}

	int status;

	//printf("Current process: %d\n", getpid());

	for(int i = 1; i < argc; i++){
		
		pid_t pid = fork();	

		if (pid > 0){
			pid_t ch_pid = wait(&status);
		
			if (WIFEXITED(status)){
				int ex_status = WEXITSTATUS(status);
				dprintf(1, "Child process: %d, Exit status: %d\n", ch_pid, ex_status);	
			}
			else {
				errx(2, "No exit status! Child 	%d did not terminate correctly!", ch_pid);
			}
		}
		else {
			
			int fd = open("/dev/null", O_WRONLY);
			close(1);
			dup(fd);
			
			if (execl(argv[i], argv[i], (char *) NULL) == -1){
				err(3, "Error in execution!");
			}
		}	
	}

	exit(0);
}

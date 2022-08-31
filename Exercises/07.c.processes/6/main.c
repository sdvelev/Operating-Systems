#include<err.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char * argv[]){

	if (argc != 2){
		errx(1,"One argument expected!");
	}

	int pid = fork();

	int status;

	if (pid > 0){
		wait(&status);

		if (WIFEXITED(status)){
			
			if(WEXITSTATUS(status) != 0){
				errx(2, "Child did not terminate correctly");
			}
			else {
			
				write(1, argv[1], sizeof(argv[1]) + 1);
				write(1, "\n", 1);
				//printf("%s\n", argv[1]);
			}
		}
	}
	else {
	
		int fd = open("/dev/null", O_WRONLY);
		close(1);
		dup(fd);

		if(execl(argv[1], argv[1], (char *) NULL) == -1){
			err(3, "Error in execution!");
		}
		
		err(1, "Error");
		
	}

	
}

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>
#include<string.h>

int main(int argc, char * argv[]){
	
	if (argc != 3){
		errx(1, "Two arguments needed!");
	}

	int pid1 = fork();
	if (pid1 == -1){
		err(3, "Unable to fork!");
	}


	if (pid1 == 0){
		
		if(execl(argv[1], argv[1], (char *) NULL) == -1){
			err(1, "Error in %s!", argv[1]);
		}
	}

	int pid2 = fork();
	if (pid2 == -1){
		err(3, "Unable to fork!");
	}


	if (pid2 == 0){
		
		if(execl(argv[2], argv[2], (char *) NULL) == -1){
			err(1, "Error in %s!", argv[2]);
		}
	}

	printf("Child 1/Command 1/ PID: %d\n", pid1);
	printf("Child 2/Command 2/ PID: %d\n", pid2);

	int status1;
	
	int pid_first = wait(&status1);
	if (pid_first == -1){
		err(4, "Error in wait!");
	}

	if(WIFEXITED(status1) && WEXITSTATUS(status1) == 0){
		printf("%d\n", pid_first);
	}
	else {
		int status2;
		int pid_second = wait(&status2);
		if (pid_second == -1){
			err(4, "Error in wait!");
		}
				
		if(WIFEXITED(status2) && WEXITSTATUS(status2) == 0){
			printf("%d\n", pid_second);
		}
		else {
			printf("-1\n");
		}
	}

	exit(0);
}

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char * argv[]){

	if (argc != 4){
		errx(1, "Three arguments needed!");
	}


	int fd = open(argv[3], O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	if (fd == -1){
		err(2, "Ërror in opening %s!", argv[3]);
	}


	for(int i = 1; i <=2; i++){
		int pid1 = fork();
		if (pid1 == -1){
			err(3, "Unable to fork!");
		}


		if (pid1 == 0){
			
			if(execl(argv[i], argv[i], (char *) NULL) == -1){
				err(1, "Error in %s!", argv[i]);
			}
		}

		int status1;
		if (wait(&status1) == -1){
			err(4, "Unable to wait!");
		}

		if (WIFEXITED(status1) && WEXITSTATUS(status1) == 0){

			if(write(fd, argv[i], strlen(argv[i])) != (ssize_t) strlen(argv[i])){
				err(5, "Error in writing!");
			}
			if(write(fd, "\n", 1) != 1){
				err(5, "Error in writing!");
			}
		}
		else {
			warnx("Child %d did not terminate correctly or exit status differ from 0!", i);
		}	
	}

	close(fd);
	exit(0);
}

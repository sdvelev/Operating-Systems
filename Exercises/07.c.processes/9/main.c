#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>

int main(int argc, char * argv[]){

	if (argc != 2){
		errx(1, "One arguments needed!");
	}


	int fd = open(argv[1], O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if (fd == -1){
		err(4, "Error in opening %s!", argv[1]);
	}

	int pid = fork();

	if (pid == 0){
			
		ssize_t write_size;

		if ((write_size = write(fd, "foo", 3)) != 3){
			if (write_size == -1){
				err(5, "Error in writing");
			}
			else{
				err(6, "Not enough bytes written!");
			}
		}
		close(fd);	
		exit(0);
	}	
	
	int status;
	wait(&status);

	int pid2 = fork();
	
	if (pid2 == 0){
		
		ssize_t write_size;

		if ((write_size = write(fd, "bar", 3)) != 3){
			if (write_size == -1){
				err(5, "Error in writing");
			}
			else{
				err(6, "Not enough bytes written!");
			}
		}
		close(fd);	
		exit(0);
	}	
	
	close(fd);
	exit(0);
}

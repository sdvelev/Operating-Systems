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

	int status;

	//printf("Current process: %d\n", getpid());

	
	pid_t pid = fork();	


	int fd = open(argv[1], O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if (fd == -1){
		err(4, "Error in opening %s!", argv[1]);
	}

	if (pid > 0){
		wait(&status);
		
		lseek(fd, 0, SEEK_SET);

		ssize_t read_size;
		char c;
		int counter = 0;

		char * coll = malloc(12);
		if (coll == NULL){
			err(7, "Cannot allocate memory!");
		}
		
		while ((read_size = read(fd, &c, 1)) != 0){
			
			if (read_size == -1){
				err(5, "Error in reading!");
			}

		coll[counter++] = c;
		coll[counter++] = ' ';
		}

		coll[--counter] = '\0';
		write(1, &coll, sizeof(coll));
		//printf("%s", coll);	
		free(coll);
	}
	else {
			
		ssize_t write_size;

		if ((write_size = write(fd, "foobar", 6)) != 6){
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

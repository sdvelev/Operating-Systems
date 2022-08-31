#include<unistd.h>
#include<sys/types.h>
#include<time.h>
#include<err.h>
#include<fcntl.h>
#include<time.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<stdint.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char * argv[]){
	if (argc < 3){
		errx(1, "Usage:At least two arguments needed!");
	}

	if (argv[1][0] < '1' || argv[1][0] > '9'){
		errx(2, "First argument must be between 1 and 9! ");
	}

	int l = argv[1][0] - 0x30;

	int fd = open("run.log", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1){
		err(3, "Unable to open run.log");
	}

	int counter = 0;

	while(1){
		
		if (counter == 2){
			exit(0);
		}		

		int pf[2];
		if (pipe(pf) == -1){
			err(4, "Error in creating pipe!");
		}	

		int pid = fork();
		if (pid == -1){
			err(5, "Error in fork!");
		}

		if (pid == 0){
			close(pf[0]);
			
			uint64_t cur_time = time(NULL);
			ssize_t write_size;
			if ((write_size = write(pf[1], &cur_time, sizeof(cur_time))) != sizeof(cur_time)){
				if (write_size == -1){
					err(6, "Error in writing in pipe!");
				}
				else if (write_size < (uint64_t) sizeof(cur_time)){
					err(7, "Unable to read enough characters!");
				}
			}
			
			if (execvp(argv[2], argv + 2) == -1){
				err(8, "Error in execvp!");
			}
		}	

		close(pf[1]);
		int status;
		if(wait(&status) == -1){
			err(9, "Error in wait!");
		}
		time_t finish_time = time(NULL);
		uint64_t exit_status;
		if (!WIFEXITED(status)){
			exit_status = 129;
		}
		else {
			exit_status = WEXITSTATUS(status);
		}
		
		time_t start_time;
		ssize_t read_size;
		if ((read_size = read(pf[0], &start_time, sizeof(start_time))) != sizeof(start_time)){
			if (read_size == -1){
				err(10, "Error in reading from pipe!");
			}
			else if (read_size < (uint64_t) sizeof(start_time)){
				err(11, "Unable to read enough bytes from pipe!");
			}
		}

		ssize_t write_size;

		
		dprintf(fd, "%ld", start_time);
		exit(0);

		//char * st = malloc(strlen(start_time));

		//snprintf(st, strlen(start_time), "%ld", start_time);


		if ((write_size = write(fd, &start_time, sizeof(start_time))) != sizeof(start_time)){
			if (write_size == -1){
				err(12, "Error in writing to run.log!");
			}
			else if (write_size < sizeof(start_time)){
				err(13, "Unable to write enough characters to run.log!");
			}
		}

		char c = ' ';

		if ((write_size = write(fd, &c, sizeof(c))) == -1 || write_size == 0){
			err(12, "Error in writing to run.log!");
		}

		if ((write_size = write(fd, &finish_time, sizeof(finish_time))) != sizeof(finish_time)){
			if (write_size == -1){
				err(12, "Error in writing to run.log!");
			}
			else if (write_size < sizeof(finish_time)){
				err(13, "Unable to write enough characters to run.log!");
			}
		}

		if ((write_size = write(fd, &c, sizeof(c))) == -1 || write_size == 0){
			err(12, "Error in writing to run.log!");
		}

		if ((write_size = write(fd, &exit_status, sizeof(exit_status))) != sizeof(exit_status)){
			if (write_size == -1){
				err(12, "Error in writing to run.log!");
			}
			else if (write_size < sizeof(exit_status)){
				err(13, "Unable to write enough characters to run.log!");
			}
		}

		c = '\n';

		if ((write_size = write(fd, &c, sizeof(c))) == -1 || write_size == 0){
			err(12, "Error in writing to run.log!");
		}

		if (exit_status != 0 && (finish_time - start_time) < l){
			++counter;
		}
		else {
			counter = 0;
		}
		
		//free(st);

		close(pf[0]);
	}
	
}

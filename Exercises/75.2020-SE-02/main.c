#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<err.h>
#include<stdint.h>
#include<unistd.h>


int main(int argc, char * argv[]){
	
	if (argc != 3){
		errx(1, "Usage: Two arguments needed!");	
	}	

	int pf[2];

	if (pipe(pf) == -1){
		err(2, "Unable to fork!");
	}

	int pid = fork();
	if (pid == -1){
		err(3, "Unable to fork!");
	}

	if (pid == 0){
		close(pf[0]);
		if (dup2(pf[1], 1) == -1){
			err(4, "Error in dup2!");
		}
		if (execlp("cat", "cat", argv[1], (char *) NULL) == -1){
			err(5, "Error in execlp!");
		}
	}

	close(pf[1]);

	int fd = open(argv[2], O_CREAT|O_TRUNC|O_RDONLY|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if (fd == -1){
		err(6, "Error in opening %s!",argv[2]);
	}

	char c;
	ssize_t read_size;

	while((read_size = read(pf[0], &c, sizeof(c))) > 0){
		if (c == 0x7D){
			if ((read_size = read(pf[0], &c, sizeof(c)) ) == -1){
				err(7, "Error in reading from %s!", argv[1]);
			}
			c = c ^ 0x20;
			if (write(fd, &c, 1) != 1){
				err(8, "Error in writing in %s!",argv[2]);
			}
		}
		else if(c == 0x55){
			continue;
		}
	}

	if (read_size == -1){
		err(7, "Error in reading from %s!", argv[1]);
	}

	close(pf[0]);
	close(fd);
	exit(0);
}


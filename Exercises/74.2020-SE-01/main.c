#include<sys/types.h>
#include<unistd.h>
#include<stdint.h>
#include<fcntl.h>
#include<err.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc, char * argv[]){
	
	if (argc != 2){
		errx(2, "Usage: One argument needed!");
	}

	if (mkfifo("np", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) == -1){
		err(1, "Unable to create named pipe!");
	}
	
	int fd = open("np", O_WRONLY);
	if (fd == -1){
		err(3, "Unable to write in named pipe!");
	}

	if(dup2(fd, 1) == -1){
		err(5, "Error in dup2!");
	}
	
	if (execlp("cat", "cat", argv[1], (char *) NULL) == -1){
		err(4, "Unable to execlp!");
	}
	exit(0);
}

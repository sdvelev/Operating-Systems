#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdint.h>
#include<stdlib.h>
#include<err.h>

int main(int argc, char * argv[]){

	if (argc != 2){
		errx(1, "Usage: 1 argument needed!");
	}

	int fd = open("np", O_RDONLY);
	if (fd == -1){
		err(2, "Error in opening named pipe!");
	}

	if (dup2(fd, 0) == -1){
		err(3, "Error in dup!");
	}

	if(execlp(argv[1], argv[1], (char *) NULL) == -1){
		err(4, "Error in execlp!");	
	}

	exit(0);
}

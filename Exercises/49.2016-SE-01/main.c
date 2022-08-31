#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main (int argc, char* argv []){

	if (argc != 2){
		errx(1, "One argument is required!");
	}


	int fd = open (argv[1], O_RDWR);
	if (fd == -1){
		errx(2,"Unable to open %s!", argv[1]);
	}

	uint8_t num;

	uint8_t countarr[256] = {0};

	ssize_t read_size;

	while((read_size = read(fd, &num, sizeof(uint8_t)))){
		if (read_size == -1 || read_size != sizeof(uint8_t)){
			close(fd);
			errx(3, "Error in reading fron %s!", argv[1]);
		}


		++countarr[num];
	}


	lseek(fd, 0, SEEK_SET);

	uint8_t i = 0;
		
	for(i = 0; i < 255; i++){
		int j;
		for(j = 0; j < countarr[i]; j++){
			if(write(fd, &i, sizeof(uint8_t)) != sizeof(uint8_t)){
				close(fd);
				errx(4,"Error in writing in %s!", argv[1]);
			}
		}

	}


	close(fd);
	exit(0);
}

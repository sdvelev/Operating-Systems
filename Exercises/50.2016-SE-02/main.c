#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char * argv[]){
	if (argc != 4) {
		errx(1, "Three arguments needed!");
	}

	int fd1 = open (argv[1], O_RDONLY);
	if (fd1 == -1){
		err(2, "Unable to read from %s!", argv[1]);
	}

	int fd2 = open (argv[2], O_RDONLY);
	if (fd2 == -1){
		err(3, "Unable to read from %s!", argv[2]);
	}

	int fd3 = open (argv[3], O_WRONLY);
	if (fd3 == -1){
		err(4, "Unable to write in %s!", argv[3]);
	}


	uint32_t begin;
	uint32_t offset;

	ssize_t read_size;

	while ((read_size = read (fd1, &begin, sizeof(uint32_t))) > 0){

		if (read_size != sizeof(uint32_t)){
			errx(5, "Unable to read from %s!", argv[1]);
		}

		read_size = read (fd1, &offset, sizeof(uint32_t));
		if (read_size == -1 || read_size != sizeof(uint32_t)){
			err(6, "Unable to read from %s!", argv[1]);
		}
		
		uint32_t* str = malloc(offset);

		//if (str == NULL)

		lseek(fd2, begin, SEEK_SET);
		
		if((read_size = read (fd2, str, sizeof(str))) > 0){
			if (read_size == -1 || read_size != sizeof(str)){
				//close(fd1);
				//close(fd2);
				//close(fd3);
				free(str);
				err(6, "Unable to read from %s!", argv[2]);
			}

		}

		if (write(fd3, str, sizeof(str)) != sizeof(str)){
			//close(fd1);
			//close(fd2);
			//close(fd3);
			free(str);
			err(7, "Unable to write in %s!", argv[3]);
		}
		free(str);
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}

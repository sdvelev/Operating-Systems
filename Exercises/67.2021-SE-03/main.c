#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<err.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc, char * argv[]){
	if (argc != 3){
		errx(1, "Two arguments are expected!");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(2, "Unable to open %s!", argv[1]);
	}

	int fd2 = open(argv[2], O_CREAT|O_RDWR|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd2 == -1){
		err(3, "Unable to create %s!", argv[2]);
	}

	struct stat st;
	
	if (fstat(fd1, &st) == -1){
		err(4, "Unable to stat %s!", argv[1]);
	}
	
	if (st.st_size % sizeof(uint16_t) != 0){
		errx(10, "Incorrect format structure!");
	}

	if (st.st_size > 524288 * sizeof(uint16_t)){
		errx(5, "Unable to work with such a bigger file!");
	}

	char * info = "int arr [] = {";
	if(write(fd2, info, 14) != 14){
		err(13, "Error in writing!");
	}

	uint16_t buff;

	ssize_t read_size;

	while((read_size = read(fd1, &buff, sizeof(buff))) > 0){
		if (read_size != sizeof(buff)){
			errx(6, "Error in reading in %s!", argv[1]);
		}

		ssize_t write_size = write(fd2, &buff, sizeof(buff));

		if (read_size != write_size){
			err(8, "Error in writing in %s!", argv[2]);
		}

		char comma = ',';

		write_size = write(fd2, &comma, sizeof(comma));
		if (write_size != 1){
			err(7, "Error in writing in %s!", argv[2]);
		}
	}
	
	if (lseek(fd2, -1, SEEK_CUR) == -1){
		err(9, "Error in lseek!");
	}

	if(write(fd2, "};\n", 3) != 3){
		err(11, "Error in writing!");
	}

	uint32_t size = st.st_size / sizeof(uint16_t);

	if (write (fd2, "int arrN = ", 11) != 11){
		err(11, "Error in writing!");
	}

	if(write(fd2, &size, sizeof(uint16_t)) != sizeof(uint16_t)){
		err(11, "Error in writing!");
	}

	if(write(fd2, ";", 1) != 1){
		err(11, "Error in writing!");
	}
	
	close(fd1);
	close(fd2);
	exit(0);
}

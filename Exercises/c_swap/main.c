#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char * argv[]){

	//Check for three arguments	
	if(argc != 3){
		errx(1, "Two arguments are needed!");
	}

	char* file_from = argv[1];
	char* file_to = argv[2];

	int fd_from, fd_to;

	//Open file1 for reading
	fd_from = open(file_from, O_RDONLY);
	if(fd_from == -1){
		err(2, "Unable to open file %s for reading!", file_from);
	}

	//Openm file2 for reading
	fd_to = open(file_to, O_RDONLY);
	if(fd_to == -1){
		err(3, "Unable to open file %s for reading!", file_to);
	}

	struct stat s;
	
	//Check size of file1
	if (stat(file_from, &s) == -1){
		err(4, "Unable to perform stat!");
	}

	//Allocage memory for storing file1 content
	void* buff = malloc(s.st_size);

	if(buff == NULL){
		err(5, "Does not have enough space!");
	}

	//Reading from file1 and storing in buffer
	if(read(fd_from, buff, s.st_size) != s.st_size){
		free(buff);
		err(6, "Unable to read from %s", file_from);
	}
	//close(fd_from);
	//Open file1 for reading and deleting content /moving cursor/ 
	fd_from = open(file_from, O_TRUNC | O_WRONLY);
	if(fd_from == -1){
		free(buff);
		err(7, "Unable to open file %s for writing!", file_from);
	}

	char c [4096];
	size_t c_size = sizeof(c);
	ssize_t read_size;

	//Reading from file2 in c
	while((read_size = read(fd_to, &c, c_size)) > 0){
		//Writing in file1 from c
		if(write(fd_from, &c, read_size) != read_size)
		{	
			free(buff);
			close(fd_from);
			close(fd_to);
			err(8, "Error in writing from %s to %s!", file_to, file_from);
		}
	}

	close(fd_to);

	//Open file2 for reading and deleting content /moving cursor/
	fd_to = open(file_to, O_TRUNC | O_WRONLY);
	if(fd_to == -1){
		free(buff);
		err(9, "Unable to open file %s for writing!", file_to);
	}

	//Writing in file2 from buffer
	if(write(fd_to, buff, s.st_size) != s.st_size){
		free(buff);
		err(10, "Unable to write in %s", file_to);
	}

	free(buff);
	close(fd_from);
	close(fd_to);
	exit(0);
}

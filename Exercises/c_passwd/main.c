#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(){

	const char* location_from = "/etc/passwd";
	const char* location_to = "./my_passwd";

	int fd1 = open (location_from, O_RDONLY);
	if (fd1 == -1){
		err(1, "Cannot open %s!", location_from);
	}

	int fd2 = open (location_to, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd2 == -1){
		err(2, "Cannot work with %s!", location_to);
	}

	char c;
	
	ssize_t read_size = 0;

	while((read_size = read(fd1, &c, 1)) == 1){

		if(c == ':'){
			c = '?';
		}

		if((write(fd2, &c, 1)) != 1){
			err(3, "Cannot write in %s!", location_to);
		}
	}
	
	close(fd1);
	close(fd2);
	exit(0);
}


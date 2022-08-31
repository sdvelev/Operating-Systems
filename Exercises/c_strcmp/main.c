#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]){
	

	if (argc != 3){
		errx(1, "Two parameters are required!");
	}

	const char* farg = argv[1];
	const char* sarg = argv[2];

	struct stat s;
	if(stat(sarg, &s) == -1){
		errx(2, "Error in stating %s!", sarg);		
	}

	uint16_t* fl = malloc(s.st_size);
	if (fl == NULL){
		errx(3, "Cannot allocate enough memory!");
	}

	uint16_t num;
	ssize_t read_size;

	int fd = open(sarg, O_RDONLY);
	if (fd == -1){
		free(fl);
		errx(4, "Cannot open %s for reading!", sarg);
	}

	int cnt = 0;

	while((read_size = read(fd, &num, sizeof(uint16_t))) == sizeof(uint16_t)){
		fl[cnt++] = num;
	}
	if (read_size == -1 || read_size == 1) {
		free(fl);
		err(5, "Cannot read from %s!", sarg);
	}

	int i;
	if(strcmp(farg, "--print") == 0){

		for(i=0; i < cnt; i++){
			printf("%d\n", fl[i]);
		}
	}
	else if (strcmp(farg, "--min") == 0){
		
		uint16_t minnum = fl[0];

		for(i=0; i < cnt; i++){
			if (minnum > fl[i]){
				minnum = fl[i];
			}
		}
		printf("%d\n", minnum);
	}
	else if (strcmp(farg, "--max") == 0){

		uint16_t maxnum = fl[0];

		for(i=0; i < cnt; i++){
			if (maxnum < fl[i]){
				maxnum = fl[i];
			}
		}
		printf("%d\n", maxnum);
	}
	else {
		close(fd);
		free(fl);
		errx(6, "Invalid first argument");
	}
	exit(0);
}

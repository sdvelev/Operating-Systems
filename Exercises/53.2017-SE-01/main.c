#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<err.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

struct unt{
	uint16_t offset;
	uint8_t o1;
	uint8_t o2;
}__attribute__((packed));

int main(int argc, char * argv[]){

	if (argc != 4){
		errx(1, "Usage: 3 arguments needed!");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(2, "Cannot open %s!", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1){
		err(3, "Cannot open %s!", argv[2]);
	}

	struct stat st1;

	if (fstat(fd1, &st1) == -1){
		err(4, "Cannot stat %s!", argv[1]);
	}

	struct stat st2;

	if(fstat(fd2, &st2) == -1){
		err(5, "Cannot stat %s!", argv[2]);
	}

	if (st1.st_size != st2.st_size){
		errx(6, "%s and %s have different sizes!", argv[1], argv[2]);
	}

	if (st1.st_size > 65535){
		errx(11, "Cannot work with files bigger than 65535!");
	}


	struct unt a;

	int fd3 = open(argv[3], O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	
	if (fd3 == -1){
		err(7, "Cannot open(create) %s!", argv[3]);
	}

	for(off_t i = 0; i < st1.st_size; i++){
		ssize_t read_size;
		read_size = read (fd1, &a.o1, sizeof(a.o1));
		if (read_size != sizeof(a.o1)){
			err(8, "Error in reading from %s!", argv[1]);
		}

		read_size = read (fd2, &a.o2, sizeof(a.o2));
		if (read_size != sizeof(a.o2)){
			err(9, "Error in reading from %s!", argv[2]);
		}

		if (a.o1 != a.o2){
			a.offset = i;
			ssize_t write_size;
			write_size = write(fd3, &a, sizeof(a));
			if (write_size != sizeof(a)){
				err(10, "Error in writing in %s!", argv[3]);
			}
		}
	}
	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}

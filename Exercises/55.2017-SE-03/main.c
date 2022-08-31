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

	struct stat st;

	if(fstat(fd2, &st) == -1){
		err(4, "Cannot stat %s!", argv[2]);
	}

	if (st.st_size > 65535){
		errx(5, "Cannot work with files bigger than 65535!");
	}



	int fd3 = open(argv[3], O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	
	if (fd3 == -1){
		err(6, "Cannot open(create) %s!", argv[3]);
	}


	char c;
	ssize_t read_size;

	while((read_size = read(fd2, &c, sizeof(c))) > 0){

		ssize_t write_size;
		if ((write_size = write(fd3, &c, sizeof(c))) != read_size){
			err(7, "Error in writing from %s!", argv[3]);
		}

	}

	if (read_size == -1){
		err(8, "Error in reading from %s!", argv[2]);
	}


	if (lseek(fd2, 0, SEEK_SET) == -1){
		err(9,"Error in lseek in %s!", argv[1]);
	}
	

	if (lseek(fd3, 0, SEEK_SET) == -1){
		err(10,"Error in lseek in %s!", argv[2]);
	}



	struct unt a;
 		
	while((read_size = read(fd1, &a, sizeof(a))) == sizeof(a)){
		if(lseek(fd2, a.offset, SEEK_SET) == -1){
			err(11, "Error in lseek in %s!", argv[2]);
		}
		
		uint8_t toread;
		ssize_t read_size2 = read(fd2, &toread, sizeof(toread));
		if (read_size2 != sizeof(toread)){
			err(12, "Error in reading from %s!", argv[1]);
		}

		if (toread != a.o1){
			errx(13, "Unequality between files");
		}

		if (lseek(fd3, a.offset, SEEK_SET) == -1){
			err(14, "Error in lseek in %s!", argv[3]);
		}

		if (write(fd3, &a.o2, sizeof(a.o2)) != sizeof(a.o2)){
			err(15, "Error in writing in %s!", argv[3]);
		}

	}

	if (read_size == -1){
			err(16, "Error in reading from %s!", argv[1]);
	}

	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}

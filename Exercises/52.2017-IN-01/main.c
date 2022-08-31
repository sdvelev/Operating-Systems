#include<stdlib.h>
#include<err.h>
#include<errno.h>
#include<fcntl.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

struct unt {

	uint16_t offset;
	uint8_t len;
	uint8_t res;

} __attribute__((packed));

int main(int argc, char * argv []){

	if (argc != 5){
		errx(1, "Incorrect number of arguments!");
	}

	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Cannot stat!");
	}

	int fd1 = open(argv[1], O_RDONLY);

	if (fd1 == -1){
		err(3, "Cannot open %s!", argv[1]);
	}

	struct stat st2;

	if (stat(argv[2], &st2) == -1){
		err(4, "Cannot stat!");
	}

	int fd2 = open(argv[2], O_RDONLY);

	if (fd2 == -1){
		err(5, "Cannot open %s!", argv[2]);
	}

	int fd3 = open(argv[3], O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH);

	if (fd3 == -1){
		err(6, "Cannot open or create %s!", argv[3]);		
	}

	int fd4 = open(argv[4], O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH);

	if (fd4 == -1){
		err(7, "Cannot open or create %s!", argv[4]);
	}

	struct unt collection, collection2;

	collection2.offset = 0;
	collection2.len = 0;
	collection2.res = 0;

	ssize_t read_size;

	while((read_size = read(fd2, &collection, sizeof(collection))) != 0){
		if (read_size != sizeof(collection)){
			err(8, "Cannot read from %s!", argv[2]);
		}

		if (lseek(fd1, collection.offset, SEEK_SET) == -1){
			err(9, "Cannot move to correct offset (%d) in %s!", collection.offset, argv[1]);
		}

		uint8_t * buff = malloc(collection.len);
	
		if (buff == NULL){
			err(13, "Cannot allocate enough memory!");
		}

		if ((read_size = read(fd1, buff, sizeof(buff))) != 0){
			if (read_size != sizeof(buff)){
				int msg = errno; 
				free(buff);
				errno = msg;
				err(10, "Cannot read from %s!", argv[1]);
			}
			
			if(buff[0] >= 0x41 && buff[0] <= 0x5A){
				
				if(write(fd3, buff, sizeof(buff)) != sizeof(buff)){
					free(buff);
					err(11, "Cannot write in %s!", argv[3]);
				}
				
				collection2.len = collection.len;
				
				if (write(fd4, &collection2, sizeof(collection2)) != sizeof(collection2)){
					free(buff);
					err(12, "Cannot write in %s!", argv[4]);
				}

				collection2.offset += collection.len; 
			}
		}
		free(buff);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	exit(0);
}

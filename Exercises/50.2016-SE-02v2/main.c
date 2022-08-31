#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>

struct unt {
	uint32_t pos;
	uint32_t lgth;
};

int main (int argc, char * argv[]){


	const char * F1 = "f1";
	const char * F2 = "f2";
	const char * F3 = "f3";

	struct stat stf1;

	if (stat(F1, &stf1) == -1){
		err(1, "Unable to stat %s!", F1);
	}

	struct stat stf2;

	if (stat(F2, &stf2) == -1){
		err(2, "Unable to stat %s!", F2);
	}

	if (stf1.st_size % 2 * sizeof(uint32_t) != 0){
		errx(3, "%s: Not right format and structure!", F1);
	}

	if (stf2.st_size % sizeof(uint32_t) != 0){
		errx(4, "%s: Not right format and structure!", F2);
	}

	int fd1 = open (F1, O_RDONLY);

	if (fd1 == -1){
		err(5, "Unable to read from %s!", F1);
	}

	int fd2 = open (F2, O_RDONLY);

	if (fd2 == -1){
		err(6, "Unable to read from %s!", F2);
	}
	
	int fd3 = open (F3,  O_TRUNC | O_CREAT | O_WRONLY, S_IRUSR, S_IWUSR);

	if (fd3 == -1){
		err(7, "Unable to create or write in %s!", F3);
	}	
	
	struct unt unt_ex;

	ssize_t read_size;

	while ((read_size = read(fd1, &unt_ex, sizeof(unt_ex))) > 0){
		if (read_size % sizeof(unt_ex) != 0){
			err(8, "%s: Error in reading!", F1);
		}
		
		off_t lsk = lseek(fd2, unt_ex.pos * sizeof(uint32_t), SEEK_SET);
		if (lsk == -1){
			err(9, "%s: Unable to reposition the file offset!", F2);
		}

		uint32_t num;

		for(uint32_t i = 0; i < unt_ex.lgth; i++){
			
			ssize_t read_size2 = read(fd2, &num, sizeof(uint32_t));
			if (read_size2 != sizeof(uint32_t)){
				err(10, "%s: Error in reading!", F2);
			}

			ssize_t write_size = write (fd3, &num, read_size2);

			if (write_size != read_size2){
				err(11, "%s: Error in writing!", F3);
			}
			
		}

	}


	if (read_size == -1){
		err(10, "%s: Error in reading!", F1);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}

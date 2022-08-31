#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<err.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<stdio.h>


struct m{
	char name[8];
	uint32_t offset;
	uint32_t length;
}__attribute__((packed));

int main(int argc, char * argv[]){

	if (argc != 2){
		errx(1, "Usage: 1 arguments needed!");
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		err(2, "Cannot open %s!", argv[1]);
	}

	struct stat st;

	if(fstat(fd, &st) == -1){
		err(4, "Cannot stat %s!", argv[1]);
	}
	
	
	if (st.st_size % sizeof(struct m) != 0){
		errx(5, "Not valid file!");
	}

	int total = st.st_size / sizeof(struct m);

	struct m collection[8];

	ssize_t read_size;

	for(int i = 0; i < total; i++){
		
		read_size = read(fd, &collection[i], sizeof(struct m));
	}

	int p[2];
	if(pipe(p) == -1){
		err(11, "Error in creating pipe!");
	}

	uint16_t result[8];

	for(int i = 0; i < total; i++){
		int x = fork();

		if (x == -1){
			err(6, "Error in fork!");
		}

		if (x == 0){
			int y = open(collection[i].name, O_RDONLY);
			if(fstat(y, &st) == -1){
				err(4, "Cannot stat %s!", argv[1]);
			}
	
			if (st.st_size % sizeof(uint16_t) != 0){
				errx(7, "Not valid file!");
			}

			uint16_t res;
			uint16_t e;

			if (lseek(y, collection[i].offset*sizeof(uint16_t), SEEK_SET) == -1){
				err(8, "Error in lseek!");
			}

			for(uint32_t j = 0; j < collection[i].length; j++){
				if (read(y, &e, sizeof(e)) != sizeof(e)){
					err(9, "Error in reading!");
				}
				res ^= e;
			}
			close(p[0]);
		
			ssize_t write_size = write(p[1], &res, sizeof(res));
			
			if (write_size == -1){
				err(10, "Error in writing!");
			}

			if (write_size != sizeof(uint16_t)){
				errx(11, "Error in reading enough bytes!");
			}
			
			close(y);	
			exit(0);

		}

		close(p[1]);
		
		int status;

		if(wait(&status) == -1){
			err(13, "Error in wait!");
		}

		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0){
			errx(14, "One of the child did not terminate or finish normally!");
		}

		uint16_t resultN;

		if ((read_size = read(p[0], &resultN, sizeof(resultN))) != sizeof(resultN)){
			if (read_size != 0){
				err(12, "Unable to read from pipe!");
			}
		}

		result[i] = resultN;
	}

	
	uint16_t ans;
	ans = result[0];

	for(int i = 1; i < total; i++){
		ans ^= result[i];
	}

	dprintf(1, "result: %dB", ans);

	close(fd);
	exit(0);
}

#include<sys/types.h>
#include<stdint.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<err.h>

int main(int argc, char * argv[]){
	
	char c;
	ssize_t read_size;
	ssize_t write_size;

	if (argc == 1){
		while((read_size = read(0, &c, sizeof(c))) != 0){
			if (read_size == -1){
				err(1, "Error in reading from STDIN!");
			}
			if ((write_size = write(1, &c, sizeof(c))) != read_size){
				err(2, "Error in writing to STDOUT!");
			}
		}
	}	

	
	for(int i = 1; i < argc; i++){
		if((strcmp(argv[i], "-")) == 0){
			while((read_size = read(0, &c, sizeof(c))) != 0){
				if (read_size == -1){
					err(1, "Error in reading from STDIN!");
				}
				if ((write_size = write(1, &c, sizeof(c))) != read_size){
					err(2, "Error in writing to STDOUT!");
				}
			}
			continue;	
		}
		
		int fd = open(argv[i], O_RDONLY);
		if (fd == -1){
			err(3, "Unable to open %s!", argv[i]);
		}

		while((read_size = read(fd, &c, sizeof(c))) != 0){
			if (read_size == -1){
				err(4, "Unable to read from %s!", argv[i]);
			}

			if ((write_size = write(1, &c ,sizeof(c))) != read_size){
				err(5, "Error in writing in %s!", argv[i]);
			}
		}
		
	}

	exit(0);
}

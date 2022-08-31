#include <err.h>    
#include <fcntl.h> 
#include <stdint.h>  
#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>  
#include<sys/stat.h>

//uint8_t arr1[]  = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080};
uint8_t arr1[]  = {0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};


int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Invalid number of arguments!");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        err(1, "Unanable to open %s!", argv[1]);
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) {
        err(3, "Unable to open %s!", argv[2]);
    }

	struct stat st;

	if (fstat(fd2, &st) == -1){
		err(12, "Error in stat in %s!", argv[1]);
	}

	if (st.st_size % sizeof(uint16_t) != 0){
		errx(13, "File %s is not in the correct format!", argv[2]);
	}
	
	int fd3 = open("result.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd3 == -1){
		err(5, "Unable to open result file!");
	}


    uint8_t number;
    uint16_t encodedNumber;
	ssize_t read_size;

    while ((read_size = read(fd1, &number, sizeof(number))) == sizeof(number)) {
       
	
		for(int i = 0; i < 8; i++){
			
			if ((number & arr1[i]) > 0){
				
				if((read_size = read(fd2, &encodedNumber, sizeof(uint16_t))) > 0){
					if (read_size != sizeof(uint16_t)){
						errx(8, "Unable to read enough characters from %s!", argv[2]);
					}

					ssize_t write_size;
					
					if ((write_size = write(fd3, &encodedNumber, sizeof(uint16_t))) > 0){
						if (write_size != read_size){
							errx(9, "Error in writing in result file!");
						}
					}

					if (write_size == -1){
						err(10, "Error in writing in result file!");
					}

				}
			}
			else {
				if(lseek(fd2, sizeof(uint16_t), SEEK_CUR) == -1){
					err(11, "Error in lseek in %s!", argv[2]);
				}
			}
			
		}
    }
    
	if (read_size == -1){
		err(4, "Unable to read from %s!", argv[1]);
	}	

	close(fd1);
	close(fd2);
	close(fd3);

    exit(0);
}

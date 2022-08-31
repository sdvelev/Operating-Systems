#include <err.h>    
#include <fcntl.h> 
#include <stdint.h>  
#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>  

uint8_t arr1[]  = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080};
uint16_t arr2[] = {0x0002, 0x0008, 0x0020, 0x0080, 0x0200, 0x0800, 0x2000, 0x8000};
uint16_t arr3[] = {0x0001, 0x0004, 0x0010, 0x0040, 0x0100, 0x0400, 0x1000, 0x4000};

uint16_t encode(const uint8_t number) {

    uint16_t result = 0;

    for (int i = 0; i < 8; i++) {
        if ((number & arr1[i]) != 0) {
            result = result | arr2[i];
        }
		else if ((number & arr1[i]) == 0) {
            result = result | arr3[i];
        }
	}

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errx(1, "Invalid number of arguments!");
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        err(1, "Unanable to open %s!", argv[1]);
    }

    int fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd2 == -1) {
        err(3, "Unable to open %s!", argv[2]);
    }

    uint16_t map[256];
    for (int i = 0; i < 256; i++) {
        map[i] = encode(i);
    }

    uint8_t number;
    uint16_t encodedNumber;
	ssize_t read_size;

    while ((read_size = read(fd1, &number, sizeof(number))) == sizeof(number)) {
        encodedNumber = map[number];
        if (write(fd2, &encodedNumber, sizeof(encodedNumber)) != sizeof(encodedNumber)) {
            err(2, "Unable to write in %s!", argv[2]);
        }
    }
    
	if (read_size == -1){
		err(4, "Unable to read from %s!", argv[1]);
	}	

	close(fd1);
	close(fd2);

    exit(0);
}

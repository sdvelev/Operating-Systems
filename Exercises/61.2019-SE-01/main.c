#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<err.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdint.h>

struct unt {
	uint32_t uid;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t time1;
	uint32_t time2;
}__attribute__((packed));

int main(int argc, char * argv[]){
	
	if (argc != 2){
		errx(1, "Exactly one argument needed!");
	}


	int fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		err(2, "Error in opening %s!", argv[1]);
	}

	struct stat st;

	if (fstat(fd, &st) == -1){
		err(3, "Error in stat %s!", argv[1]);
	}	

	if (st.st_size % (sizeof (struct unt)) != 0){
		errx(4, "Argument %s is not in correct format!", argv[1]);
	}

	ssize_t read_size;

	struct unt collection;

	uint16_t counter = 0;
	uint64_t totalLength = 0;

	while ((read_size = read(fd, &collection, sizeof(collection))) > 0){

		if (read_size != sizeof(collection)){
			err(6, "Unable to read enough characters!");
		}
		++counter;
		totalLength = totalLength + (collection.time2 - collection.time1); 

		printf("Total Length: %ld\n", totalLength);

	}

	if (read_size == -1){
		err(5, "Error in reading from %s!", argv[1]);
	}


	uint64_t meanValue = totalLength / counter;

	printf("Mean Value: %ld\n", meanValue);

	if(lseek(fd, 0, SEEK_SET) == -1){
		err(7, "Error in lseek in %s!", argv[1]);
	}

	uint64_t totalLengthMean = 0;

	while ((read_size = read(fd, &collection, sizeof(collection))) > 0){

		if (read_size != sizeof(collection)){
			err(6, "Unable to read enough characters!");
		}
		totalLengthMean = totalLengthMean + ((collection.time2 - collection.time1) - meanValue) * ((collection.time2 - collection.time1) - meanValue);
	   printf("Total Length Mean: %ld\n", totalLengthMean);	
	}

	uint64_t dis = totalLengthMean / counter;


	printf("Dispersion: %ld\n", dis);


	if(lseek(fd, 0, SEEK_SET) == -1){
		err(7, "Error in lseek in %s!", argv[1]);
	}


	while ((read_size = read(fd, &collection, sizeof(collection))) > 0){

		if (read_size != sizeof(collection)){
			err(6, "Unable to read enough characters!");
		}
	
	
		if ((collection.time2 - collection.time1) * (collection.time2 - collection.time1) > dis){
			uint32_t currUid = collection.uid;
			printf("UID: %d\t", currUid);
			off_t curPos = lseek(fd, 0, SEEK_CUR);
			if (curPos == -1){
				err(7, "Error in lseek in %s!", argv[1]);
			}	

			if (lseek(fd, 0, SEEK_SET) == -1){
				err(7, "Error in lseek in %s!", argv[1]);
			}

			uint64_t maxLongevity = 0;

			while((read_size = read(fd, &collection, sizeof(collection))) > 0){
				
				if (read_size != sizeof(collection)){
					err(6, "Unable to read enough characters from %s!", argv[1]);
				}

				if (collection.uid == currUid && maxLongevity < (collection.time2 - collection.time1)){
					maxLongevity = collection.time2 - collection.time1;
				}
			}

			if (read_size == -1){
				err(5, "Error in reading from %s!", argv[1]);
			}

			printf("Max Length Session: %ld\n", maxLongevity);

			if (lseek(fd, curPos, SEEK_SET) == -1){
				err(7, "Error in lseek in %s!", argv[1]);
			}
		}
	}

	if (read_size == -1){
		err(5, "Error in reading from %s!", argv[1]);	
	}

	exit(0);
}

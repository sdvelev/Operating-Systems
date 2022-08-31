#include<unistd.h>
#include<fcntl.h>
#include<err.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdint.h>
#include<stdlib.h>


int cmp(const uint32_t * a, const uint32_t * b){
	if (*a > *b)
		return 1;
	else if  (*a < *b)
		return -1;

	return 0;
}

int main (int argc, char * argv[]){

	if (argc != 3){
		errx(1, "Two arguments needed!");
	}

	char * bin = argv[1];
	char * sorted = argv[2];

	struct stat st;

	if (stat(bin, &st) == -1){
		err(2, "Error in stat %s!", bin);
	}

	if (st.st_size % sizeof(uint32_t) != 0){
		errx(3, "%s must contain only uint32_t numbers!", bin);
	}

	int total = st.st_size / sizeof(uint32_t);

	int half = total / 2;
	int rest = total - half;

	uint32_t * collection = malloc(half * sizeof (uint32_t));

	if (collection == NULL){
		err(4, "Cannot allocate enough memory!");
	}

	int fd1 = open(bin, O_RDONLY);
	if (fd1 == -1){
		free(collection);
		err(5, "Error in opening %s!", bin);
	}	

	int read_size;

	if ((read_size = read(fd1, collection, half * sizeof(uint32_t))) != half * sizeof(uint32_t)){
		free(collection);
		err(7, "Error in reading from %s!", bin);
	}

	qsort(collection, half, sizeof(uint32_t), cmp);
	
 	int t1 = open("temp1", O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if (t1 == -1){
		free(collection);
		err(6, "Error in opening temporary file!");
	}	

	int write_size;

	if ((write_size = write(t1, collection, half * sizeof(uint32_t))) != half * sizeof(uint32_t)){
		free(collection);
		err(8, "Error in writing in temporary file!");
	}	

	free(collection);

	int t2 = open("temp2", O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if (t2 == -1){
		free(collection);
		err(9, "Error in opening temporary file!");
	}

	uint32_t * collection2 = malloc(rest * sizeof(uint32_t));
	if (collection2 == NULL){
		err(4, "Cannot allocate enough memory!")	;
	}
	
	if ((read_size = read(fd1, collection2, rest * sizeof(uint32_t))) != rest * sizeof(uint32_t)){
		free(collection2);
		err(7, "Error in reading from %s!", bin);
	}

	qsort(collection2, rest, sizeof(uint32_t), cmp);

	if ((write_size = write(t2, collection2, rest * sizeof(uint32_t))) != rest * sizeof(uint32_t)){
		free(collection2);
		err(8, "Error in writing in temporary file!");
	}

	free(collection2);

	if (lseek(t1, 0, SEEK_SET) == -1){
		err(10, "Error in lseek in temporary file!");
	}
	
	if (lseek(t2, 0, SEEK_SET) == -1){
		err(10, "Error in lseek in temporary file!");
	}
	
	uint32_t a;
	uint32_t b;

	int rs1;
	int rs2;

	int fd2 = open(sorted, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR,S_IRGRP|S_IROTH);
	if (fd2 == -1){
		err(11, "Error in opening %s!", sorted);
	}

	while((rs1 = read(t1, &a, sizeof(a))) == sizeof(a) && (rs2 = read(t2, &b, sizeof(b))) == sizeof(b)){
		if (a <= b){
			if (write(fd2, &a, sizeof(a)) != sizeof(a)){
				err(11, "Error in writing in %s!", sorted);
			}
			lseek(t2, -1 * sizeof(b), SEEK_CUR);
		}
		else {

			if (write(fd2, &b, sizeof(b)) != sizeof(b)){
				err(11, "Error in writing in %s!", sorted);
			}
		
			lseek(t1, -1 * sizeof(b), SEEK_CUR);
		}
	}

	if (rs1 == -1 || rs2 == -1){
		err(12, "Error in reading from temporary file!");
	}

	if (rs1 == 0){
		while((rs2 = read(t2, &b, sizeof(b))) == sizeof(b)){
			if (write(fd2, &b, sizeof(b)) != sizeof(b)){
				err(11, "Error in writing in %s!", sorted);
			}
		}
	}
	else if (rs2 == 0){
		while((rs1 = read(t1, &a, sizeof(a))) == sizeof(a)){
			if (write(fd2, &a, sizeof(a)) != sizeof(a)){
				err(11, "Error in writing in %s!", sorted);
			}
		}
	}

	if (rs1 == -1 || rs2 == -1){
		err(12, "Error in reading from temporary file!");
	}

	unlink("temp1");
	unlink("temp2");


	close(fd1);
	close(fd2);
	close(t1);
	close(t2);
	exit(0);
}

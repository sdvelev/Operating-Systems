
#include <stdio.h>
#include<err.h>
#include<fcntl.h>
#include<unistd.h>


int main(int argc, char * argv[]){
	
	if (argc != 3){
		err(1, "Not enough arguments" );
	}

	int fd1;
	if ( (fd1 = open(argv[1], O_RDONLY)) == -1 ){
		err(2, "Cannot open argument 1 for reading");
	}

	int fd2;

	if ( (fd2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR)) == -1){
		err(3, "Cannot open argument 2 for reading");	
	}
		
	char buf[256];

	int read_size;

	while ( (read_size = read(fd1, buf, sizeof(buf))) > 0) {
		if(write(fd2, buf, read_size) == -1) {
			err(4, "Cannot write in argument 2");
		}
	}


	close(fd1);

	//close_file(fd1);

	close(fd2);
	
	return 0;	
}

#include<stdint.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<err.h>
#include<stdlib.h>

int main(int argc, char * argv []){
	
	if (argc != 3){
		errx(1, "Two arguments needed!");		
	}

	int n = argv[1][0] - '0';
	int d = argv[2][0] - '0';
	
	int pf1[2];
	int pf2[2];

	if (pipe(pf1) == -1){
		err(2, "Error in creating unnamed pipe!");
	}

	if (pipe(pf2) == -1){
		err(2, "Error in creating unnamed pipe!");
	}
	
	char c = 'a';

	const char * ms1 = "DING ";
	const char * ms2 = "DONG\n";

	int pid = fork();
	
	if (pid == -1){
		err(3, "Error in fork!");
	}

	if (pid == 0){
		
		close(pf1[1]);
		close(pf2[0]);

		for(int i = 0; i < n; i++){
			if (read(pf1[0], &c, sizeof(c)) == -1){
				err(4, "Error in reading from pipe!");
			}

			ssize_t write_size;

			if ((write_size = write(1, ms2, 5)) != 0){
				if (write_size == -1){
					err(5, "Error in writing to stdout!");
				}
				else if (write_size != 5){
					err(6, "Unable to write enough characters!");
				}
			}

			if (write(pf2[1], &c, sizeof(c)) == -1){
				err(7, "Error in writing to pipe2!");
			}

		}
		exit(0);
	}

	close(pf1[0]);
	close(pf2[1]);

	for(int i = 0; i < n; i++){
		
		ssize_t write_size;

		if ((write_size = write(1, ms1, 5)) != 0){
			
			if (write_size == -1){
				err(5, "Error in writing io stdout!");
			}
			else if (write_size != 5){
				err(6, "Unable to read enough characters!");
			}
		}

		if (write(pf1[1], &c, sizeof(c)) == -1){
			err(8, "Error in writing to pipe1!");
		}

		if (read(pf2[0], &c, sizeof(c)) == -1){
			err(9, "Error in reading from pipe2!");
		}

		sleep(d);

	}

	exit(0);
}

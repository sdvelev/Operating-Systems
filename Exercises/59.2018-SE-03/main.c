#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/types.h>
#include<fcntl.h>
#include<err.h>
#include<string.h>

int main(int argc, char * argv[]){
	
	ssize_t read_size;
	
	if (argc ==  3 && (strcmp(argv[1], "-c") == 0) ){
		
		if (strlen(argv[2]) == 1){
			
			int pos = argv[2][0] - '0';
			char c;
			int counter = 0;

			while((read_size = read(0, &c, sizeof(c)) ) > 0){
				
				if (c == '\n'){
					counter = 0;
					if(write(1, "\n", 1) == -1){
						err(2, "Error in writing to stdout!");
					}
					continue;
				}

				++counter;

				if (counter == pos){
					if (write(1, &c, sizeof(c)) == -1){
						err(2, "Error in writing to STDOUT!");
					}
				}

			}
			
			if (read_size == -1){
				err(1, "Error in reading from STDIN!");
			}

		}
		else if (strlen(argv[2]) == 3){
			
			int pos1 = argv[2][0] - '0';
			int pos2 = argv[2][2] - '0';
			
			if (pos1 > pos2){
				errx(3, "Incorrect command arguments!");
			}

			char c;
			int counter = 0;
			
			while((read_size = read(0, &c, sizeof(c)) ) > 0){

				if (c == '\n'){
					counter = 0;
					if(write(1, "\n", 1) == -1){
						err(2, "Error in writing to stdout!");
					}
					continue;
				}

				++counter;

				if (counter >= pos1 && counter <= pos2){
					if (write(1, &c, sizeof(c)) == -1){
						err(2, "Error in writing to STDOUT!");
					}
				}

			}

			if (read_size == -1){
				err(1, "Error in reading from STDIN!");
			}
		}
	}
	else if ((strcmp(argv[1], "-d") == 0) && argc == 5){
		char delim = argv[2][0];

		if (strcmp(argv[3], "-f") != 0){
			errx(3, "Inorrect command arguments!");
		}

		if (strlen(argv[4]) == 1){
			int fl = argv[4][0] - '0';
			char c;
			int curFl = 1;
			

			while((read_size = read(0, &c, sizeof(c))) > 0){
			
				if (c == '\n'){
					curFl = 1;
					if(write(1, "\n", 1) == -1){
						err(2, "Error in writing to stdout!");
					}
					continue;
				}	

				if (c == delim){
					++curFl;
					continue;
				}

				if(curFl == fl){
					if (write(1, &c, sizeof(c)) == -1){
						err(2, "Error in writing to STDOUT!");
					}
					continue;
				}

			}
			if (read_size == -1){
				err(1, "Error in reading from STDIN!");
			}
				

		}
		else if (strlen(argv[4]) == 3){
			int fl1 = argv[4][0] - '0';
			int fl2 = argv[4][2] - '0';
			char c;
			int curFl = 1;

			while((read_size = read(0, &c, sizeof(c))) > 0){

				if (c == '\n'){
					curFl = 1;
					if(write(1, "\n", 1) == -1){
						err(2, "Error in writing to stdout!");
					}
					continue;
				}	

				if (c == delim){
					++curFl;
					continue;
				}

				if(curFl >= fl1 && curFl <= fl2){
					if (write(1, &c, sizeof(c)) == -1){
						err(2, "Error in writing to STDOUT!");
					}
					continue;
				}

			}
			if (read_size == -1){
				err(1, "Error in reading from STDIN!");
			}

		}
	}
	else {
			errx(3, "Incorrect command arguments!");
	}

	exit(0);
}

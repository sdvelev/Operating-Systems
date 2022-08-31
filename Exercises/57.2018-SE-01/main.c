#include<stdint.h>
#include<unistd.h>
#include<sys/types.h>
#include<err.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char * argv[]){
	
	if(argc == 1){
		err(1, "There are no arguments!");
	}	
	
	if (argc > 4){
		err(2, "There are more arguments than needed!");
	}

	ssize_t read_size;
	char c;
	int flag = 0;


	if((strcmp(argv[1], "-d")) == 0){
		
		char * to_delete = malloc(strlen(argv[2]) + 1);

		if (to_delete == NULL){
			err(3, "Unable to allocate enough memory!");
		}

		strcpy(to_delete, argv[2]);
	
		while((read_size = read(0, &c, sizeof(c))) != 0){
			if (read_size == -1){
				err(4, "Unable to read from standart input!");
			}	

			for(unsigned int i = 0; i < strlen(to_delete); i++){
				if (c == to_delete[i]){
					flag = 1;
					break;
				}
			}
			
			if (flag){
				flag = 0;
				continue;
			}	
			
			if (write(1, &c, sizeof(c)) <= 0){
				err(5, "Unable to write to stdout!");
			}
		}
		
		free(to_delete);

	}
	else if ((strcmp(argv[1], "-s")) == 0){
		
		char prev = ' ';
		flag = 0;

		char * to_squeeze = malloc(strlen(argv[2]) + 1);
		if (to_squeeze == NULL){
			err(3, "Unable to allocate enough memory!");
		}

		strcpy(to_squeeze, argv[2]);



		while((read_size = read(0, &c, sizeof(c))) != 0){

			if (read_size == -1){
				err(4, "Unable to read from standart input!");
			}	

			if (prev == c){
				for(unsigned int i = 0; i < strlen(to_squeeze); i++){
					if (c == to_squeeze[i]){
						flag = 1;
						break;
					}
				}
			}

			prev = c;

			if (flag){
				flag = 0;
				continue;
			}

			if (write(1, &c, sizeof(c)) != sizeof(c)){
				err(5, "Unable to write to stdout!");
			}
			
		}
		free(to_squeeze);
	}
	else if (argc == 3 && strlen(argv[1]) == strlen(argv[2])){
		char * from = malloc(strlen(argv[1]) + 1);
		strcpy(from, argv[1]);

		char * to = malloc(strlen(argv[2]) + 1);
		strcpy(to, argv[2]);

		unsigned index;
		flag = 0;

		while((read_size = read(0, &c, sizeof(c))) != 0){
			if (read_size == -1){
				err(4, "Unable to read from standart input!");
			}

			for(unsigned i = 0; i < strlen(from); i++){
				if (c == from[i]){
					flag = 1;
					index = i;
					break;
				}
			}


			if (flag){
				flag = 0;
				char r = to[index];
				if (write(1, &r, sizeof(r)) <= 0){
					err(5, "Unable to write to stdout!");
				}
				continue;
			}
			
			if (write(1, &c, sizeof(1)) <= 0){
				err(5, "Unable to write to stdout!");
			}
		}
	}
	else {
		errx(6, "Incorrect command arguments!");
	}	

	exit(0);
}	

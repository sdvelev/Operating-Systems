#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<err.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>

struct helper{
	int n;
	int c;
	int b; //begin of line
};

void print_function(int fd, struct helper * m){

	char c;
	ssize_t read_size;
	while((read_size = read(fd, &c, sizeof(c))) == sizeof(c)){
		

		if(m->n == 1 && m->b == 1){
			
			printf("%d ", m->c);

			setbuf(stdout, NULL);
			
			//writeln(m->c);
			m->b = 0;
		}

		ssize_t write_size;
		write_size = write(1, &c, sizeof(c));
		if (write_size != read_size){
			err(3, "Error in writing!");
		}

		if (c == '\n'){
			m->b = 1;
			m->c++;
		}
	}

	if(read_size == -1){
		err(2, "Error in reading!");
	}
}

int main(int argc, char * argv[]){


	struct helper h = {.n = 0, .c = 1, .b = 1};

	if (argc == 1){
		print_function(0, &h);
	}
	
	int q;

	if(!strcmp(argv[1], "-n")){
		q = 2;
		h.n = 1;

		if(argc == 2){
			print_function(0, &h);
		}

	}
	else {
		q = 1;
	}

	for(int i = q; i < argc; i++){
		
		if(!strcmp(argv[i], "-")){
			print_function(0, &h);
			continue;
		}

		int fd = open(argv[i], O_RDONLY);
		if (fd == -1){
			err(1, "Error in opening %s!", argv[i]);
		}

		print_function(fd, &h);
		
		close(fd);
	}	
		
	exit(0);
}

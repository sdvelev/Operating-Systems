#include<stdlib.h>
#include<stdio.h>
#include<err.h>	
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char * argv[]){

	if(argc != 2){
		errx(1, "One argument expected!");
	}

	int pf[2];

	int res = pipe(pf);
	if(res == -1){	
		err(2, "Unable to create pipe!");
	}

	int pid = fork();

	if (pid == -1){
		err(4, "Error in fork!");
	}

	if (pid == 0){
		close(pf[0]);
		dup2(pf[1], 1);
		
		if(execl("/bin/cat", "cat", argv[1], (char *) NULL) == -1){
			err(3, "Error in execl!");
		}
	}	
	else if (pid > 0){
		close(pf[1]);
		dup2(pf[0], 0);
		
		if (execl("/bin/sort", "sort", (char *) NULL) == -1){
			err(3, "Error in execl!");
		}
	}

	exit(0);
}

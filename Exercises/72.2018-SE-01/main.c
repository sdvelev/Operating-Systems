// find ~ -mindepth 1 -type f -printf "%T@ %p\n" | sort -nr | head -1 | cut -d " " -f2
#include<stdint.h>
#include<sys/types.h>
#include<fcntl.h>
#include<err.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char * argv[]){
	
	if (argc != 2){
		errx(3, "Usage: One arguments needed!");
	}

	int pf1[2];
	int pf2[2];
	int pf3[2];

	if (pipe(pf1) == -1){
		err(1, "Error in creating pipe1!");
	}

	int pid = fork();
	if (pid == -1){
		err(2, "Error in fork!");
	}

	if (pid == 0){
		close(pf1[0]);
		
		if(dup2(pf1[1] ,1) == -1){
			err(4, "Error in dup2!");
		}

		if (execlp("find", "find", argv[1], "-mindepth", "1", "-type", "f", "-printf", "%T@ %p\n", (char *) NULL) == -1){
			err(5, "Error in execlp!");
		}

	}

	close(pf1[1]);

	if (pipe(pf2) == -1){
		err(6, "Error in creating pipe2!");
	}

	int pid2 = fork();
	if (pid2 == -1){
		err(2, "Error in fork!");
	}

	if (pid2 == 0){
		
		if (dup2(pf1[0], 0) == -1){
			err(4, "Error in dup2!");
		}
		close(pf2[0]);

		if (dup2(pf2[1], 1) == -1){
			err(4, "Error in dup2!");
		}

		if (execlp("sort", "sort", "-nr", (char *) NULL) == -1){
			err(5, "Error in execlp!");
		}
	}

	close(pf1[0]);
	close(pf2[1]);

	if (pipe(pf3) == -1){
		err(6, "Error in creating pipe2!");
	}

	int pid3 = fork();
	if (pid3 == -1){
		err(2, "Error in fork!");
	}

	if (pid3 == 0){
		if (dup2(pf2[0],0) == -1){
			err(4, "Error in dup2!");
		}
		close(pf3[0]);
		if (dup2(pf3[1], 1) == -1){
			err(4, "Error in dup2!");
		}

		if (execlp("head", "head", "-1", (char *) NULL) == -1){
			err(5, "Error in execlp!");
		}
	}	

	close(pf2[0]);
	close(pf3[1]);

	if (dup2(pf3[0] ,0) == -1){
		err(4, "Error in dup2!");
	}

	close(pf3[0]);

	if (execlp("cut", "cut", "-d", " ", "-f2", (char *) NULL) == -1){
		err(5, "Error in execlp!");
	}

	exit(0);
}

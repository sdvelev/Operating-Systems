#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char* argv[]) {

	if (argc == 1){
		errx(1, "There are no arguments!");
	}


	for (int i = 1; i < argc; i++) {
		
		int fd = open(argv[i], O_RDONLY);
		if (fd == -1){
			warn("Cannot open argument %d!", i);
			continue;
		}

//		fprintf(stderr,"%s:\n", argv[i]);

		char c[256];
		ssize_t rs;

		while ((rs = read(fd, &c, sizeof(c))) > 0)	{
			if(write(1, &c, rs) != rs){
				err(3, "%s: Cannot write in buffer", argv[i]);
			}
		}
		if (rs == -1) {
			warn("Cannot read from argument %s!", argv[i]);
		}
		close(fd);
	}




}


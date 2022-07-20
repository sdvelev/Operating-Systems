#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<err.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<errno.h>

const int MAGIC_SIZE = 3;
const int RAM_SIZE = 4;
const int INSTRUCTION_SIZE = 25;

struct unt {
	uint8_t opcode;
	int64_t operands[3];
}__attribute__((packed));


void setFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//set (tar, val) - write val in cell with address tar	
	int64_t tar = instruction->operands[0];
	int64_t val = instruction->operands[1];
	
	if (tar >= ram_size || tar < 0){
		free(collection);
		errx(11, "Incorrect instruction in %s!", program);
	}
	collection[tar] = val;
}


void outFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//out (v) - print symbol on stdout from cell with address v
	int64_t v = instruction->operands[0];
	
	if (v >= ram_size || v < 0){
		free(collection);
		errx(11, "Incorrect instruction in %s!", program);
	}

	char c = (char) collection[v];

	printf("%c", c);
}


void loadFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//load (to, from_ptr) - copy value from from_ptr cell address into to cell address
	int64_t to = instruction->operands[0];
	int64_t from_ptr = instruction->operands[1];

	if (to >= ram_size || from_ptr >= ram_size || collection[from_ptr] >= ram_size || to < 0 || from_ptr < 0 || collection[from_ptr] < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	collection[to] = collection[collection[from_ptr]];
}

void storeFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//store (to_ptr, from) - copy value from from cell address into value of to_ptr cell address
	int64_t to_ptr = instruction->operands[0];
	int64_t from = instruction->operands[1];

	if (to_ptr >= ram_size || collection[to_ptr] >= ram_size || from >= ram_size || to_ptr < 0 || collection[to_ptr] < 0 || from < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	collection[collection[to_ptr]] = collection[from];
}


void jmpFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size, int fd, const int numberOfInstructions){
	
	//jmp (idx) - move to instruction with address the value of cell with address idx
	int64_t idx = instruction->operands[0];

	if (idx >= ram_size || idx < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}
	
	if (collection[idx] >= numberOfInstructions || collection[idx] < 0){
		free(collection);
		errx(16, "Incorrect instruction (invalid instruction address) in %s!", program);
	}
	
	if (lseek(fd, MAGIC_SIZE + RAM_SIZE + collection[idx] * INSTRUCTION_SIZE, SEEK_SET) == -1){
		int old_errno = errno;
		free(collection);
		errno = old_errno;
		err(12, "Unable to lseek in %s!", program);
	}
	
}


void sgzFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size, int fd, struct stat st){

	//sgz (v) - move to next instruction if v is greater than 0
	int64_t v = instruction->operands[0];

	if (v >= ram_size || v < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	if (collection[v] > 0){
		off_t res =  lseek(fd, INSTRUCTION_SIZE, SEEK_CUR);
		if (res == -1){
			int old_errno = errno;
			free(collection);
			errno = old_errno;
			err(12, "Unable to lseek in %s!", program);
		}
		if (res > st.st_size){
			res = lseek (fd, -INSTRUCTION_SIZE, SEEK_CUR);
			if (res == -1){
				int old_errno = errno;
				free(collection);
				errno = old_errno;
				err(12, "Unable to lseek in %s!", program);
			}
		}
	}

}

void addFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//add (res, v1, v2) - in cell with address res add sum of numbers with cells with addresses v1 and v2	
	int64_t res = instruction->operands[0];
	int64_t v1 = instruction->operands[1];
	int64_t v2 = instruction->operands[2];

	if (res >= ram_size || v1 >= ram_size || v2 >= ram_size || res < 0 || v1 < 0 || v2 < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	collection[res] = collection[v1] + collection[v2];
}


void mulFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//mul (res, v1, v2) - in cell with address res add product of numbers with cells with addresses v1 and v2	
	int64_t res = instruction->operands[0];
	int64_t v1 = instruction->operands[1];
	int64_t v2 = instruction->operands[2];

	if (res >= ram_size || v1 >= ram_size || v2 >= ram_size || res < 0 || v1 < 0 || v2 < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	collection[res] = collection[v1] * collection[v2];
}

void divFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//div (res, v1, v2) - in cell with address res add division of numbers with cells with addresses v1 and v2	
	int64_t res = instruction->operands[0];
	int64_t v1 = instruction->operands[1];
	int64_t v2 = instruction->operands[2];

	if (res >= ram_size || v1 >= ram_size || v2 >= ram_size || res < 0 || v1 < 0 || v2 < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	if (collection[v2] == 0){
		free(collection);
		errx(13, "Incorrect division by 0 in %s!", program);
	}

	collection[res] = collection[v1] / collection[v2];
}

void modFunction(struct unt * instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//mod (res, v1, v2) - in cell with address res add mod of numbers with cells with addresses v1 and v2	
	int64_t res = instruction->operands[0];
	int64_t v1 = instruction->operands[1];
	int64_t v2 = instruction->operands[2];

	if (res >= ram_size || v1 >= ram_size || v2 >= ram_size || res < 0 || v1 < 0 || v2 < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	if (collection[v2] == 0){
		free(collection);
		errx(13, "Incorrect division by 0 in %s!", program);
	}

	collection[res] = collection[v1] % collection[v2];
}

void sleepFunction(struct unt* instruction, int64_t * collection, const char * program, uint32_t ram_size){

	//sleep (v) - wait v milliseconds and continue
	int64_t v = instruction->operands[0];

	if (v >= ram_size || v < 0){
		free(collection);
		errx(11, "Incorrect instruction (invalid address) in %s!", program);
	}

	time_t seconds = collection[v] / 1000;
	
	long remMilliseconds = collection[v] % 1000; 

	long remNanoseconds = remMilliseconds * 1000000;

	struct timespec req = {seconds, remNanoseconds};
	
	int res = nanosleep(&req, NULL);

	if (res != 0){
		int old_errno = errno;
		free(collection);
		errno = old_errno;
		err(14, "Error in nanosleep!");
	}
	
}

int main (int argc, char * argv[]){
	
	//Check for enough arguments
	if (argc != 2){
		errx(1, "Exactly one argument needed!");
	}

	const char * program = argv[1];

	//Open file as argument 
	int fd = open(program, O_RDONLY);

	if (fd == -1){
 		err(2, "Unable to open %s!", program);
	}

	struct stat st;

	//Stat argument
	if (fstat(fd, &st) == -1){
		//Choose not to close file descriptors explicitly as err/errx do that
		//close(fd);
		err(3, "Unable to stat %s!", program);
	}

	//Check for correct file size
	if ((st.st_size - MAGIC_SIZE - RAM_SIZE) % INSTRUCTION_SIZE != 0 ){
		errx(8, "Incorrect file format structure in %s!", program);
	}

	//Calculate total number of instructions
	const int numberOfInstructions = (st.st_size - MAGIC_SIZE - RAM_SIZE) / INSTRUCTION_SIZE;

	char magic[MAGIC_SIZE];

	//Read magic word
	ssize_t read_size = read(fd, magic, sizeof(magic));

	if (read_size == -1){
		err(4, "Unable to read from %s!", program);
	}
	else if (read_size != (ssize_t) sizeof(magic)){
		errx(5, "Unable to read enough characters in magic word from %s!", program);
	}

	//Check for correct magic word
	if (magic[0] != 'O' || magic[1] != 'R' || magic[2] != 'C'){
		errx(6, "Incorrect magic word in %s!", program);
	}

	uint32_t ram_size;

	//Read ram_size
	read_size = read(fd, &ram_size, sizeof(ram_size));

	if (read_size == -1){
		err(4, "Unable to read from %s!", program);
	}
	else if (read_size != (ssize_t) sizeof(ram_size)){
		errx(7, "Unable to read enough bytes in ram_szie from %s!", program);
	}

	//Allocate enough memory as wantedi
	int64_t * collection = malloc(ram_size * sizeof(int64_t));

	//Check if there is available allocated memory
	if (collection == NULL && ram_size != 0){
		err(15, "Unable to allocate enough memory!");
	}

	//Declare struct for reading instructions
	struct unt instruction;
	
	int currentInstruction = -1;

	//Read instructions one by one
	while ((read_size = read(fd, &instruction, sizeof(instruction))) != 0){
		
		if (read_size == -1){
			int old_errno = errno;
			//Free dynamic memory
			free(collection);
			errno = old_errno;
			err(4, "Unable to read from %s!", program);
		}
		else if (read_size != (ssize_t) sizeof(instruction)){
			free(collection);
			errx(9, "Unable to read enough bytes in from instruction from %s!", program);
		}
		
		//Increment by 1 the number of read instructions
		++currentInstruction;

		//Undertake different actions according to opcode of the instruction
		switch(instruction.opcode){
			case 0x00:
				//nop - do nothing
				break;
			case 0x95:
				//set (tar, val) - write val in cell with address tar	
				setFunction(&instruction, collection, program, ram_size);
				break;
			case 0x5D:
				//load (to, from_ptr) - copy value from from_ptr cell address into to cell address
				loadFunction(&instruction, collection, program, ram_size);
				break;
			case 0x63:
				//store (to, from_ptr) - copy value from from cell address into value of to_ptr cell address
				storeFunction(&instruction, collection, program, ram_size);
				break;
			case 0x91:
				//jmp (idx) - move to instruction with address the value of cell with address idx
				jmpFunction(&instruction, collection, program, ram_size, fd, numberOfInstructions);
				currentInstruction = collection[instruction.operands[0]] - 1;
				break;
			case 0x25:	
				//sgz (v) - move to next instruction if v is greater than 0
				if (currentInstruction != numberOfInstructions - 1){
					sgzFunction(&instruction, collection, program, ram_size, fd, st);
					if (instruction.operands[0] > 0){
						++currentInstruction;
					}
				}
			 	break;
			case 0xAD:
				//add (res, v1, v2) - in cell with address res add sum of numbers in cells with addresses v1 and v2	
				addFunction(&instruction, collection, program, ram_size);
				break;	
			case 0x33:
				//mul (res, v1, v2) - in cell with address res add product of numbers in cells with addresses v1 and v2	
				mulFunction(&instruction, collection, program, ram_size);
				break;
			case 0x04:	
				//div (res, v1, v2) - in cell with address res add division of numbers in cells with addresses v1 and v2	
				divFunction(&instruction, collection, program, ram_size);
				break;
			case 0xB5:
				//mod (res, v1, v2) - in cell with address res add modlue of numbers in cells with addresses v1 and v2	
				modFunction(&instruction, collection, program, ram_size);
				break;
			case 0xC1:
				//out (v) - print symbol on stdout from cell with address v
				outFunction(&instruction, collection, program, ram_size);
				break;
			case 0xBF:	
				//sleep (v) - wait v milliseconds and continue
				sleepFunction(&instruction, collection, program, ram_size);
				break;
			default:
				free(collection);
				errx(10, "Invalid opcode in %s!", program);	
		}
	}

	//Close file descriptor in the end
	close(fd);
	//Free dynamic memory
	free(collection);
	exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <fcntl.h>

#include "fileCompressor.h"

list_node* global_list;

typedef struct args {
	int isRecursive;
	char* flag;
	char* path;
	char* codebook;
} args;

args* newArgsObject(){
	args* newArgsObj = (args*)malloc(sizeof(args));
	newArgsObj->isRecursive = 0;
	newArgsObj->flag = NULL;
	newArgsObj->codebook = NULL;
	newArgsObj->path = NULL;
	return newArgsObj;
}

//returns args struct or NULL if error
args* parseArgs(int argc, char* argv[]){
	
	args* CLArgs = newArgsObject();

	//first find flag - should be within first 2 args (not including name of program)
	//if it is the 3rd arg, improper format
	int i;
	for(i = 1; i <= 3; i++){
		if(strcmp(argv[i], "-b") == 0){
			CLArgs->flag = "build";
			break;
		}
		else if(strcmp(argv[i], "-c") == 0){
			CLArgs->flag = "compress";
			break;
		}
		else if(strcmp(argv[i], "-d") == 0){
			CLArgs->flag = "decompress";
			break;
		}		
	}

	//have found no flag, thus return error
	if(!CLArgs->flag){
		printf("Error, no flag specified or improper flag.\n");
		return NULL;
	}

	//no recursive flag specified
	if(i == 2){
		CLArgs->isRecursive = 0;
	}
	
	//check for recursive flag
	//flag was 3rd argument, so recursive flag should be 2nd argument
	else{
		if(strcmp(argv[2], "-R") == 0){
			CLArgs->isRecursive = 1;
		}
		else{
			printf("Error, expected recursive flag as 2nd argument\n");
			return NULL;
		}
	}

	// //Check rest of arguments, starting from i+1
	// ++i;
	// for(i; i < argc; i++){

	// }

	return CLArgs;

}

int main(int argc, char **argv){

	if(strcmp(argv[1], "-R") == 0 && strcmp(argv[1], "-b") == 0){
		directoryHandler(argv[2], "index");
	}
	else if(strcmp(argv[1], "-b") == 0){
		fileHandler(argv[2], "index");
	}

	directoryHandler(argv[3], "index");

	int size = getLength(global_list);

	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;



	int fd = creat("HuffmanCodebook", mode);
	write(fd, "*\n", 2);
	HuffmanCodes(size, global_list, fd);
	close(fd);

	return 0;
}

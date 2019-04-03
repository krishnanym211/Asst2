#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

#include "fileCompressor.h"

list_node* global_list;

/*typedef struct args {
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

	//have not found flag, thus return error
	if(!CLArgs->flag){
		printf("Error, no flag specified or improper flag given.\n");
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

	++i;

	//no more args
	if(i >= argc){
		printf("Error, expected path or file argument\n");
		return NULL;
	}

	//Assign path input
	CLArgs->path = argv[i];

	//we expect no more arguments for build
	if(strcmp(CLArgs->flag, "build") == 0){
		if(i != argc - 1){
			printf("Error, too many arguments provided for build flag\n");
			return NULL;
		}
	}

	++i;

	if(i >= argc){
		printf("Error, expected path of codebook to use\n");
	}

	CLArgs->codebook = argv[i];

	return CLArgs;

}*/

//Helper used to populate two arrays of Huffman Codes and Tokens from the codebook
void populate(char ** codes, char ** tokens, char * input, int length) {
    int i = 0, j = 0;
    int last_whitespace = 0;
    int string_length = 0;
    int index = 0;
    for (i = 0; i < length - 1; ++i) {
        // check for tabs and newline characters in codebook
        if (input[i] == '\t' || input[i] == '\n') {
            //if found, seperate into two arrays
            char * string = (char *)malloc(sizeof(char) * (string_length + 1));
            for (j = 0; j < string_length; ++j) {
                string[j] = input[last_whitespace + j];
            }
            string[string_length] = '\0';
            last_whitespace += string_length + 1;
            string_length = 0;
            if (input[i] == '\t') {
                // Codebook will always have codes before the tab and tokens after.
                codes[index] = (char *)malloc(strlen(string) + 1);
                strncpy(codes[index], string, (strlen(string)));
            } else {
                tokens[index] = (char *)malloc(strlen(string) + 1);
                strncpy(tokens[index], string, (strlen(string)));
                ++index;
            }
            free(string);
            // If current char is not a delimiter, just increase the length of the string
        } else {
            ++string_length;
        }
    }
}

//counts the number of lines(codes) in the codebook
int count_codebook(char * input, int length) {
    int count = 0;
    int i = 0;
    for (i = 0; i < length; ++i) {
        /* Number of lines is equal to number of tabs in properly formatted codebook */
        if (input[i] == '\t') {
            ++count;
        }
    }
    return count;
}

// Return index of given string in array
int search(char ** arr, int size, char * string) {
    int i = 0;
    for (i = 0; i < size; ++i) {
        if (strcmp(arr[i], string) == 0) {
            
            return i;
        }
    }
    return -1;
}

int compress ( int hcz, int file, int codebook){
    //find total length of file and create a string of that size
    char * temp = (char *)malloc(sizeof(char) * INT_MAX);
    int total_length = read(file, temp, INT_MAX);
    char * input = (char *)malloc(sizeof(char) * (total_length + 1));
    strcpy(input, temp);
    free(temp);
        
    //put contents of file into one string
    temp = (char *)malloc(sizeof(char) * INT_MAX);
    int codebook_length = read(codebook, temp, INT_MAX);
    char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
    strcpy(codebook_input, temp);
    free(temp);
    
    // Get total number of tokens in codebook
    int size = count_codebook(codebook_input, codebook_length);
    char ** codes = (char **)malloc(sizeof(char *) * size);
    char ** tokens = (char **)malloc(sizeof(char *) * size);
    
    // Put tokens and codes from codebook into respective arrays
    populate(codes, tokens, codebook_input, codebook_length);
    
    int i = 0, j = 0;
    int token_length = 0;
    int last_whitespace = 0;
    // Get indices for whitespace tokens first because they are relatively common
    int space_index = search(tokens, size, "~)!(@s*#&$^");
    int tab_index = search(tokens, size, "~)!(@t*#&$^");
    int nl_index = search(tokens, size, "~)!(@n*#&$^");
    for (i = 0; i < total_length; ++i) {
        // Use previous method for tokenizing input
        if (input[i] == '\t' || input[i] == '\n' || input[i] == ' ') {
            if (token_length > 0) {
                char * string = (char *)malloc(sizeof(char) * (token_length + 1));
                for (j = 0; j < token_length; ++j) {
                    string[j] = input[last_whitespace + j];
                }
                string[token_length] = '\0';
                last_whitespace += token_length + 1;
                token_length = 0;
                /* Write the code that corresponds to the token */
                int index = search(tokens, size, string);
                if (index != -1) {
                    write(hcz, codes[index], strlen(codes[index]));
                    /* If token is not present in codebook, return with error message (should not happen under normal operation) */
                } else {
                    printf("ERROR: No corresponding code for token. Exiting.\n");
                    return -1;
                }
                free(string);
            } else {
                ++last_whitespace;
            }
            /* Also write the code that corresponds to the current whitespace */
            if (input[i] == ' ') {
                write(hcz, codes[space_index], strlen(codes[space_index]));
            } else if (input[i] == '\t') {
                write(hcz, codes[tab_index], strlen(codes[tab_index]));
            } else {
                write(hcz, codes[nl_index], strlen(codes[nl_index]));
            }
            /* If current char is not a separator, just increase token length */
        } else {
            ++token_length;
        }
    }
    return 0;
}

void decompress ( int result, int file, int codebook){
    
    //find total length of file and create a string of that size
    char * temp = (char *)malloc(sizeof(char) * INT_MAX);
    int total_length = read(file, temp, INT_MAX);
    char * input = (char *)malloc(sizeof(char) * (total_length + 1));
    strcpy(input, temp);
    free(temp);
    
    //put contents of file into one string
    temp = (char *)malloc(sizeof(char) * INT_MAX);
    int codebook_length = read(codebook, temp, INT_MAX);
    char * codebook_input = (char *)malloc(sizeof(char) * (codebook_length + 1));
    strcpy(codebook_input, temp);
    free(temp);
    
    // Get total number of tokens in codebook
    int size = count_codebook(codebook_input, codebook_length);
    char ** codes = (char **)malloc(sizeof(char *) * size);
    char ** tokens = (char **)malloc(sizeof(char *) * size);
    
    // Put tokens and codes from codebook into respective arrays
    populate(codes, tokens, codebook_input, codebook_length);
    
    int last_break = 0;
    int code_length = 0;
    int i;
    int j;
    for(i = 0; i < total_length; i++){
        ++code_length;
        char * code = (char *)malloc(sizeof(char) * (code_length + 1));
        for (j = 0; j < code_length; ++j) {
            code[j] = input[last_break + j];
        }
        code[code_length] = '\0';
        // Check if code exists within codebook; if yes, then write corresponding token
        int index = search(codes, size, code);
        if (index >= 0) {
            // If token is escape character, write corresponding whitespace
            if (strcmp(tokens[index], "~)!(@s*#&$^") == 0) {
                write(result, " ", strlen(" "));
            } else if (strcmp(tokens[index], "~)!(@t*#&$^") == 0) {
                write(result, "\t", strlen("\t"));
            } else if (strcmp(tokens[index], "~)!(@n*#&$^") == 0) {
                write(result, "\n", strlen("\n"));
            } else {
                write(result, tokens[index], strlen(tokens[index]));
            }
            // Advance the last breakpoint in input and reset length of code
            last_break += code_length;
            code_length = 0;
        }
        free(code);
    }
}

int main(int argc, char **argv){

	
	return 0;
}

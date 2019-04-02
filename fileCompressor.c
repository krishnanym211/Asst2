#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "fileCompressor.h"


int main(int argc, char **argv){
    if(argv < 3){
        printf("ERROR: Insufficient arguments.\n");
        return EXIT_FAILURE;
    } else if (argc > 5) {
        printf("ERROR: Excessive arguments.\n");
        return EXIT_FAILURE;
    }
    
    int isFile = 1;
    int isRec = 0;
    if(strcmp("-R",argv[1]) == 0 || strcmp("-R",argv[2]) == 0){
        isRec = 1;
    }
    
    char* file = argv[3 +(isRec -1)];
    char Oper = ' ';
    
    if(isRec){
        if (strcmp("-R", argv[1]) == 0) {
            Oper = argv[2][1];
        } else {
            Oper = argv[1][1];
        }
    } else {
        Oper = argv[1][1];
    }
    if(Oper != 'b' && Oper != 'c' && Oper != 'd') {
        printf("ERROR: Invalid flag(s). Please refrence Readme.txt for correct flags and thier descriptions.\n");
        return EXIT_FAILURE;
    }
    
    struct stat pstat;
    if (stat(file, &pstat) !=0){
        printf("ERROR: Directory or File "\s" doesn't exist. \n", file);
        return EXIT_FAILURE;
    }
    
    idFile = S_ISREG(pstat.st_mode);
    if(isRec && isFile){
        int isPath= 0;
        for(int i = 0; i < strlen(file); ++i){
            if(file[i] =='/'){
                isPath = 1;
                break;
            }
        }
    }
    if(file[strlen(file)-1] == '/'){
        file[strlen(file)-1]= '\0'
    }
    //same Operations with recursive flag or not
    if(isFile) {
        int fdFile = open(file, O_RDONLY);
        if (fdFile < 0) {
            printf("Error: Could not open file \"%s\". Please try again.\n", file);
            return EXIT_FAILURE;
        }
        char * temp = (char *)malloc(sizeof(char) * INT_MAX);
        int totalLength = read(fdFile, temp, INT_MAX);
        char * input = (char *)malloc(sizeof(char) * (totalLength + 1));
        strcpy(input, temp);
        free(temp);
        if (Oper == 'b') {
            if (argc > (4 + isRec - 1)) {
                printf("ERROR: Excessive arguments.\n");
                return EXIT_FAILURE;
            } else if (argc < (4 + isRec - 1)) {
                printf("ERROR: Insufficient arguments.\n");
                return EXIT_FAILURE;
            }
            int fdCodebook = open("./HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fdCodebook < 0) {
                printf("Error: Could not open or create HuffmanCodebook. Please try again.\n");
                return EXIT_FAILURE;
            }
            //Size holds the number of unique tokens
            int size = tokenize(input);
            if (size > 0) {
                HuffmanCodes(size, head, fdCodebook);
            }
            write(fdCodebook,"\n",1);
            close(fdCodebook);
        }
        if (Oper == 'c' || Oper == 'd') {
            if (argc > (5 + isRec - 1)) {
                printf("ERROR: Excessive arguments.\n");
                return EXIT_FAILURE;
            } else if (argc < (5 + isRec - 1)) {
                printf("ERROR: Insufficient arguments.\n");
                return EXIT_FAILURE;
            }
            /* Check to ensure HuffmanCodebook file was provided */
            if (strlen(argv[argc - 1]) < strlen("HuffmanCodebook")) {
                printf("Error: HuffmanCodebook file required. Please try again.\n");
                return EXIT_FAILURE;
            }
            char * testCodebook = argv[argc - 1] + (strlen(argv[argc - 1]) - 15);
            if (strcmp(testCodebook, "HuffmanCodebook") != 0) {
                printf("Error: HuffmanCodebook file required. Please try again.\n");
                return EXIT_FAILURE;
            }
            int fdCodebook = open(argv[argc - 1], O_RDONLY);
            if (fdCodebook < 0) {
                printf("Error: Could not open file \"%s\". Please try again.\n", argv[argc - 1]);
                return EXIT_FAILURE;
            }
            /* Get all contents of file all in one string using previous method */
            temp = (char *)malloc(sizeof(char) * INT_MAX);
            int codebookLength = read(fdCodebook, temp, INT_MAX);
            char * codebookInput = (char *)malloc(sizeof(char) * (codebookLength + 1));
            strcpy(codebookInput, temp);
            free(temp);
            
            int CBcount = 0;
            for(int i = 0; i< codebookLength; ++i){
                if (codebookLenght[i] =='\t' {
                    ++CBcount;
                }
            }
            int size = CBcount;
            char ** codes = (char **)malloc(sizeof(char *) * size);
            char ** tokens = (char **)malloc(sizeof(char *) * size);
            /* Put tokens and codes from codebook into respective arrays */
            populate_arrs(codes, tokens, codebookInput, codebookLength);
            if (flag == 'c') {
                /* Create new file name with .hcz extension */
                char * hczfile = strcat(file, ".hcz");
                int fd_hcz = open(hczfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd_hcz < 1) {
                    printf("Error: Could not open or create file \"%s\". Please try again.\n", hczfile);
                    return EXIT_FAILURE;
                }
                compress(fd_hcz, input, total_length, codes, tokens, size);
                close(fd_hcz);
            } else {
                char * test = (char *)malloc(sizeof(char) * 5);
                test = file + strlen(file) - 4;
                /* Ensure that file is an .hcz file */
                if (strcmp(test, ".hcz") != 0) {
                    printf("Error: Decoding requires a .hcz file.\n");
                    return EXIT_FAILURE;
                }
                /* Cut off last four chars to get rid of .hcz and make new file */
                char * resfile = file;
                resfile[strlen(file) - 4] = '\0';
                int fd_res = open(resfile, O_WRONLY | O_CREAT , 0644);
                if (fd_res < 1) {
                    printf("Error: Could not open or create file \"%s\". Please try again.\n", resfile);
                    return EXIT_FAILURE;
                }
                //call decompress
                decompress(fd_res, input, total_length, codes, tokens, size);
                close(fd_res);
            }
            close(fd_codebook);
        }
        close(fd_file);
    } else if (isRec){
        if(Oper =='b'){
            if (argc < 4) {
                printf("ERROR: Insufficient arguments.\n");
                return EXIT_FAILURE;
            } else if (argc >4) {
                printf("ERROR: Excessive arguments.\n");
                return EXIT_FAILURE;
            }
            int fdCodebook = open("./HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fdCodebook < 0) {
                printf("Error: Could not open or create HuffmanCodebook.\n");
                return EXIT_FAILURE;
            }
            //Size holds the number of unique tokens
            int size = tokenize(input);
            if (size > 0) {
                HuffmanCodes(size, head, fdCodebook);
            }
            write(fdCodebook,"\n",1);
            close(fdCodebook);
        } else if (Oper == 'c' || Oper == 'd') {
            if (argc < 5) {
                printf("Error: Not enough arguments. Please try again.\n");
                return EXIT_FAILURE;
            } else if (argc > 5) {
                printf("Error: Too many arguments. Please try again.\n");
                return EXIT_FAILURE;
            }
            /* Check to ensure HuffmanCodebook file was provided */
            if (strlen(argv[argc - 1]) < strlen("HuffmanCodebook")) {
                printf("Error: HuffmanCodebook file required. Please try again.\n");
                return EXIT_FAILURE;
            }
            char * testCodebook = argv[argc - 1] + (strlen(argv[argc - 1]) - 15);
            if (strcmp(testCodebook, "HuffmanCodebook") != 0) {
                printf("Error: HuffmanCodebook file required. Please try again.\n");
                return EXIT_FAILURE;
            }
            int fdCodebook = open(argv[argc - 1], O_RDONLY);
            if (fdCodebook < 0) {
                printf("Error: Could not open file \"%s\". Please try again.\n", argv[argc - 1]);
                return EXIT_FAILURE;
            }
            /* Get all contents of file all in one string using previous method */
            temp = (char *)malloc(sizeof(char) * INT_MAX);
            int codebookLength = read(fdCodebook, temp, INT_MAX);
            char * codebookInput = (char *)malloc(sizeof(char) * (codebookLength + 1));
            strcpy(codebookInput, temp);
            free(temp);
            
            int CBcount = 0;
            for(int i = 0; i< codebookLength; ++i){
                if (codebookLenght[i] =='\t' {
                    ++CBcount;
                }
            }
            int size = CBcount;
            char ** codes = (char **)malloc(sizeof(char *) * size);
            char ** tokens = (char **)malloc(sizeof(char *) * size);
            /* Put tokens and codes from codebook into respective arrays */
            populate_arrs(codes, tokens, codebookInput, codebookLength);
            /* Both compression and decompression can use the same recursive_function call */
            recursive_function(0, file, flag, codes, tokens, size);
            free(codes);
            free(tokens);
            close(fdCodebook);
        }
    }else {
        printf("Error: Directory or non-regular file path specified without recursive flag. Please try again.\n");
        return EXIT_FAILURE;
    }
    free(head);
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fileCompressor.h"

//Adds slash to end of directory name
char* addSlash (char* directoryName){
    int length = strlen(directoryName)+2;
    char* toReturn = (char*)malloc(sizeof(char)*length);
    snprintf(toReturn, length, "%s/", directoryName);
    return toReturn;
}

char* getPath(char* currPath, char* pathToAdd, char* typeOfPathToAdd){
	int path_size;
	if(strcmp(typeOfPathToAdd, "directory") == 0){
		path_size = (strlen(currPath)) + (strlen(pathToAdd)) + 1;
		char* pathToReturn = (char*)malloc(sizeof(char)*path_size);
		snprintf(pathToReturn, path_size, "%s%s", currPath, pathToAdd);
		return pathToReturn;
	}
	else{
		path_size = (strlen(currPath)) + (strlen(pathToAdd)) + 1;
		char* pathToReturn = (char*)malloc(sizeof(char)*path_size);		
		snprintf(pathToReturn, path_size, "%s%s", currPath, pathToAdd);
		return pathToReturn;
	}
}

int checkIfFileIsRegular(char* filename){

    struct stat pstat;
    if (stat(filename, &pstat) != 0) {
        printf("Error, cannot open file %s\n", filename);
    }
    // Check if this is a regular file
    return S_ISREG(pstat.st_mode);
}

void fileHandler(char* filename, args* arguments){
    
    printf("fileHandler: %s, mode: %s\n", filename, arguments->flag);

    if(!checkIfFileIsRegular(filename)){
        printf("File: %s is not a regular file.\n", filename);
    }

    if(strcmp(arguments->flag, "build") == 0){
        indexFile(filename);
        // Mergesort(global_list);
    }

	return;
}

void directoryHandler(char* directoryName, args* arguments){
	
	struct dirent *dirEnt;
  
    DIR *dir = opendir(directoryName);
  
    if (dir == NULL)
    {
        return;
    }

    while ((dirEnt = readdir(dir)) != NULL){
    	if (strcmp(dirEnt->d_name, ".") != 0 && strcmp(dirEnt->d_name, "..") != 0 && strcmp(dirEnt->d_name, ".DS_Store") != 0){
    		
    		if(dirEnt->d_type == DT_DIR){
                //add slash to end of directory name
                char* currDirectoryName = addSlash(dirEnt->d_name);
				char* dirPath = getPath(directoryName, currDirectoryName, "directory");
				printf("Calling directory handler on %s\n", dirPath);
				directoryHandler(dirPath, arguments);
    		}
    		else if(dirEnt->d_type == DT_REG){
    			//Add error checking for file type (i.e. valid file)
				char* filePath = getPath(directoryName, dirEnt->d_name, "file"); 
				printf("Calling file handler on %s\n", filePath);
				fileHandler(filePath, arguments);
                // Mergesort(globalList);
    		}

    		//neither a directory nor a file
    	}
    }
    closedir(dir);
    return;
}



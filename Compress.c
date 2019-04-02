#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
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

void fileHandler(char* filename, char* mode){
    if(strcmp(mode, "index") == 0){
        indexFile(filename);
    }
	return;
}

void directoryHandler(char* directoryName, char* mode){
	
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
				directoryHandler(dirPath, mode);
    		}
    		else if(dirEnt->d_type == DT_REG){
    			//Add error checking for file type (i.e. valid file)
				char* filePath = getPath(directoryName, dirEnt->d_name, "file"); 
				printf("Calling file handler on %s\n", filePath);
				fileHandler(filePath, mode);
                // Mergesort(globalList);
    		}

    		//neither a directory nor a file
    	}
    }

    closedir(dir);
    return;
}

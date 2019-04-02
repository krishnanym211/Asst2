#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "fileCompressor.h"


int main(int argc, char **argv){
	directoryHandler("test/", "index");
	return 0;
}
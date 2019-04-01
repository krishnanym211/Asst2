#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

typedef struct MinHeap {
	int a;
} MinHeap;

typedef struct Node {
	int a;
} Node;

Node* wordFrequenices;

MinHeap* insertIntoMinHeap(MinHeap* base, MinHeap* toAdd);

void buildCodebook();
//Tokenizer function

//Adds every token from file to global linked list (containing word frequencies)
int indexFile(char* filename);


void buildHuffmanTree();

//Creates codebook using linked list of frequencies across all files
void buildCodebook();

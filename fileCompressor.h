#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void fileHandler(char* filename, char* mode);

void directoryHandler(char* directoryName, char* mode);

int indexFile(char* filename);

//node on the Huffman Tree that carries the word, the frequency and the links to right and left children
typedef struct MinheapNode {
    char* token;
    unsigned int freq;
    struct MinheapNode *left, *right;
} MinheapNode;

//A collection of the Minheap Nodes pointing to each other
typedef struct Minheap {
    unsigned int size;
    //maximum capacity of nodes in the tree
    unsigned int cap;
    MinheapNode** tree;
} Minheap;

//Structure for the linked list that has the tokens with frequenices in lowest to highest order
typedef struct list_node {
    unsigned int freq;
    char* token;
    struct list_node* next;
} list_node;

extern list_node* global_list;

int getLength(list_node* head);

void HuffmanCodes(int size, list_node* node, int fileDir);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fileCompressor.h"

//INDEXING CODE

int errno;

list_node* global_list;

list_node* new_list_node(int freq, char* token){
    list_node* new = (struct list_node*)malloc(sizeof(list_node));
    new->freq = freq;
    new->token = strdup(token);
    new->next = NULL;
    return new;
}

list_node* copy_list_node(list_node* original){
    list_node* copy = (struct list_node*)malloc(sizeof(list_node));
    copy->freq = original->freq;
    copy->token = strdup(original->token);
    copy->next = NULL;
    return copy;
}

int getLength(list_node* head){
    if(!head){
        return 0;
    }
    list_node* ptr;
    int length = 0;
    for(ptr = head; ptr != NULL; ptr = ptr->next){
        length++;
    }
    return length;
}

void printLL(list_node* head){
    list_node* ptr;
    int length = getLength(head);
    for(ptr = head; ptr != NULL; ptr = ptr->next){
        printf("Token: %s Frequency: %d\n", ptr->token, ptr->freq);
    }
}

list_node* merge(list_node* left, list_node* right){

    list_node* mergedListHead;
    list_node* mergedListPtr;
    list_node* leftPtr = left;
    list_node* rightPtr = right;

    /* Initialize mergedListHead */
    if(leftPtr->freq <= rightPtr->freq){
        mergedListHead = copy_list_node(leftPtr);
        leftPtr = leftPtr->next;
    }
    else{
    mergedListHead = copy_list_node(rightPtr);
        rightPtr = rightPtr->next;
    }

    mergedListPtr = mergedListHead;

    while(leftPtr || rightPtr){
        if(!leftPtr){
            mergedListPtr->next = copy_list_node(rightPtr);
            rightPtr = rightPtr->next;
        }
    else if(!rightPtr){
            mergedListPtr->next = copy_list_node(leftPtr);
            leftPtr = leftPtr->next;
        }
    else if(leftPtr->freq <= rightPtr->freq){
            mergedListPtr->next = copy_list_node(leftPtr);
            leftPtr = leftPtr->next;
        }
    else{
            mergedListPtr->next = copy_list_node(rightPtr);
            rightPtr = rightPtr->next;
        }
    mergedListPtr = mergedListPtr->next;
    }
    return mergedListHead;
}

list_node* Mergesort (list_node* head){

    int length = getLength(head);
    if(length < 2){
        return head;
    }

    int midpoint = length/2;
    int list_nodeNum = 1;
    list_node* ptr = head;
    while(list_nodeNum < midpoint){
        ptr = ptr->next;
        list_nodeNum++;
    }
    list_node* left = head;
    list_node* endOfLeft = ptr;
    list_node* right = ptr->next;
    endOfLeft->next = NULL;
    list_node* leftSorted = Mergesort(left);
    list_node* rightSorted = Mergesort(right);
    return merge(leftSorted, rightSorted);
}

//inputPos points to either start of string, or latest position
char* tokenizer(char** inputPos){
    //essentially, spaces, tabs, and newlines are hard coded delimeters -
    // but they are delimeters which are returned as tokens

    char* string = *inputPos;
    
    char* token;
    
    int tokenSize = 0;
    int i = 0;
    char curr;

    //increment tokenSize until end of token
    while(i < strlen(string)){
        
        //set curr traverser to current position, increment tokenSize
        curr = string[i];
        ++tokenSize;

        //delimeter has been reached
        if(curr == ' ' || curr == '\t' || curr == '\n'){
            //current token is delimter
            if(tokenSize == 1){
                break;
            }
            //reached delimeter, i.e. end of current token
            else{
                //do not include delimeter in current token
                tokenSize -= 1;
                break;
            }
        }
        ++i;
    }

// important: modify input pointer to start of next token

    //should never be greater than, but may be equal to
    if(tokenSize >= strlen(string)){
        //this tells caller to stop tokenizing
        *inputPos = NULL;
    }

    else{
        *inputPos = *inputPos + tokenSize;
    }

    //nothing to return
    if(tokenSize == 0){
        return NULL;
    }

    //copies token from origin string to return string

    else {
        token = (char*)calloc(tokenSize+1, sizeof(char));
        memcpy(token, string, tokenSize);
    }   

    return token;
}

//returns 1 if reached end of file, 0 if not
int read_line(int fd, char** lineBufferPtr){

    char* lineBuffer = *lineBufferPtr;

    char* curr = (char*)calloc(2, sizeof(char));

    int i = 0;
    int bytes;

    while(curr[0] != '\n' && i < 31999){
        bytes = read(fd, curr, 1);
        //end of file
        if(bytes == 0){
            //null terminate string
            lineBuffer[i] = '\0';
            return 1;
        }
        lineBuffer[i] = curr[0];
        i++;
    }

    //null terminate string
    lineBuffer[i] = '\0';
    return 0;
}

void addTokenToIndex(char* token){

    if(!global_list){
        global_list = new_list_node(1, token);
        return;
    }
    
    list_node* ptr;
    list_node* prev;

    for(ptr = global_list; ptr != NULL; ptr = ptr->next){
        if(strcmp(token, ptr->token) == 0){
            ptr->freq += 1;
            return;
        }
        prev = ptr;
    }

    //word is not in list
    prev->next = new_list_node(1, token);
    return;

}

//Returns 1 on successful execution, 0 otherwise
int indexFile(char* filename){

    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1){
        perror(strerror(errno));
        printf("File: %s was unable to be opened\n", filename);
        return 0;
    }

    char* token;

    // char* str = "\tSample string which\nmimics a file.\n";

    char* lineBuffer = (char*)calloc(32000, sizeof(char));
    char** lineBufferPtr = &lineBuffer;

    int endOfFile;
    int numberOfLines = 0;
    while(endOfFile != 1){
        endOfFile = read_line(fd, lineBufferPtr);
        
        if(numberOfLines == 0 && endOfFile){
            printf("File: %s is empty.\n", filename);
            return 1;
        }

        ++numberOfLines;
        //create a duplicate so as to not modify string, make pointer to string
        char* copyOfString = strdup(lineBuffer);
        char** strPtr = &(copyOfString);
        // printf("Line: |%s|\n", lineBuffer);
        while(*strPtr){
            if((token = tokenizer(strPtr)) != NULL){
                printf("Token: |%s|\n", token);
                addTokenToIndex(token);
            }
        }
    }

    printLL(global_list);

    return 1;
}

//INDEXING CODE



//Creates a new Node on tree using data and frequency provided
MinheapNode* createNode (char* word, unsigned int freq){
    MinheapNode* node = (MinheapNode*) malloc (sizeof(MinheapNode));
    node->left = node->right = NULL;
    node->token = word;
    node->freq = freq;
    
    return node;
}

//creates a minheap with the max capacity provided
Minheap* createMinheap(unsigned int cap){
    Minheap* minheap = (Minheap*) malloc (sizeof(Minheap));
    minheap->size = 0;
    minheap->cap = cap;
    minheap->tree = (MinheapNode**) malloc(minheap->cap * sizeof(MinheapNode*));
    return minheap;
}

//swaps to minheap nodes
void swapNodes(MinheapNode** first, MinheapNode** second){
    MinheapNode* temp = *first;
    *first = *second;
    *second = temp;
}

//performs basic heapify algorithm to sort it into a min heap
void heapify(Minheap* minheap, int index){
    int floor = index;
    int left = 2*index+1;
    int right = left +1;
    
    if(left < minheap->size && minheap->tree[left] != NULL && minheap->tree[floor] != NULL && minheap->tree[left]->freq < minheap->tree[floor]->freq){
        floor = left;
    }
    if(right < minheap->size && minheap->tree[right] != NULL && minheap->tree[floor] != NULL && minheap->tree[right]->freq < minheap->tree[floor]->freq){
        floor = right;
    }
    if(minheap->tree[floor] != NULL && minheap->tree[index] != NULL && floor != index){
        swapNodes(&minheap->tree[floor],&minheap->tree[index]);
        heapify(minheap, floor);
    }
}

//boolean to check if heap is of size one
int isSizeOne(Minheap* minHeap){
    return (minHeap->size ==1);
}

//Helper to remove minimum value item from the heap
MinheapNode* removeMin (Minheap* minheap){
    MinheapNode* ptr = minheap->tree[0];
    minheap->tree[0] = minheap->tree[minheap->size -1];
    
    --minheap->size;
    heapify(minheap,0);
    return ptr;
}

//helper to new node into the minheap
void insertMinNode (Minheap* minheap, MinheapNode* minheapNode){
    ++minheap->size;
    int x = minheap->size-1;
    while (x && minheapNode->freq <minheap->tree[(x-1)/2]->freq){
        minheap->tree[x] = minheap->tree[(x-1)/2];
        x = (x-1)/2;
    }
    minheap->tree[x] = minheapNode;
}

//builds the minheap
void buildMinheap (Minheap* minheap){
    int x = minheap->size - 1;
    int i;
    
    for (i = (x - 1) / 2; i >= 0; --i){
        heapify(minheap, i);
    }
}

//boolean to determine if it is a leaf node or not
int isLeaf (MinheapNode* root){
    return !(root->left) && !(root->right);
}

//creates a minheap with capacity of the size and inserts all element of the list into it
Minheap* fullMinheap(unsigned int size, list_node* node){
    Minheap* minheap = createMinheap(size);
    list_node* ptr = node;
    int i;
    for (i = 0; i < size; ++i){
        minheap->tree[i] = createNode(ptr->token, ptr->freq);
        ptr = ptr->next;
        if(ptr==NULL){
            break;
        }
    }
    minheap->size = size;
    buildMinheap(minheap);
    return minheap;
}

MinheapNode* buildHuffmanTree( unsigned int size, list_node* node){
    MinheapNode *left, *right, *parent;
    Minheap * root = fullMinheap(size, node);
    while (!isSizeOne(root)) {
        left = removeMin(root);
        right = removeMin(root);
        int leftfreq = 0, rightfreq = 0;
        if (left != NULL) {
            leftfreq = left->freq;
        }
        if (right != NULL) {
            rightfreq = right->freq;
        }
        char temp[3] = "/\\\0";
        parent = createNode(temp, leftfreq + rightfreq);
        parent->left = left;
        parent->right = right;
        insertMinNode(root, parent);
    }
    return removeMin(root);
}

void writeCodes (MinheapNode* node, unsigned short code_arr[], int parent, int fileDir){
    if (node->left != NULL) {
        code_arr[parent] = 0;
        writeCodes(node->left, code_arr, parent+1, fileDir);
    }
    if (node->right != NULL) {
        code_arr[parent] = 1;
        writeCodes(node->right, code_arr, parent+1, fileDir);
    }
    if (node->left == NULL && node->right == NULL) {
        int x = 0;
        char buf[parent];
        int count = 0;
        for (x = 0; x < parent; ++x) {
            if (code_arr[x] == 1 || code_arr[x] == 0) {
                buf[x] = code_arr[x] + '0';
                ++count;
            } else {
                break;
            }
        }
        write(fileDir, buf, count);
        write(fileDir, "\t", 1);
        write(fileDir, node->token, strlen(node->token));
        write(fileDir, "\n", 1);
    }
}

// void HuffmanCodes(int size, list_node* node, int fileDir){
//     MinheapNode* floor = buildHuffmanTree(size, node);
//     unsigned short codes[size];
//     writeCodes(floor, codes, 0, fileDir);
// }

//used for testing the Huffman Coding
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    
    printf("\n");
}

void printCodes(MinheapNode* root, int arr[], int top){
    
    // Assign 0 to left edge and recur
    if (root->left) {
        
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    
    // Assign 1 to right edge and recur
    if (root->right) {
        
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    
    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
        
        printf("%s: ", root->token);
        printArr(arr, top);
    }
}

void HuffmanCodes(list_node* data, unsigned int size){
    // Construct Huffman Tree
    MinheapNode* root = buildHuffmanTree(size, data);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[size];

    printCodes(root, arr, 0);
}

// int main() {
    
//     // list_node* head = NULL;
//     // list_node* a = NULL;
//     // list_node* b = NULL;
//     // list_node* c = NULL;
//     // list_node* d = NULL;
//     // list_node* e = NULL;
    
//     // // allocate 6 nodes in the list
//     // head  = (list_node*)malloc(sizeof(list_node));
//     // a = (list_node*)malloc(sizeof(list_node));
//     // b = (list_node*)malloc(sizeof(list_node));
//     // c = (list_node*)malloc(sizeof(list_node));
//     // d = (list_node*)malloc(sizeof(list_node));
//     // e = (list_node*)malloc(sizeof(list_node));
    
    
//     // head->token = "a"; //assign data in first node
//     // head->next = a; // Link first node with second
//     // head->freq = 5;
    
//     // a->token = "dog"; //assign data in first node
//     // a->next = b; // Link first node with second
//     // a->freq = 9;
    
//     // b->token = "cat"; //assign data in first node
//     // b->next = c; // Link first node with second
//     // b->freq = 12;
    
//     // c->token = "button"; //assign data in first node
//     // c->next = d; // Link first node with second
//     // c->freq = 13;
    
//     // d->token = "ball"; //assign data in first node
//     // d->next = e; // Link first node with second
//     // d->freq = 16;
    
//     // e->token = "and"; //assign data in first node
//     // e->next = NULL; // Link first node with second
//     // e->freq = 45;


    
//     // unsigned int size = 6;

//     indexFile("test2.txt");
//     int size = getLength(global_list);
//     if(global_list){
//         HuffmanCodes(global_list, size);
//     }
    
//     return 0;
// }


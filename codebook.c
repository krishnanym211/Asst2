#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codebook.h"

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
    
    for (int i = 0; i < size; ++i){
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
        set_codes(node->left, code_arr, parent+1, fileDir);
    }
    if (node->right != NULL) {
        code_arr[parent] = 1;
        set_codes(node->right, code_arr, parent+1, fileDir);
    }
    if (node->left == NULL && node->right == NULL) {
        int x = 0;
        char buf[parent];
        int count = 0;
        for (x = 0; i < parent; ++x) {
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

void HuffmanCodes(int size, list_node* node, int fileDir){
    MinheapNode* floor = buildHuffmanTree(size, root);
    short codes[size];
    writeCodes(base, codes, 0, fileDir);
}

//used for testing the Huffman Coding
/*void printArr(int arr[], int n)
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

int main() {
    
    list_node* head = NULL;
    list_node* a = NULL;
    list_node* b = NULL;
    list_node* c = NULL;
    list_node* d = NULL;
    list_node* e = NULL;
    
    // allocate 6 nodes in the list
    head  = (list_node*)malloc(sizeof(list_node));
    a = (list_node*)malloc(sizeof(list_node));
    b = (list_node*)malloc(sizeof(list_node));
    c = (list_node*)malloc(sizeof(list_node));
    d = (list_node*)malloc(sizeof(list_node));
    e = (list_node*)malloc(sizeof(list_node));
    
    
    head->token = "a"; //assign data in first node
    head->next = a; // Link first node with second
    head->freq = 5;
    
    a->token = "dog"; //assign data in first node
    a->next = b; // Link first node with second
    a->freq = 9;
    
    b->token = "cat"; //assign data in first node
    b->next = c; // Link first node with second
    b->freq = 12;
    
    c->token = "button"; //assign data in first node
    c->next = d; // Link first node with second
    c->freq = 13;
    
    d->token = "ball"; //assign data in first node
    d->next = e; // Link first node with second
    d->freq = 16;
    
    e->token = "and"; //assign data in first node
    e->next = NULL; // Link first node with second
    e->freq = 45;
    
    unsigned int size = 6;
    
    HuffmanCodes(head, size);
    
    return 0;
}*/


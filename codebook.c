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
    int smallest = index;
    int left = 2*index+1;
    int right = left +1;
    
    if((left< minheap->size) && (minheap->tree[left]->freq < minheap->tree[smallest]-> freq)){
        smallest = left;
    }
    if((right< minheap->size) && (minheap->tree[left]->freq < minheap->tree[smallest]-> freq)){
        smallest = right;
    }
    if(smallest != index){
        swapNodes(&minheap->tree[smallest],&minheap->tree[index]);
        heapify(minheap, smallest);
    }
}

//boolean to check if heap is of size one
int isSizeOne(MinHeap* minHeap){
    return (minHeap->size ==1);
}

//Helper to remove minimum value item from the heap
MinheapNode* removeMin (Minheap* minheap){
    MinheapNode* ptr = minheap->tree[0];
    minheap->tree[0] = mineahp->tree[minheap->size -1];
    
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
void buildMinheap (Minheap* minhead){
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
    MinHeap* minheap = createMinHeap(size);
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
        left = removemin(root);
        right = removemin(root);
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
    short code[size];
    writeCodes(base, codes, 0, fileDir);
}



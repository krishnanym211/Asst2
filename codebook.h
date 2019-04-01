
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
typedef struct node {
    unsigned int freq;
    char* token;
    struct list_node* next;
} list_node;

void HuffmanCodes(int size, list_node* node, int fileDir);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int errno;

//Structure for the linked list that has the tokens with frequenices in lowest to highest order
typedef struct list_node {
    unsigned int freq;
    char* token;
    struct list_node* next;
} list_node;

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
            return 1;
        }
        lineBuffer[i] = curr[0];
        i++;
    }

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

int indexFile(char* filename){

    int fd;
    if ((fd = open("test_file.txt", O_RDONLY)) == -1){
        perror(strerror(errno));
    }

    char* token;

    // char* str = "\tSample string which\nmimics a file.\n";

    char* lineBuffer = (char*)calloc(32000, sizeof(char));
    char** lineBufferPtr = &lineBuffer;

    int endOfFile;
    while(endOfFile != 1){
        endOfFile = read_line(fd, lineBufferPtr);

        //create a duplicate so as to not modify string, make pointer to string
        char* copyOfString = strdup(lineBuffer);
        char** strPtr = &(copyOfString);
        // printf("String: %s\n", lineBuffer);
        while(*strPtr){
            token = tokenizer(strPtr);
            // printf("Token: %s\n", token);
            addTokenToIndex(token);
        }
    }

    printLL(global_list);

    return 1;
}

int main(int argc, char **argv){
    indexFile("Hello");
	return 0;
}
/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>


/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096
int va;

typedef struct seglistnode{
        int start;
        int size;
        int Holebool;
        struct seglistnode *prev;
        struct seglistnode *next;
    }seglistnode;

typedef struct freelistnode{
    unsigned long pconverter;
    int pages;
    seglistnode *seghead;
    struct freelistnode *prev;
    struct freelistnode *next;
}freelistnode;


freelistnode *head, *tail;

int pagesused = 0;
int spaceunused = 0;
int mainchainlength =0;
int subchainlengtharray[100000];



// long mapping;





/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init(){
    head = NULL; 
    tail = head;
    va = 1000;
}


/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
    
}


/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 

void* mems_malloc(size_t size){
    int isspaceinhole = 0; 
    if(head == NULL){     
        va = 1000; 
                                  /* or no space in freelist to be implemented*/
        addNewChain(size);
        
    }
    else{
        freelistnode *current = head;

    // Traverse forward
        // printf("All traversal:\n");
        while (current != NULL) {
            seglistnode *currentseg = current->seghead;
            while(currentseg != NULL) {
                if(currentseg->Holebool == 1){
                    if(currentseg->size>size){
                        int old_size = currentseg->size;
                        currentseg->size = size;
                        currentseg->Holebool = 0;
                        seglistnode *newnode = mmap(NULL, sizeof(seglistnode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                            if (newnode == MAP_FAILED) {
                                perror("mmap failed");
                                return NULL;
                            }
                        int a = currentseg->start;
                        newnode->next = currentseg->next;
                        currentseg->next = newnode;
                        newnode->Holebool = 1;
                        newnode->start = currentseg->start + currentseg->size;
                       
                        newnode->size = old_size - size;
                        newnode->prev = currentseg;
                        // newnode->next = NULL;
                        isspaceinhole = 1;
                        void* returnvalue = a;
                        return returnvalue;
                    }
                }
                currentseg = currentseg->next;
            }
        
            
            current = current->next;
        }
        
        
        if(isspaceinhole == 0){
            freelistnode *current = head;

    // Traverse forward
            // printf("All traversal:\n");
            while (current->next != NULL) {
                
                
                current = current->next;
                }
                va = va+ (current->pages)*PAGE_SIZE;
            
            addNewChain(size);
        }
    }

    // else if(isspaceinhole == 0){
    //     addNewChain(size);

    // }
}


/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats(){
    
}


/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void*v_ptr){
    
}


/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr){
    int v_a = v_ptr;
    freelistnode *current = head;

    
    
    while (current != NULL) {
        seglistnode *currentseg = current->seghead;
        while(currentseg != NULL) {
            if(currentseg->start == v_a) {
                currentseg->Holebool = 1; 
            }
            currentseg = currentseg->next;
        }
        // printf("\n");
        
        current = current->next;
    }
    combineHoles();
    combineHoles();
    
}
void combineHoles(){
    freelistnode *current = head;
    // printf("a");
  
    while (current != NULL) {
        seglistnode *currentseg = current->seghead;
        
        while(currentseg->next != NULL && currentseg != NULL) {
            // seglistnode  *temp = currentseg->next;
            if(currentseg->Holebool == 1 && currentseg->next->Holebool == 1){
                seglistnode  *temp = currentseg->next;
                currentseg->size = currentseg->size+ temp->size;
                
                currentseg->next = temp->next;
                // free(temp);
                // currentseg->next = currentseg->next->next;
                // if(currentseg->next->next != NULL){
                // currentseg->next->next->prev = currentseg;}
            }
            else{
                currentseg = currentseg->next;
            }

            
        }
        
        
        current = current->next;
    }
}
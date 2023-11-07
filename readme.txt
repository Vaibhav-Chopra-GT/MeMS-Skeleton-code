Documentation for MeMS
----------------------------
This project has a header file called mems.h which consists of all the inner workings and functionalities of the Memory management system.
The task performed is essentially allocation and deallocation of memory. There is a doubly linked list data structure which stores the status
of various sections of the memory alloted and other specifics. A variable in the sub linked list node stores whether the memory has been 
allocated to a process or not(is a hole), etc. These are the two main data structures.

The main list/freelist nodes are defined as "struct freelistnode". This has variables like -
                                    
                                    pages - number of pages alloted
                                    pconverter - The starting MeMS physical address of each main node to eventually convert
                                    virtual addresses into physical addresses
                                    Seghead - head pointer of segment list
                                    etc.(prev, next)

The struct seglistnode contains variables like- 

                                    holebool - if the memory section(subnode or sublist node) has 
                                    been alloted to a process. (Note: no actual allocation)
                                    base and bounds in the form of start and size etc.

The working of the program -
---------------------------------
The crux of the program is the ---mems_malloc--- function. This function first uses mmap to allot the no. of pages required for
the given size parameter. For eg. if the size given is 1000 it provides an entire page of 4096 (unit is bytes) which is the pagesize
defined as a MACRO. This is because of the restriction that only multiples of PAGESIZE can be given as arguments to mmap. Then the
function makes a main list node if head(global variable pointing to the head of the main list) is null. If head is non null it looks 
for holes big enough to accommodate the required size. Basic framework-

                                    -iterate through the main list
                                    -for each node in main list iterate through its sublist
                                    -if the size of a sublist PROCESS node is greater than required size-
                                                        spacefound = 1 (kind of like a flag)
                                    -else keep going
                                    -if eventually no space is found make a new main node

Then the deletion and merging of holes is taken care of inside mems_free. A hole is merged/combined with others only when deletion takes
place. This is why the framework for mems_malloc works, because we consider a hole as one segment only(which has been taken care of
during deletion) Also, a situation like this can arise solely because of deletion.


The function combineholes -
---------------------------
The function iterates over a linked list of freelistnode objects, where each node contains a pointer to a linked list of seglistnode
objects. The function then iterates over each seglistnode object in the linked list and checks if the current node and the next node
are both holes (i.e., holebool is 1). If so, the function combines the two holes by adding the size of the next node to the size of
the current node and updating the next pointer of the current node to point to the node after the next node. 
Finally, the function moves on to the next freelistnode object in the linked list.


mems_finish
------------
This clears up using munmap all the space allocated using mmap. This includes the freelist and sublists.

example2.c is the sample testcase file
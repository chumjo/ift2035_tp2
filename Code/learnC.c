#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
    //newBlock *find;


    //int *addr = mymalloc(8000);
    int *addr2 = mymalloc(sizeof(int));
    int *addr1 = mymalloc(sizeof(int)*5000);

    //char *src = mmap(0, sizeof(block2), PROT_READ, MAP_SHARED, -1, 0);

    //printf("Sorry, can't talk now.");
    //printf("I'm busy!");
    return(0);
}

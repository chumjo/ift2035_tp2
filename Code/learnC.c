#include <stdio.h>
#include <stdlib.h>
#include "mymallock.h"


int main()
{
    typedef struct block *newBlock;
    typedef struct block block;

    //newBlock *find;

    struct blockk        {
            size_t size;
            int free;
            struct block *next, *pred;
        };

    int tm = 1876870;
    int *dix = &tm;
    int page = 4096;


    //struct block block1 = {*dix, sizeof(1024), false, NULL};
    newBlock block1 = mallock(sizeof(*block1));
    block1->size = 1000;
    block1->free = 0;

    newBlock block2 = mallock(sizeof(*block2));
    newBlock block3 = mallock(sizeof(*block3));


    //struct block block2 = {*dix, sizeof(1024), false, block2};

    printf("The size of a block is %i\n", sizeof(block1));
    printf("The size of a block is %i\n", sizeof(block2));
    printf("The size of a block is %i\n", sizeof(block3));
    printf("The size of a block is %i\n", sizeof(struct block));

    printf("page - 2*block : %i\n", page-(2*sizeof(block2)));

    //int *addr = mymallock(8000);
    int *addr2 = mymallock(sizeof(int));
    int *addr1 = mymallock(4096);

    //char *src = mmap(0, sizeof(block2), PROT_READ, MAP_SHARED, -1, 0);

    //printf("Sorry, can't talk now.");
    //printf("I'm busy!");
    return(0);
}

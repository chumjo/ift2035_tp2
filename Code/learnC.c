#include <stdio.h>
#include <stdlib.h>
//#include "mymalloc.h"


int main()
{

	if(!NULL)
		printf("!NULL\n");

	if(1)
		printf("1\n");

	if(0)
		printf("0\n");
/*	int *a = mymalloc(sizeof(int));
	int *b = mymalloc(sizeof(int));
	int *c = mymalloc(sizeof(int));
	int *d = mymalloc(sizeof(int));

	myfree(a);
	myfree(b);

	typedef struct block
    {
        size_t size;
        struct block *free;
        struct block *next, *pred;
    } *Block;

    printf("Size of struct :%i\n", sizeof(struct block));*/

    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
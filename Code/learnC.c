#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{

	int *a = mymalloc(sizeof(int)*256);

	printList();
	
	int *b = mymalloc(sizeof(int)*256);
	int *c = mymalloc(sizeof(int)*256);
	int *d = mymalloc(sizeof(int)*256);
	int *e = mymalloc(sizeof(int)*256);

	printList();

    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
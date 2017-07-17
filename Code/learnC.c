#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
	int *a = mymalloc(sizeof(int) * 1024 * 1024);

    for(int j = 0; j < 1024 * 1024; j ++){
        a[j]= j;
    }
    myfree(a);

    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
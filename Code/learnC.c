#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
	
	int *addr1 = mymalloc(sizeof(int));
    int *addr2 = mymalloc(512);

    *addr1 = 1980;

    myfree(addr2);
    myfree(addr1);

    printf("On est sortie!!\n");

    printf("addr1 : %i = %i\n", addr1, *addr1);

    int *addr3 = mymalloc(512);

    *addr3 = 2007;

    printf("addr3 : %i = %i\n", addr3, *addr3);
    printf("addr1 : %i = %i\n", addr1, *addr1);

/*	for (int i = 0; i < 50000; i++){

		printf("boucle1 : %i\n", i);

    	int *a = mymalloc(sizeof(int) * 512);
	    for(int j = 0; j < 512; j ++){
	    	//printf("boucle2 : %i\n", j);
	    	a[j]= j;
	    }
    	myfree(a);
	}*/

/*	printf("COOL\n");


	int *b = mymalloc(sizeof(int) * 1024 * 1024);

	for(int j = 0; j < 1024 * 1024; j ++){
    	b[j]= j;
  	}
 	myfree(b);


 	printf("COOL 2\n");*/

    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
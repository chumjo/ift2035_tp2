#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
	
/*	int *addr1 = mymalloc(512);
    int *addr2 = mymalloc(512);
    int *addr3 = mymalloc(512);

    *addr1 = 1980;
    *addr2 = 1994;
    *addr3 = 2007;

    myfree(addr2);
    myfree(addr1);



    int *addr4 = mymalloc(1024);

    printf("addr1 : %i = %i\n", addr1, *addr1);

    *addr4 = 2035;

    printf("addr4 : %i = %i\n", addr4, *addr4);
    printf("addr1 : %i = %i\n", addr1, *addr1);*/


/*	for (int i = 0; i < 500000; i++){

		printf("boucle1 : %i\n", i);
    	int *a = mymalloc(sizeof(int) * 512);

	    for(int j = 0; j < 512; j ++){
	    	//printf("boucle2 : %i\n", j);
	    	a[j]= j;
	    }
    	myfree(a);
	}*/

	for (int i = 0; i < 500000; i++){
    	int *a = mymalloc(sizeof(int) * 512);
    	for(int j = 0; j < 512; j ++){
    		a[j]= j;
    	}
    	myfree(a);
  	}

  	printf("COOL!!!\n");



/*
	printf("COOL\n");


	int *b = mymalloc(sizeof(int) * 1024 * 1024);

	printf("COOL\n");


	for(int j = 0; j < 1024 * 1024; j ++){
    	b[j]= j;
  	}

  	printf("COOL\n");

 	myfree(b);


 	printf("COOL 2\n");*/

    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
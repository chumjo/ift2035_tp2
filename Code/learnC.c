#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{

/*	int *a = mymalloc(sizeof(int));
	int *b = mymalloc(sizeof(int));
	int *c = mymalloc(sizeof(int));

	myfree(b);

	printList();
    printLast();
    printFree();
    printLastFree();

    printf("--------------------------------------------------------------------------\n");

	b = mymalloc(sizeof(int));

	printList();
    printLast();
    printFree();
    printLastFree();*/

/*    int *a = mymalloc(sizeof(int) * 1024 * 1024);
    int *b = mymalloc(sizeof(int) * 1024 * 1024);
    int *c = mymalloc(sizeof(int) * 1024 * 1024);
    int *d = mymalloc(sizeof(int) * 1024 * 1024);
    int *e = mymalloc(sizeof(int) * 1024 * 1024);

    printList();
    printFree();

    myfree(a);
    myfree(b);
    myfree(c);
    myfree(d);

    printList();
    printFree();

    a = mymalloc(sizeof(int) * 1024 * 1024);
    b = mymalloc(sizeof(int) * 1024 * 1024);
    c = mymalloc(sizeof(int) * 1024 * 1024);
    d = mymalloc(sizeof(int) * 1024 * 1024);

    printList();
    printFree();
*/

	for (int i = 0; i < 10; i++){
		printf("boucle1 : %i\n", i);

		int *a = mymalloc(sizeof(int) * 1024 * 1024);
		//int *a = mymalloc(sizeof(int) * 32);

		printf("adresse = %i\n", a);
    	for(int j = 0; j < 1024 * 1024; j += 1000){
    	//for(int j = 0; j < 32; j ++){
    		a[j]= j;
    	}
    	myfree(a);
	}

	printList();
	printFree();
	printLastFree();



    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
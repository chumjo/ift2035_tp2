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

    printf("--------------------------------------------------------------------------\n");

	b = mymalloc(sizeof(int));

	printList();
    printLast();
    printFree();*/

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
    printFree();*/
/*	int *a = mymalloc(sizeof(int) * 128);

	for (int i = 0; i < 10; i++){
		printf("boucle1 : %i\n", i);

		a = mymalloc(sizeof(int) * 128);
		//int *a = mymalloc(sizeof(int) * 32);
		printf("adresse = %i\n", a);

	   	for(int j = 0; j < 1024 * 1024; j += 1000){
    	//for(int j = 0; j < 32; j ++){
    		a[j]= j;
    	}
    	//myfree(a);
	}

	myfree(a);

	printList();
	printFree();*/



typedef struct node node;
struct node {
  int valueInt;
  char valueChar;
  node *next;
  node *previous;
};

// Petit programme pour testé si malloc et free fonctionne bien
  // La racine
  node *root = mymalloc(sizeof(node));
  root->valueInt = 0;
  root->valueChar = 115;
  root->next = NULL;
  root->previous = NULL;

  printf("Et de 1...\n");
  // Création d'une liste doublement chaînée
  node *current = root;
  for(int i = 0; i < 20; i ++){
    node *p = mymalloc(sizeof(node)*128);
    p->valueInt = i;
    p->valueChar = 115;
    p->next = NULL;
    p->previous = current;
    current->next = p;
    current = p;
  }

  printList();
  printFree();
  printLast();
  printLastFree();

  printf("Et de 2...\n");
/*  // On efface quelques éléments
  current = root;
  int n = 0;
  while (current->next){
    n++;
    if (n % 4 != 0) {
      // Rien à faire
      current = current->next;
    }
    else {
      // On efface ce noeud
      node *tmp;
      current->previous->next = current->next;
      current->next->previous = current->previous;
      tmp = current->next;
      myfree(current);
      current = tmp;
    }
}*/
 


    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
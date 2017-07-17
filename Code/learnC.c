#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
    //newBlock *find;


    //int *addr = mymalloc(8000);

    myfree(NULL);

    int *a = malloc(sizeof(int));
    myfree(a);
    free(a);

    int *b = mymalloc(sizeof(int));

    myfree(b);

    int *c = mymalloc(sizeof(int) * 20);

    for(int j = 0; j < 20; j ++){
      c[j]= j;
      printf("addr : %i = %i\n", &c[j], c[j]);
    }

    printf("c : %i\n", c);

    myfree(c);

    printf("On est sortie!!\n");
    


    return(0);
}

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{

  for (int i = 0; i < 500000; i++){
    int *a = mymalloc(sizeof(int) * 512);
    for(int j = 0; j < 512; j ++){
      a[j]= j;
    }
    myfree(a);
  }


    return(0);
}

//gcc -o learnC learnC.c mymalloc.c
//gcc -o test tests.c mymalloc.c
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main()
{
    typedef struct bloc *newBloc;
    typedef struct bloc bloc;

    //newBloc *find;

    struct bloc
        {
            size_t size;
            int free;
            struct bloc *next, *pred;
        };

    int tm = 1876870;
    int *dix = &tm;
    int page = 4096;


    //struct bloc bloc1 = {*dix, sizeof(1024), false, NULL};
    newBloc bloc1 = malloc(sizeof(*bloc1));
    bloc1->size = 1000;
    bloc1->free = 0;

    newBloc bloc2 = malloc(sizeof(*bloc2));
    newBloc bloc3 = malloc(sizeof(*bloc3));


    //struct bloc bloc2 = {*dix, sizeof(1024), false, bloc2};

    printf("The size of a bloc is %i\n", sizeof(bloc1));
    printf("The size of a bloc is %i\n", sizeof(bloc2));
    printf("The size of a bloc is %i\n", sizeof(bloc3));
    printf("The size of a bloc is %i\n", sizeof(struct bloc));

    printf("page - 2*bloc : %i\n", page-(2*sizeof(bloc2)));

    //int *addr = mymalloc(8000);
    int *addr2 = mymalloc(sizeof(int));
    int *addr1 = mymalloc(sizeof(int)*2000);

    //char *src = mmap(0, sizeof(bloc2), PROT_READ, MAP_SHARED, -1, 0);

    //printf("Sorry, can't talk now.");
    //printf("I'm busy!");
    return(0);
}

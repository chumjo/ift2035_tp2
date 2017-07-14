 #include <stdio.h>
int main()
{
    typedef struct bloc *tototo;
    typedef struct bloc toto;

    struct bloc
    {
        void *addr;
        //size_t size;
        int free;
        struct bloc *next;
    };

    int tm = 1876870;
    int* dix = &tm;

    //struct bloc bloc1 = {*dix, sizeof(1024), false, NULL};
    tototo bloc1 = {*dix, NULL};
    struct bloc* bloc3 = {*dix, NULL};
    toto bloc2 = {*dix, NULL};
    //struct bloc bloc2 = {*dix, sizeof(1024), false, bloc2};

    printf("The size of a bloc is %i\n", sizeof(bloc1));
    printf("The size of a bloc is %i\n", sizeof(bloc3));
    printf("The size of a bloc is %i\n", sizeof(bloc2));

    //printf("Sorry, can't talk now.");
    //printf("I'm busy!");
    return(0);
}

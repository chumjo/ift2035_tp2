// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/mman.h>

//Macros
#define BLOC_SIZE (sizeof(struct bloc))
#define PAGE (4096)


typedef struct bloc
    {
        size_t size;
        int free;
        struct bloc *next, *pred;
    } *Bloc;


Bloc blocList(){

    printf("On retrieve la liste!\n");

    static Bloc memoryList = NULL;

    if(memoryList == NULL){

        printf("Creation du premier bloc pour la liste de bloc!\n");

        memoryList = malloc(sizeof(*memoryList));
        //memoryList = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        memoryList->size = PAGE-BLOC_SIZE;
        memoryList->free = 1;
        memoryList->next = NULL;
        memoryList->pred = NULL;

        printf("memoryList -> size :%i\n", memoryList->size);
        printf("memoryList -> free :%i\n", memoryList->free);
        printf("memoryList -> next :%i\n", memoryList->next);
        printf("memoryList -> pred :%i\n", memoryList->pred);
    }

    printf("C'est un succes!!! :-)\n");

    return memoryList;
}

Bloc find(size_t size){

    printf("On trouve un espace dans la memoire qu'on possede deja! \n");

    Bloc current = blocList();

    printf("memoryList -> size :%i\n", current->size);
    printf("memoryList -> free :%i\n", current->free);
    printf("memoryList -> next :%i\n", current->next);
    printf("memoryList -> pred :%i\n", current->pred);

    while(current != NULL){

        printf("Je cherches...\n");

        if((current->free == 1) && (current->size > (size + BLOC_SIZE))){
            printf("OMG!!! J'ai trouve!!!!\n");
            break;
        }
        else
            current = current->next;
    }

    return current;
}

Bloc endList(){

    Bloc current = blocList();

    while(current->next != NULL){
        current = current->next;
    }
}

void *mymalloc(size_t size){

    printf("I am in mymalloc! \n");

    // À modifier
    void *addr;

    //Si on a besoin d'une nouvelle page mémoire
    if(size > (PAGE-2*BLOC_SIZE)){
        printf("Size bigger than a page. Custom size page : %i\n", size);

        Bloc last = endList();

        void *newPage = malloc(size + 2*BLOC_SIZE);

        Bloc newBloc = addr;
        newBloc->size = size;
        newBloc->free = 0;
        newBloc->next = NULL;
        newBloc->pred = last;

        last->next = newBloc;

        return newBloc+1;
    }
    else
        printf("Taille respectable, on continue!\n");

    addr = find(size);

    if(addr != NULL){
        printf("Found a space in current memory. Addresse : %i\n", addr);
        

    }
    else{
        printf("Couldn't find a space in current memory. New page : %i\n", PAGE);
        //addr = mmap(PAGE);
    }


    return addr;
}


void myfree(void *ptr){
    // À modifier

    return;
}



//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>

//Macros
#define BLOC_SIZE sizeof(struct bloc);
#define PAGE 4096;


typedef struc bloc *newBloc;


struct bloc
    {
        size_t size;
        int free;
        struct bloc *next, *pred;
    };


newBloc memoryList; 


void *mymalloc(size_t size){
    // À modifier
    void *addr;


    //Si on a besoin d'une nouvelle page mémoire
    if(size > PAGE-2*BLOC_SIZE){
        printf("Size bigger than a page. Custom size page : %s\n", size);
    	//addr = mmap(size);
        return NULL;
    }
    addr = find(size);

    else if(addr != NULL){
        printf("Found a space in current memory. Addresse : %s\n", *addr);
    	
    }
    else{
        printf("Couldn't find a space in current memory. New page : %s\n", PAGE);
        //addr = mmap(PAGE);
    }
    

    return addr;
}

void *find(size_t size){

    newBloc current = memoryList;

    while(current != NULL){

        if(current.free == 1 && current.size < size+BLOC_SIZE)
            break;
        else
            current = current.next;
    }

    return current;
}

void myfree(void *ptr){
    // À modifier

    return;
}



//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
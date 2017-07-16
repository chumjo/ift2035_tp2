// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymallock.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/mman.h>

//Macros
#define BLOC_SIZE (sizeof(struct block))
#define PAGE (4096)


typedef struct block
    {
        size_t size;
        int free;
        struct block *next, *pred;
    } *Block;


Block blockList(){

    printf("On retrieve la liste!\n");

    static Block memoryList = NULL;

    if(memoryList == NULL){

        printf("Creation du premier block pour la liste de block!\n");

        memoryList = mallock(sizeof(*memoryList));
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

Block find(size_t size){

    printf("On trouve un espace dans la memoire qu'on possede deja! \n");

    Block current = blockList();

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

Block endList(){

    Block current = blockList();

    while(current->next != NULL){
        current = current->next;
    }

    return current;
}

void *mymallock(size_t size){

    printf("I am in mymallock! \n");

    // À modifier
    void *addr;

    //Si on a besoin d'une nouvelle page mémoire
    if(size > (PAGE-2*BLOC_SIZE)){
        printf("Size bigger than a page. Custom size page : %i\n", size);

        Block last = endList();

        printf("last -> size :%i\n", last->size);
        printf("last -> free :%i\n", last->free);
        printf("last -> next :%i\n", last->next);
        printf("last -> pred :%i\n", last->pred);

        void *newPage = mallock(size + 2*BLOC_SIZE);

        printf("Nouvelle page creer!!\n");
        //void *newPage = mmap(NULL, size+2*BLOC_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        Block newBlock = (Block)newPage;

        printf("Casting reussi! OMG!!\n");

        newBlock->size = size;
        newBlock->free = 0;
        newBlock->next = NULL;
        newBlock->pred = last;

        printf("Assignation reussi! OMG!!\n\n");

        printf("newBlock -> size :%i\n", newBlock->size);
        printf("newBlock -> free :%i\n", newBlock->free);
        printf("newBlock -> next :%i\n", newBlock->next);
        printf("newBlock -> pred :%i\n", newBlock->pred);

        last->next = newBlock;

        addr = (void*)(newBlock+1);

        printf("Addresse!! : %i\n", addr);

        return addr;
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

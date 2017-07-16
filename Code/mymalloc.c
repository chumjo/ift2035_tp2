// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/mman.h>

//Macros
#define BLOCK_SIZE (sizeof(struct block))
#define PAGE (4096)


typedef struct block
    {
        size_t size;
        int free;
        struct block *next, *pred;
    } *Block;


void printBlock(Block b){

    printf("\nThis Block -> size :%i\n", b->size);
    printf("This Block -> free :%i\n", b->free);
    printf("This Block -> next :%i\n", b->next);
    printf("This Block -> pred :%i\n\n", b->pred);

    return;
}


Block blockList(){

    printf("On reccupere la liste de Block\n");

    static Block memoryList = NULL;

    if(memoryList == NULL){

        printf("\n***** Creation du premier block pour la liste de block! *****\n\n");

        memoryList = malloc(sizeof(*memoryList));
        //memoryList = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        memoryList->size = PAGE-BLOCK_SIZE;
        memoryList->free = 1;
        memoryList->next = NULL;
        memoryList->pred = NULL;

        printf("memoryList -> size :%i\n", memoryList->size);
        printf("memoryList -> free :%i\n", memoryList->free);
        printf("memoryList -> next :%i\n", memoryList->next);
        printf("memoryList -> pred :%i\n\n", memoryList->pred);
    }

    return memoryList;
}

Block findSpace(size_t size){

    printf("On cherche ou inserer le prochain block...\n");

    Block current = blockList();

    while(current->next != NULL){

        printBlock(current);

        if((current->free == 1) && (current->size > (size + BLOCK_SIZE))){
            break;
        }
        else
            current = current->next;
    }

    return current;
}

void splitBlock(Block b){


    return;
}


void *mymalloc(size_t size){

    printf("\n|------------|\n|- Mymalloc -|\n|------------|\n");
    printf("On cherche un espace de taille : %i\n", size);

    // À modifier
    void *addr;

    //Trouver le bloc où on doit insérer
    Block predBlock = findSpace(size);

    printf("On insere ici :\n");
    printBlock(predBlock);

    //Si on a besoin d'une nouvelle page mémoire
    if(size > (predBlock->size) + BLOCK_SIZE){

        printf("Oups! Il manque d'espace, on en demande une nouvelle page : %i\n", size);

        void *newPage;

        if(size > (PAGE-2*BLOCK_SIZE)){

            printf("Nouvelle Page de grande taille. Taille custom : %i\n", size);

            newPage = malloc(size + 2*BLOCK_SIZE);
            //void *newPage = mmap(NULL, size+2*BLOCK_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }
        else {

            printf("Nouvelle Page : %i\n", PAGE);

            newPage = malloc(PAGE);
        }

        Block newBlock = (Block)newPage;

        newBlock->size = size;
        newBlock->free = 0;
        newBlock->next = NULL;
        newBlock->pred = predBlock;

        printf("Nouveau block :\n");
        printBlock(newBlock);

        predBlock->next = newBlock;

        addr = (void*)(newBlock+1);

        printf("Addresse!! : %i\n", addr);

        return addr;        
    }

    else {

        if(predBlock->size != size)
            splitBlock(predBlock);
        else{
            predBlock->free = 0;
        }

        return predBlock+1;
    }
}


void myfree(void *ptr){
    // À modifier

    return;
}



//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

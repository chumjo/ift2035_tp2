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

    //printf("\nBlock :%i\n", b);

    //printf("\n   size :%i\n", b->size);
    //printf("   free :%i\n", b->free);
    //printf("   next :%i\n", b->next);
    //printf("   pred :%i\n\n", b->pred);

    return;
}


Block blockList(){

    //printf("On reccupere la liste de Block\n");

    static Block memoryList = NULL;

    if(memoryList == NULL){

        //printf("\n***** Creation du premier block pour la liste de block! *****\n\n");

        memoryList = malloc(sizeof(*memoryList));
        //memoryList = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        memoryList->size = PAGE-BLOCK_SIZE;
        memoryList->free = 1;
        memoryList->next = NULL;
        memoryList->pred = NULL;

        //printf("memoryList -> size :%i\n", memoryList->size);
        //printf("memoryList -> free :%i\n", memoryList->free);
        //printf("memoryList -> next :%i\n", memoryList->next);
        //printf("memoryList -> pred :%i\n\n", memoryList->pred);
    }

    return memoryList;
}

Block findSpace(size_t size){

    //printf("On cherche ou inserer le prochain block...\n");

    Block current = blockList();

    while(current->next != NULL){

        //printBlock(current);

        if((current->free == 1) && (current->size > (size + BLOCK_SIZE))){
            break;
        }
        else
            current = current->next;
    }

    return current;
}

void splitBlock(Block b, size_t size){

    //printf("On Split le block!\n");
    //printf("Adresse courante : %i\n", b);
    //printf("Taille du block : %i\n", size);

    Block newBlock = ((void*) (b+1)) + size;
    newBlock->size = b->size - size;
    newBlock->free = 1;
    newBlock->next = b->next;
    newBlock->pred = b;

    b->next = newBlock;
    b->free = 0;
    b->size = size;

    //printf("Adresse du nouveau block : %i\n", newBlock);

    return;
}


void *mymalloc(size_t size){

    //printf("\n|------------|\n|- Mymalloc -|\n|------------|\n");
    //printf("On cherche un espace de taille : %i\n", size);

    // À modifier
    void *addr;

    //Trouver le bloc où on doit insérer
    Block predBlock = findSpace(size);

    //printf("On insere ici : %i\n", predBlock);
    //printBlock(predBlock);

    //Si on a besoin d'une nouvelle page mémoire
    if(size + BLOCK_SIZE > (predBlock->size)){

        //printf("Oups! Il manque d'espace, on en demande une nouvelle page : %i\n", size);

        void *newPage;

        if(size > (PAGE-2*BLOCK_SIZE)){

            //printf("Nouvelle Page de grande taille. Taille custom : %i\n", size);

            newPage = malloc(size + 2*BLOCK_SIZE);
            //void *newPage = mmap(NULL, size+2*BLOCK_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }
        else {

            //printf("Nouvelle Page : %i\n", PAGE);

            newPage = malloc(PAGE);
        }

        Block newBlock = (Block)newPage;

        newBlock->size = PAGE;
        newBlock->free = 0;
        newBlock->next = NULL;
        newBlock->pred = predBlock;

        printBlock(newBlock);

        splitBlock(newBlock, size);

        printBlock(newBlock);

        //printf("Nouveau block :\n");
        //printBlock(newBlock);

        predBlock->next = newBlock;

        printBlock(predBlock);
        printBlock(predBlock->pred);

        addr = (void*)(newBlock+1);

        //printf("Addresse!! : %i\n", addr);

        return addr;        
    }

    else {

        //printf("size block : %i\n", predBlock->size);
        //printf("size demande : %i\n", size);

        if(predBlock->size != size){
            //printf("Different!!\n");
            splitBlock(predBlock, size);
        }
        else{
            predBlock->free = 0;
        }

        return predBlock+1;
    }
}


void myfree(void *ptr){
    // À modifier
    if(ptr == NULL){
        //printf("Pointeur NULL, rien n'est libéré...\n");
    }
    else{

        Block current = blockList();

        while(current != ptr){
            current = current->next;
        }

        if(!current);
            //printf("Pointeur pas allouer avec mymalloc...\n")
        else
            current->free = 1;
    }


    return;
}



//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

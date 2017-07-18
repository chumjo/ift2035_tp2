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

typedef struct head
{
    struct block *first, *last, *firstFree, *lastFree;
} *Head;


typedef struct block
{
    size_t size;
    struct block *next, *pred, *free;
} *Block;


void printBlock(Block b){

    //printf("\nBlock :%i\n", b);

    //printf("\n   size :%i\n", b->size);
    //printf("   free :%i\n", b->free);
    //printf("   next :%i\n", b->next);
    //printf("   pred :%i\n\n", b->pred);

    return;
}


int aligne32(size_t size){

    int rest = size % BLOCK_SIZE;
    if(rest != 0)
        return size - rest + BLOCK_SIZE;
    else
        return size;
}


Head getHead(){

    static Head head = NULL;

    if(head == NULL){

        head = malloc(sizeof(head));
        //memoryList = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        Block firstBlock = malloc(PAGE);

        firstBlock->size = PAGE - BLOCK_SIZE;

        firstBlock->free = NULL;
        firstBlock->next = NULL;
        firstBlock->pred = NULL;

        head->first = firstBlock;
        head->last = firstBlock;
        head->firstFree = firstBlock;
        head->lastFree = firstBlock;
    }

    return head;
}

Block getFirst(){return getHead()->first;}
Block getLast(){return getHead()->last;}
Block getFirstFree(){return getHead()->firstFree;}
Block getLastFree(){return getHead()->lastFree;}

void setFirstFree(Block b)
{
    getHead()->firstFree = b;
}
void setLast(Block b)
{
    getLast()->next = b;
    getHead()->last = b;
}

void setLastFree(Block b)
{
    getLastFree()->free = b;
    getHead()->lastFree = b;
}

int checkIfFree(Block b){

    Block current = getFirstFree();

    while(current != NULL){

        if(current == b)
            return 1;

        current = current->free;
    }

    return 0;
}

Block findSpace(size_t size){

    //printf("On cherche ou inserer le prochain block...\n");

    //On vérifie s'il y a de l'espace dans les blocks libérés
    Block current = getFirstFree();
    Block last = NULL;

    while(current != NULL){

        if(current->size >= size){

            if(!last){
                setFirstFree(current->free);
            }
            else{
                last->free = current->free;
            }

            return current;
        }
        else{
            last = current;
            current = current->free;
        }
    }

    return getLast();
}

void splitBlock(Block b, size_t size){

    //printf("Reste : %i\n", b->size);

    printBlock(b);

    Block newBlock = ((void*) (b+1)) + size;

    newBlock->size = b->size - size - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = b->next;
    newBlock->pred = b;

    b->size = size;

    setLastFree(newBlock);

    if(newBlock->next == NULL)
        setLast(newBlock);

    return;
}


void newPage(Block b, size_t sizePage, size_t sizeBlock){

    void *newPage;

    newPage = malloc(sizePage);

    //On cree un nouveau block au début de la nouvelle page
    Block newBlock = (Block)newPage;

    newBlock->size = sizePage - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = NULL;
    newBlock->pred = b;

    if(sizeBlock + BLOCK_SIZE != sizePage)
        splitBlock(newBlock, sizeBlock);
    else
        setLast(newBlock);

    b->next = newBlock;

    return;
}

//Fonction qui merge le block courant et le suivant si c'est possible
void mergeNextBlock(Block b){

    //printf("MergeNextBlock!\n");
    Block next = b->next;

    if(!next)
        return;


    if(checkIfFree(next) && b->size + (void*) (b+1)){

        //printf("On merge :\n");
        printBlock(b);
        printBlock(next);

        b->size = b->size + (next->size) + BLOCK_SIZE;
        b->next = next->next;
        b->free = next->free;

        if(b->next != NULL)
            b->next->pred = b;

        printBlock(b);
    }
    
    return;
}


void *mymalloc(size_t size){

    printf("First Block : %i\n", getFirst());

    //printf("Size : %i --> ", size);
    size = aligne32(size);
    //printf("%i\n", size);

    //printf("\n|------------|\n|- Mymalloc -|\n|------------|\n");
    //printf("On cherche un espace de taille : %i\n", size);

    void *addr;

    //Trouver le bloc où on doit insérer
    Block blockInsert = findSpace(size);

    //Si on a besoin d'une nouvelle page mémoire
    if(size + BLOCK_SIZE > (blockInsert->size) && blockInsert->next == NULL){

        //printf("Oups! Il manque d'espace, on en demande une nouvelle page...\n");

        //Si la nouvelle page doit être plus grande que 4ko
        if(size > (PAGE-BLOCK_SIZE)){

            //printf("Nouvelle Page de grande taille. Taille custom : %i\n", size);

            newPage(blockInsert, size + BLOCK_SIZE, size);
            //void *newPage = mmap(NULL, size+2*BLOCK_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }
        else {

            //printf("Nouvelle Page : %i\n", PAGE);

            newPage(blockInsert, PAGE, size);
        }

        Block newBlock = blockInsert->next;

        printBlock(blockInsert);
        printBlock(newBlock);

        addr = (void*)(newBlock+1);

        return addr;        
    }

    else {

        if(blockInsert->size != size){
            splitBlock(blockInsert, size);
        }

        return blockInsert+1;
    }

    return NULL;
}


void myfree(void *ptr){
    // À modifier
    //printf("Myfree!\n");

    if(ptr == NULL){
        //printf("Pointeur NULL, aucune memoire n'est liberee...\n");
        return;
    }

    Block current = getFirst();

    //printf("ptr : %i\n", ptr);
    //printf("current : %i\n", current+1);

    while(current+1 != ptr){

        current = current->next;

        //printf("current : %i\n", current+1);
        if(!current){
            //printf("Pointeur pas allouer avec mymalloc...\n");
            return;
        }
    }

    if(current->free != NULL){
        //Espace memoire déjà libéré...
        return;
    }

    //printf("On libere : %i\n", current);
    current->free = NULL;
    setLastFree(current);

    if(current->next != NULL)
        mergeNextBlock(current);

    if(current->pred != NULL && current->pred->free == 1)
        mergeNextBlock(current->pred);

    void *first = getFreeList();

    printf("First free : %i\n", first);

    if(first == NULL){
        printf("On ajoute le premier free!!\n");
        first = &current;
    }

    first = getFreeList();

    printf("First free : %i\n", first);

    /*if(current->size >= PAGE + BLOCK_SIZE){
        //munmap.....
    }*/
    
    return;
}

//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

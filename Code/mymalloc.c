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
    struct block *next, *prev, *free;
} *Block;

//Statique
Head getHead(){

    static Head head = NULL;

    if(head == NULL){

        head = malloc(sizeof(struct head));
        //memoryList = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        //On cree un nouveau block au début de la nouvelle page
        Block firstBlock = malloc(PAGE);

        firstBlock->size = PAGE - BLOCK_SIZE;

        firstBlock->free = NULL;
        firstBlock->next = NULL;
        firstBlock->prev = NULL;

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

void setFirstFree(Block b);
void setLastFree(Block b);
void setLast(Block b);
void setFirst(Block b);

void setFirstFree(Block b)
{
    if(!b){
        getHead()->firstFree = NULL;
        setLastFree(NULL);
    }
    else{
        Block current = getFirstFree();

        if(!current)
            setLastFree(b);

        b->free = current;
        getHead()->firstFree = b;
    }
    
    return;
}

void removeFirstFree(){

    Block first = getFirstFree();

    if(first->free == NULL)
    	setLastFree(NULL);

    getHead()->firstFree = first->free;
    first->free = NULL;

    return;
}

void setLast(Block b)
{
    getLast()->next = b;
    getHead()->last = b;
}

void setLastFree(Block b){

    getHead()->lastFree = b;
    return;
}

//TMP fonctions pour imprimer
void printBlock(Block b){

    printf("\nBlock :%i\n", b);

    printf("\n   size :%i\n", b->size);
    printf("   free :%i\n", b->free);
    printf("   next :%i\n", b->next);
    printf("   prev :%i\n\n", b->prev);

    return;
}

void printList(){

    printf("|||***--- Liste de Blocks ---***|||\n");

    Block current = getFirst();

    while(current){

        //printBlock(current);

        current = current->next;
    }

    printf("NULL...\n\n");

    return;
}

void printFree(){

    printf("|||***--- Liste de Free Blocks ---***|||\n");

    Block current = getFirstFree();

    while(current){

        printBlock(current);

        current = current->free;
    }

    printf("NULL...\n\n");

    return;
}

void printLast(){

    printf("|||***--- Last ---***|||\n");

    printBlock(getLast());

    return;
}

void printLastFree(){

    printf("|||***--- Last Free ---***|||\n");

    if(getLastFree())
        printBlock(getLastFree());

    return;
}


int aligne32(size_t size){

    int rest = size % BLOCK_SIZE;
    if(rest != 0)
        return size - rest + BLOCK_SIZE;
    else
        return size;
}


Block findFreePrev(Block b){

    Block current = getFirstFree();

    while(current != NULL){

        if(current->free == b)
            return current;

        current = current->free;
    }

    return NULL;
}

Block newPage(size_t size){

    //printf("Nouvelle Page!!\n");

    //On cree un nouveau block au début de la nouvelle page
    int sizePage;

    if(size > PAGE-BLOCK_SIZE){
        //TODO align4096
        sizePage = size+BLOCK_SIZE;
    }
    else
        sizePage = PAGE;


    Block newBlock = malloc(sizePage);

    newBlock->size = sizePage - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = NULL;
    newBlock->prev = getLast();

    setLast(newBlock);

    return newBlock;
}

Block findSpace(size_t size){

    //printf("On cherche ou inserer le prochain block...\n");
    //On vérifie s'il y a de l'espace dans les blocks libérés
    Block current = getFirstFree();
    Block previous = NULL;

    while(current != NULL){

        if(current->size >= size){

            //printf("Ce block fera l'affaire!\n");

            if(previous == NULL){
                //printf("C'est le premier free!!\n");
                removeFirstFree();
            }
            else{
                previous->free = current->free;
                current->free = NULL;
            }

            //printBlock(current);

            return current;
        }
        else{
            previous = current;
            current = current->free;
        }
    }

    //Aucun espace libre, on va ajouter à la fin
    Block newBlock = newPage(size);

    return newBlock;
}

void splitBlock(Block b, size_t size){

    //printf("Reste : %i\n", b->size);
    Block newBlock = ((void*) (b+1)) + size;

    newBlock->size = b->size - size - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = b->next;
    newBlock->prev = b;

    b->size = size;

    //if(newBlock->next != NULL)
    setFirstFree(newBlock);

    if(newBlock->next == NULL){
        //printf("On vient de splitter le dernier block\n");
        setLast(newBlock);
    }

    return;
}


//Fonction qui merge le block courant et le suivant si c'est possible
void mergeNextBlock(Block b){

    //printf("MergeNextBlock!\n");
    Block next = b->next;

    Block prevFree = findFreePrev(next);

    if(prevFree && b->size + (void*) (b+1) == next){

        b->size = b->size + (next->size) + BLOCK_SIZE;
        b->next = next->next;

        if(b->next != NULL)
            b->next->prev = b;

        prevFree->free = next->free;

    }
    
    return;
}

//Fonction qui merge le block courant et le précédent si c'est possible
void mergePreviousBlock(Block b){

    //printf("MergeNextBlock!\n");
    Block previous = b->prev;

    Block prevFree = findFreePrev(previous);

    if(prevFree && previous->size + (void*) (b+1) == b){

        previous->size = b->size + (previous->size) + BLOCK_SIZE;
        previous->next = b->next;

        if(b->next != NULL)
            b->next->prev = previous;

        prevFree->free = previous->free;

        previous->free = NULL;

    }
    
    return;
}


void *mymalloc(size_t size){

    //printf("\n|------------|\n|- Mymalloc -|\n|------------|\n");

    size = aligne32(size);

    //printf("On cherche un espace de taille : %i\n", size);

    //Trouver le bloc où on doit insérer
    Block blockInsert = findSpace(size);

    //printBlock(blockInsert);
    if(blockInsert->size > size){
        //printf("ON SPLIT!!!\n");
        splitBlock(blockInsert, size);
    }

    //printf("ADRESSE DE RETOUR DU MALLOC : %i\n", blockInsert+1);
    //printBlock(blockInsert);

    return blockInsert+1;
}


void myfree(void *ptr){

    if(ptr == NULL){
        //printf("Pointeur NULL, aucune memoire n'est liberee...\n");
        return;
    }

    ptr -= BLOCK_SIZE;

    //Block current = ptr;

	Block current = getFirst();

    while(current != ptr){

        current = current->next;

        if(current == NULL){
            //printf("Pointeur pas allouer avec mymalloc...\n");
            return;
        }
    }

    //printBlock(current);

    if(current->free != NULL || current == getLastFree()){
        //printf("Deja libere...\n");

        return;
    }

    //printf("On libere : %i\n", current);
    //printf("SET FREE : \n");

    setFirstFree(current);

/*    if(current->next != NULL){
        mergeNextBlock(current);
    }

    if(current->prev != NULL){
        mergePreviousBlock(current);
    }*/

    /*if(current->size >= PAGE + BLOCK_SIZE){
        //munmap.....
    }*/
    
    return;
}

//Voici l'appel de mmap pour allouer les pages.
//mmap(NULL, nb_page * 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

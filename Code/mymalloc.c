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
    if(getLastFree() == NULL){
        //printf("Set First et Last Free!\n");
        setFirstFree(b);
        getHead()->lastFree = b;
    }
    else{
        getLastFree()->free = b;
        getHead()->lastFree = b;
    }
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

        printBlock(current);

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

    Block lastFree = getLastFree();

    if(lastFree)
        printBlock(lastFree);
    else
        printf("NULL...\n");

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

Block findSpace(size_t size){

    //printf("On cherche ou inserer le prochain block...\n");

    //On vérifie s'il y a de l'espace dans les blocks libérés
    Block current = getFirstFree();
    Block previous = NULL;

    while(current != NULL){

        if(current->size >= size){

            if(previous == NULL){
                
                if(getLastFree() == current)
                    setLastFree(NULL);

                setFirstFree(current->free);
            }
            else{
                previous->free = current->free;
            }


            return current;
        }
        else{
            previous = current;
            current = current->free;
        }
    }

    //Aucun espace libre, on va ajouter à la fin
    //printf("Pas de block libre on ajoute a la fin\n");
    return getLast();
}

void splitBlock(Block b, size_t size){

    //printf("Reste : %i\n", b->size);

    Block newBlock = ((void*) (b+1)) + size;

    newBlock->size = b->size - size - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = b->next;
    newBlock->prev = b;

    b->size = size;

    if(newBlock->next != NULL)
        setLastFree(newBlock);

    if(newBlock->next == NULL){
        //printf("On vient de splitter le dernier block\n");
        setLast(newBlock);
    }

    return;
}


void newPage(Block b, size_t sizePage, size_t sizeBlock){

    //On cree un nouveau block au début de la nouvelle page
    Block newBlock = malloc(sizePage);

    newBlock->size = sizePage - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = NULL;
    newBlock->prev = b;

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

    if(next->free == NULL && getLastFree() != next)
        return;

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

    if(previous->free == NULL && getLastFree() != previous)
        return;

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

    void *addr; //Pour l'adresse qui sera retourné à l'usagé

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

        addr = (void*)(newBlock+1);

        return addr;        
    }

    else {

        //printBlock(blockInsert);

        if(blockInsert->size != size){
            //printf("ON SPLIT!!!\n");
            splitBlock(blockInsert, size);
        }

        //printf("ADRESSE DE RETOUR DU MALLOC : %i\n", blockInsert+1);

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

    if(current->free != NULL || current == getLastFree()){
        //Espace memoire déjà libéré...
        return;
    }

    printf("On libere : %i\n", current);

    current->free = NULL;
    setLastFree(current);

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

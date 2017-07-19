//Jonathan Larose
//Lisanne Duquette

// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

//Macros
#define BLOCK_SIZE (64)
#define PAGE (4096)

typedef struct head
{
    struct block *first, *last, *firstFree, *lastFree, *firstPage, *lastPage;
} *Head;


typedef struct block
{
    size_t size, pageSize;
    struct block *next, *prev, *free, *nextPage;
} *Block;


void setFirstFree(Block b);
void setLastFree(Block b);
void setLast(Block b);
void setFirst(Block b);
void printBlock(Block b);

//Statique
Head getHead(){

    static Head head = NULL;

    if(head == NULL){

        //Alloue un peu de mémoire pour le head
        //head = malloc(sizeof(struct head));
        head = mmap(NULL, sizeof(struct head), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        //Crée un nouveau block vide
        //Block firstBlock = malloc(PAGE);
        Block firstBlock = mmap(NULL, PAGE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        firstBlock->size = PAGE - BLOCK_SIZE;   //Size du block
        firstBlock->free = NULL;                //Prochain Block libre (si ce block fait partie de la liste de block libre)
        firstBlock->next = NULL;                //prochain block
        firstBlock->prev = NULL;                //block pécédent
        firstBlock->nextPage = NULL;            //Prochaine page mémoire (si ce block est le premier d'une page mémoire)

        //Pointe les éléments du Header vers le nouveau block
        head->first = firstBlock;       //Premier Block de la liste de tout les blocks
        head->last = firstBlock;        //Dernier Block de la liste de tout les blocks
        head->firstFree = firstBlock;   //Premier Block de la liste de blocks libres
        head->lastFree = firstBlock;    //Dernier Block de la liste de blocks libres
        head->firstPage = firstBlock;   //Premier Block de la liste de page mémoire
        head->lastPage = firstBlock;    //Dernier Block de la liste de page mémoire
    }

    return head;
}

//Getters pour les éléments du Head
Block getFirst(){return getHead()->first;}
Block getLast(){return getHead()->last;}
Block getFirstFree(){return getHead()->firstFree;}
Block getLastFree(){return getHead()->lastFree;}
Block getFirstPage(){return getHead()->firstPage;}
Block getLastPage(){return getHead()->lastPage;}

void setFirstFree(Block b);
void setLastFree(Block b);
void setLast(Block b);
void setFirst(Block b);
void printBlock(Block b);

//Fonction qui ajoute un block libre au début de la liste de block libre
void setFirstFree(Block b)
{
    if(!b){
        //Si ont met le premier élément à NULL, on met le dernier free à NULL aussi
        getHead()->firstFree = NULL;
        setLastFree(NULL);
    }
    else{
        Block current = getFirstFree();

        //Si le premier élément de la liste de Block libre est null, le nouveau premier block est aussi le dernier
        if(!current)
            setLastFree(b);

        b->free = current;
        getHead()->firstFree = b;
    }
    
    return;
}

//Fonction qui enlève le premier élément de la liste de blocks libres
void removeFirstFree(){

    Block first = getFirstFree();

    if(first->free == NULL)
    	setLastFree(NULL);

    getHead()->firstFree = first->free;
    first->free = NULL;

    return;
}

//Fonction qui ajoute un block à la liste fin de la liste de block
void setLast(Block b)
{
    getLast()->next = b;
    getHead()->last = b;
    return;
}

//Fonction qui set le block comme étant le dernier block libre
void setLastFree(Block b){

    getHead()->lastFree = b;
    return;
}

//Ajoute une page à la liste de page
void setLastPage(Block b){

	getLastPage()->nextPage = b;
	getHead()->lastPage = b;

	return;
}

//Fonction qui aligne la taille d'un espace demandé sur un multiple de nos blocks
int alignBlock(size_t size){

    int rest = size % BLOCK_SIZE;
    if(rest != 0)
        return size - rest + BLOCK_SIZE;
    else
        return size;
}

int alignPage(size_t size){

    int rest = size % PAGE;
    if(rest != 0)
        return size - rest + PAGE;
    else
        return size;
}

//Fonction qui crée une nouvelle page mémoire au besoin
Block newPage(size_t size){


    //On cree un nouveau block au début de la nouvelle page
    int sizePage;

    //Si on veut une page plus grande que la taille normale
    if(size > PAGE-BLOCK_SIZE){

        size = alignPage(size);
        sizePage = size+BLOCK_SIZE;
    }
    else
        sizePage = PAGE;

    //Block newBlock = malloc(sizePage);
    Block newBlock = mmap(NULL, sizePage, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    newBlock->size = sizePage - BLOCK_SIZE;
    newBlock->pageSize = newBlock->size;

    newBlock->free = NULL;
    newBlock->next = NULL;
    newBlock->prev = getLast();
    newBlock->nextPage = NULL;

    //C'est le dernier BLock et la dernière page
    setLast(newBlock);
    setLastPage(newBlock);

    return newBlock;
}

//Fonction qui retourne un block libre d'au moins la taille demandé
Block findSpace(size_t size){

    //On vérifie s'il y a de l'espace dans les blocks libérés
    Block current = getFirstFree();
    Block previous = NULL;

    while(current != NULL){

        //Si on trouve un block
        if(current->size >= size){

            //On le retire le block de la liste de block libre
            if(previous == NULL){
                removeFirstFree();
            }
            else{
                previous->free = current->free;
                current->free = NULL;
            }

            //On le retourne
            return current;
        }
        //Sinon, on regarde le prochain block libre
        else{
            previous = current;
            current = current->free;
        }
    }

    //Aucun espace libre disponible, on crée une nouvelle page mémoire
    Block newBlock = newPage(size);

    return newBlock;
}

//Fonction qui traverse les pages mémoire et retourne la page mémoire dans laquelle se trouve le pointeur (NULL sinon)
Block findPage(void *ptr){

	Block current = getFirstPage();

	while(current != NULL){

		int page = current->pageSize / BLOCK_SIZE;
		void* deb = (void*) current;
		void* fin = (void*) (current + page);

		if(deb <= ptr && ptr < fin){

			return current;
		}
		else
			current = current->nextPage;
	}

	return NULL;
}

//Fonction qui split un block trop grand par la taille voulu
void splitBlock(Block b, size_t size){

    Block newBlock = ((void*) (b+1)) + size;

    newBlock->size = b->size - size - BLOCK_SIZE;

    newBlock->free = NULL;
    newBlock->next = b->next;
    newBlock->prev = b;

    b->size = size;

    //Le nouveau block est libre
    setFirstFree(newBlock);

    //Si le nouveau block est le dernier
    if(newBlock->next == NULL){

        setLast(newBlock);
    }

    return;
}

void *mymalloc(size_t size){

    //On veut que la taille soit un multiple de nos block
    size = alignBlock(size);

    //Trouver le bloc où on doit insérer
    Block blockInsert = findSpace(size);

    //Si le block est trop gros, on le split
    if(blockInsert->size > size){

        splitBlock(blockInsert, size);
    }

    //On retourne l'adresse utilisable à l'utilisateur
    return blockInsert+1;
}


void myfree(void *ptr){

    if(ptr == NULL){
        //printf("Pointeur NULL, aucune memoire n'est liberee...\n");
        return;
    }

    //On cherche si le pointeur est dans une de nos page
    //Block current = findPage(ptr);

    Block current = getFirst();

    //Si non, le pointeur n'a pas été retourné par mymalloc
    if(!current){
    	//printf("Pointeur pas allouer par mymalloc\n");
    	return;
    }

    //Cherche le pointeur dans la page
    while(current+1 != ptr){

    	current = current->next;

        //Si absent...
        if(current == NULL){
            //printf("Pointeur pas allouer par mymalloc\n");
            return;
        }
    }

    //Si le pointeur est déjà libre...
    if(current->free != NULL || current == getLastFree()){
        //printf("Deja liberee...\n");
        return;
    }

    setFirstFree(current);
    
    return;
}

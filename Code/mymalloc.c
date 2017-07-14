// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit être dans ce fichier

#include "mymalloc.h"

typedef int bool;
#define true 1;
#define false 0;


struct bloc
{
	void *addr;
	size_t size;
	bool free;
	struct bloc *next;
};

void *mymalloc(size_t size){
    // À modifier
    void *addr;

    //Si on a besoin d'une nouvelle page mémoire
    if(size > 4096)
    	addr = mmap(size);
    else
    	addr = mmap(4096);

    //Ajouter à la liste des adresses avec le size
    //TODO
    //TODO

    return addr;

    return NULL;
}

void myfree(void *ptr){
    // À modifier
    return;
}

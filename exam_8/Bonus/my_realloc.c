#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Precizam faptul ca functia reala 'realloc' contine metadate despre
// marimea veche a zonei de memorie care trebuie inlocuita, deci old_size
// nu va exista, de aceea in general avem doar 2 argumente
void *my_realloc(void *ptr, size_t new_size, size_t old_size)
{
	// Mai intai, vom trata cazurile extreme
	// Daca ptr e initializat cu NULL, pur si simplu alocam cu malloc
	if (!ptr) {
		ptr = malloc(new_size);
		return ptr;
	}

	// Daca size-ul este 0, pur si simplu dam free
	if (new_size == 0 && ptr != NULL) {
		free(ptr);
		return NULL;
	}

	// Apoi, daca avem un pointer valid si o lungime mai mare decat 0, vom cauta un
	// bloc nou de memorie
	void *new_ptr = (void *)malloc(new_size);
	if (!new_ptr) {
		printf("couldn't find new block of memory!\n");
		return NULL;
	}

	// Vom copia dintr-o parte in alta datele
	if (old_size > new_size) {
		memcpy(new_ptr, ptr, new_size);
	} else {
		memcpy(new_ptr, ptr, old_size);
	}

	// Eliberam zona de memorie veche
	free(ptr);

	// Returnam noua zona de memorie
	return new_ptr;
}
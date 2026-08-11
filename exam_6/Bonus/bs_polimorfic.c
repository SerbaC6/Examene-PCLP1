#include <stdio.h>
#include <stdlib.h>

void *polimorphic_bsearch(void *key, void *base, size_t num, size_t data_size, int (*cmp)(const void *, const void *))
{
	// Tratarea cazurilor in care colectia este goala sau pointerii sunt invalizi
	if (num == 0 || !base || !key) {
		return NULL;
	}

	// Facem cast la char* pentru a ne putea deplasa din octet in octet in memorie
	char *arr = (char *)base;
	
	int left = 0;
	int right = num - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;

		// Calculam adresa elementului de la mijloc inmultind indexul cu marimea in octeti
		void *curr_elem = (void *)(arr + mid * data_size);

		// Folosim functia de comparare primita ca parametru
		int res = cmp(key, curr_elem);

		// Daca rezultatul este 0, am gasit elementul cautat
		if (res == 0) {
			return curr_elem;
		}

		// Ajustam capetele intervalului in functie de rezultatul comparatiei
		if (res < 0) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}

	// Daca nu am gasit cheia, returnam NULL
	return NULL;
}
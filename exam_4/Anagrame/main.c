#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#define LINIE_MAX 81
#define WORD_MAX 101
#define ANAGRAME_MAX 10

char **allocate_matrix(int len);
void swap_strings(char *str1, char *str2);
void sepcuv(FILE *in, char delim[], char **cuv);
void sort_words(char **cuv, int len);
void add_words(char ***big_matrix, int n);
void eliminate_words(char ***cuv, int *len);
void sort_matrix(char ***big_matrix, int n);
void sort_anagrame_lines(char **final, int nr_lines);
void show_and_create_final(char **final, char ***big_matrix, int n);

int main(void)
{
	int n = 0;
	char **cuv, delim[] = " \n?!.,-;()";

	FILE *in = fopen("cuvinte.txt", "rt");
	if (!in) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	char line[LINIE_MAX];
	// calculam numarul total de cuvinte din text
	while (fgets(line, LINIE_MAX, in)) {
		char *p;
		p = strtok(line, delim);

		while (p != NULL) {
			n++;
			p = strtok(NULL, delim);
		}
	}
	fseek(in, 0, SEEK_SET);

	// "cuv" este un vector de cuvinte, pe care il vom sorta si ii vom elimina duplicatele
	cuv = allocate_matrix(n);
	if (!cuv) {
		fprintf(stderr, "malloc failed!\n");
		return -1;
	}

	sepcuv(in, delim, cuv);
	sort_words(cuv, n);
	eliminate_words(&cuv, &n);
	
	// big_matrix = matricea care imbina cele doua matrici de caractere facute
	// intr-una singura, pe doua linii, ca in indicatiile enuntului
	char **big_matrix[2];
	big_matrix[0] = cuv;
	big_matrix[1] = allocate_matrix(n);
	if (!big_matrix[1]) {
		fprintf(stderr, "matrix allocation failed!\n");
		return -1;
	}
	add_words(big_matrix, n);
	sort_matrix(big_matrix, n);

	char **final_words = allocate_matrix(n);
	if (!final_words) {
		fprintf(stderr, "matrix allocation failed!\n");
		return -1;
	}

	show_and_create_final(final_words, big_matrix, n);

	for (int i = 0; i < n; i++) {
		free(cuv[i]);
		free(big_matrix[1][i]);
		free(final_words[i]);
	}
	free(big_matrix[1]);
	free(cuv);
	free(final_words);
	fclose(in);
	return 0;
}

char **allocate_matrix(int len)
{
	char **aux = (char **)malloc(len * sizeof(char *));
	if (!aux) {
		fprintf(stderr, "malloc failed!\n");
		return NULL;
	}

	for (int i = 0; i < len; i++) {
		aux[i] = (char *)malloc(WORD_MAX * sizeof(char));
		if (!aux[i]) {
			fprintf(stderr, "malloc failed\n");
			return NULL;
		}
	}

	return aux;
}

void swap_strings(char *str1, char *str2)
{
	char aux[WORD_MAX];
	strcpy(aux, str1);
	strcpy(str1, str2);
	strcpy(str2, aux);
}

// Functie ce extrage cuvintele intr-un vector de cuvinte, adica o matrice de caractere,
// cu litera mica
void sepcuv(FILE *in, char delim[], char **cuv)
{
	char line[LINIE_MAX];
	int cnt = 0;

	while (fgets(line, LINIE_MAX, in)) {
		char *p;
		p = strtok(line, delim);
		
		while (p != NULL) {
			int i = 0;
			while (p[i]) {
				p[i] = tolower(p[i]);
				i++;
			}

			strcpy(cuv[cnt], p);
			cnt++;
			p = strtok(NULL, delim);
		}
	}
}

// Functie ce sorteaza cuvintele in ordine lexicografica intr-o matrice de caractere
void sort_words(char **cuv, int len)
{
	int i, j;

	for (i = 0; i < len - 1; i++) {
		for (j = i + 1; j < len; j++) {
			if (strcmp(cuv[i], cuv[j]) > 0) {
				swap_strings(cuv[i], cuv[j]);
			}
		}
	}
}

// Functie ce elimina cuvintele duplicate dintr-o matrice de caractere
void eliminate_words(char ***cuv, int *len)
{
	int i, j, n = (*len);

	for (i = 0; i < n - 1; i++) {
		if (strcmp((*cuv)[i], (*cuv)[i + 1]) == 0) {
			int poz = i;
			
			for (j = poz; j < n - 1; j++) {
				strcpy((*cuv)[j], (*cuv)[j + 1]);
			}
			n--;
			i--;
		}
	}
	// eliberam locul unde erau duplicatele
	for (i = n; i < (*len); i++) {
		free((*cuv)[i]);
	}
	*len = n;
}

// Functie ce adauga linia a doua din matricea propusa in indicatii
void add_words(char ***big_matrix, int n)
{
	char sorted[WORD_MAX];
	int i, j, k;

	for (k = 0; k < n; k++) {
		strcpy(sorted, big_matrix[0][k]);
		int len = strlen(sorted);

		for (i = 0; i < len - 1; i++) {
			for (j = i + 1; j < len; j++) {
				if (sorted[i] > sorted[j]) {
					char aux = sorted[i];
					sorted[i] = sorted[j];
					sorted[j] = aux;
				}
			}
		}

		strcpy(big_matrix[1][k], sorted);
	}
}

// Functie ce sorteaza coloanele matricei in ordine lexicografica
void sort_matrix(char ***big_matrix, int n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (strcmp(big_matrix[1][i], big_matrix[1][j]) > 0) {
				swap_strings(big_matrix[1][i], big_matrix[1][j]);
				swap_strings(big_matrix[0][i], big_matrix[0][j]);
			}
		}
	}
}

// Functie ce sorteaza liniile cu anagrame
void sort_anagrame_lines(char **final, int nr_lines)
{
	int i;

	for (i = 0; i < nr_lines; i++) {
		if (strchr(final[i], '-')) {
			char *anagrame[ANAGRAME_MAX], new_order[WORD_MAX];
			char aux[WORD_MAX], *p;
			int cnt_anagrame = 0;

			strcpy(aux, final[i]);
			p = strtok(aux, "-");
			while (p != NULL) {
				// Luam doar pointerii, pe care ulterior ii vom copia inapoi 
				// in new_order
				anagrame[cnt_anagrame++] = p;
				p = strtok(NULL, "-");
			}

			// Sortam cuvintele
			sort_words(anagrame, cnt_anagrame);

			// Le adaugam intr-o noua linie, pe care o vom folosi la suprascrierea
			// linii curente din final
			strcpy(new_order, anagrame[0]);
			for (int j = 1; j < cnt_anagrame; j++) {
				strcat(new_order, "-");
				strcat(new_order, anagrame[j]);
			}
			strcpy(final[i], new_order);
		}
	}
}

// Functie ce afiseaza rezultatul in forma dorita
void show_and_create_final(char **final, char ***big_matrix, int n)
{
	int cnt = 0, i = 1, j;
	strcpy(final[cnt], big_matrix[0][0]);

	// Incepem formarea sirurilor finale
	while (i < n) {
		if (strcmp(big_matrix[1][i - 1], big_matrix[1][i]) == 0) {
			strcat(final[cnt], "-");
			strcat(final[cnt], big_matrix[0][i]);
		} else {
			strcpy(final[++cnt], big_matrix[0][i]);
		}
		i++;
	}

	char sep[] = "-\n";
	// Crestem cnt fiindca vrem sa aflam numarul total de linii, nu indicele final
	cnt++;
	// Sortam mai intai liniile cu anagrame, apoi liniile dupa primul cuvant
	sort_anagrame_lines(final, cnt);
	for (i = 0; i < cnt - 1; i++) {
		for (j = i + 1; j < cnt; j++) {
			char line1[WORD_MAX], line2[WORD_MAX];
			char *str1, *str2;
		   
			strcpy(line1, final[i]);
			strcpy(line2, final[j]);
			
			str1 = strtok(line1, sep);
			str2 = strtok(line2, sep);
			
			if (strcmp(str1, str2) > 0) {
				swap_strings(final[i], final[j]);
			}
		}
	}


	for (i = 0; i < cnt; i++) {
		printf("%s\n", final[i]);
	}
}
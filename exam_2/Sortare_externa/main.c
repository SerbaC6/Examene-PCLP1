#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long *alloc_long_arr(int len);
int see_min(long *arr, int n, long *cursor);
int verif_eof(long *cursor, int n);

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Add more arguments!\n");
		return -1;
	}

	int i, n = argc - 2;
	long *cursor, *curr_num, nr;
	// incepem indexarea de la 1, deci avem nevoie de n + 1 (0...n)
	cursor = alloc_long_arr(n + 1);
	curr_num = alloc_long_arr(n + 1);

	for (i = 1; i <= n; i++) {
		FILE *in = fopen(argv[i], "rb");
		if (!in) {
			fprintf(stderr, "Can't open %s!\n", argv[i]);
			return -1;
		}

		// Citim numarul curent din fisierul curent
		if (fread(&nr, sizeof(long), 1, in)) {
			// Retinem unde este cursorul din fiecare fisier intr-un array separat
			cursor[i] = ftell(in);
			curr_num[i] = nr;
		} else {
			cursor[i] = -1;
			// In cazul in care avem un fisier gol din start, il initializam ca atare
			curr_num[i] = 0;
		}

		fclose(in);
	}

	// Verificam daca macar un fisier are date (toate pot fi goale din start)
	// (programare defensiva, in cazul in care toate fisierele sunt goale)
	int ok = verif_eof(cursor, n);
	// Deschidem fisierul in care adaugam numerele
	FILE *out = fopen(argv[argc - 1], "wb");
	if (!out) {
		fprintf(stderr, "Can't open %s!\n", argv[argc - 1]);
		return -1;
	}

	while (ok) {
		// Cautam indicele minimului din array-ul curent de long-uri extrase din fisiere
		int cnt_min = see_min(curr_num, n, cursor);

		// Scriem in fisier rezultatul
		fwrite(&curr_num[cnt_min], sizeof(long), 1, out);

		FILE *in = fopen(argv[cnt_min], "rb");
		if (!in) {
			fprintf(stderr, "Can't open %s!\n", argv[cnt_min]);
			break;
		}
		// Ne ducem in fisier la locul unde am extras ultima oara din fisier
		fseek(in, cursor[cnt_min], SEEK_CUR);
		
		// Citim numarul
		if (fread(&nr, sizeof(long), 1, in)) {
			// Am introdus numarul in fisierul final, il inlocuim cu urmatorul din acelasi fisier
			cursor[cnt_min] = ftell(in);
			curr_num[cnt_min] = nr;
		} else {
			cursor[cnt_min] = -1; // fisierul s-a terminat
		}

		fclose(in);
		ok = verif_eof(cursor, n);
	}
	
	free(cursor);
	free(curr_num);
	fclose(out);
	return 0;
}

// Functie ce aloca un array de long-uri
long *alloc_long_arr(int len)
{
	long *arr = (long *)malloc(len * sizeof(long));
	if (!arr) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	return arr;
}

//  Functie ce intoarce indicele celui mai mic numar dintre fisierele neepuizate
int see_min(long *arr, int n, long *cursor)
{
	int i, cnt_min = -1;
	long min_local;

	for (i = 1; i <= n; i++) {
		// sarim peste fisierele la care am ajuns la final
		if (cursor[i] == -1) {
			continue;
		}

		if (cnt_min == -1 || min_local > arr[i]) {
			min_local = arr[i];
			cnt_min = i;
		}
	}

	return cnt_min;
}

// Functie ce verifica daca mai avem fisiere din care sa citim
int verif_eof(long *cursor, int n)
{
	for (int i = 1; i <= n; i++) {
		if (cursor[i] != -1) {
			return 1;
		}
	}
	return 0;
}
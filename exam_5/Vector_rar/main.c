#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int x, poz;
} termen;

typedef struct {
	int nn, n;
	int *x, *poz;
} SparceArray;

int *decomp(SparceArray sa);
SparceArray comp(int n, int *v);
void begin_writing(FILE *in, FILE *out, int min_len, int max_len);
int *allocate_int_arr(int len);


int main(int argc, char **argv)
{
	if (argc != 5) {
		fprintf(stderr, "Not enough arguments!\n");
		return -1;
	}

	int min_len, max_len;

	min_len = atoi(argv[3]);
	max_len = atoi(argv[4]);

	FILE *in = fopen(argv[1], "rb");
	if (!in) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	FILE *out = fopen(argv[2], "wb");
	if (!out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	begin_writing(in, out, min_len, max_len);
	fclose(in);
	fclose(out);

	return 0;
}

int *decomp(SparceArray sa)
{
	// Am alocat cu functia calloc deoarece initializeaza toate elementele
	// vectorului cu 0
	int *v = allocate_int_arr(sa.n);

	int i;
	for (i = 0; i < sa.nn; i++) {
		v[sa.poz[i]] = sa.x[i];
	}

	return v;
}

SparceArray comp(int n, int *v)
{
	SparceArray sa;
	int nr_nen = 0;

	for (int i = 0; i < n; i++) {
		if (v[i] != 0) {
			nr_nen++;
		}
	}

	sa.poz = allocate_int_arr(nr_nen);
	sa.x = allocate_int_arr(nr_nen);
	sa.n = n;
	sa.nn = nr_nen;

	int cnt_t = 0;
	for (int i = 0; i < n; i++) {
		if (v[i] != 0) {
			sa.x[cnt_t] = v[i];
			sa.poz[cnt_t] = i;
			cnt_t++;
		}
	}

	return sa;
}

void begin_writing(FILE *in, FILE *out, int min_len, int max_len)
{
	int i;

	for (int len = min_len; len <= max_len; len++) {
		fseek(in, 0, SEEK_SET);

		int *v = allocate_int_arr(len);
		SparceArray now;
		now.x = NULL;
		now.poz = NULL;
		now.n = -1;

		// Cautam un vector valid 
		while (fread(&now.n, sizeof(int), 1, in)) {
			// Daca gasim un vector bun, il salvam
			if (now.n == len) {
				fread(&now.nn, sizeof(int), 1, in);

				now.x = allocate_int_arr(now.nn);
				now.poz = allocate_int_arr(now.nn);

				for (i = 0; i < now.nn; i++) {
					fread(&now.x[i], sizeof(int), 1, in);
				}

				for (i = 0; i < now.nn; i++) {
					fread(&now.poz[i], sizeof(int), 1, in);
				}

				// Adunam in vectorul suma ce am gasit
				int *curr_v = decomp(now);
				for (i = 0; i < len; i++) {
					v[i] += curr_v[i];
				}
				free(curr_v);
				free(now.x);
				free(now.poz);
			} else { // continuam cu urmatorul vector
				fread(&now.nn, sizeof(int), 1, in);
				fseek(in, now.nn * 2 * sizeof(int), SEEK_CUR);
			}
		}

		// Scriem vectorul, dupa ce am gasit toti vectorii cu lungimea curenta
		// si i-am adunat la vectorul suma
		SparceArray output = comp(len, v);
		fwrite(&output.n, sizeof(int), 1, out);
		fwrite(&output.nn, sizeof(int), 1, out);

		for (i = 0; i < output.nn; i++) {
			fwrite(&output.x[i], sizeof(int), 1, out);
		}

		for (i = 0; i < output.nn; i++) {
			fwrite(&output.poz[i], sizeof(int), 1, out);
		}

		free(output.poz);
		free(output.x);
		free(v);
	}
}

int *allocate_int_arr(int len)
{
	int *aux = (int *)calloc(len, sizeof(int));
	if (!aux) {
		fprintf(stderr, "calloc failed!\n");
		return NULL;
	}

	return aux;
}
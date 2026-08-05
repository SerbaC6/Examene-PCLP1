#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
	int lin, col;
	double val;
} termen;

typedef struct {
	int nl, nc, nn;
	termen *term;
} matrice_rara;

int line_cnt(FILE *in);
void search_ord(int ord, FILE *in);
void add_values(FILE *in, matrice_rara *mat);
void show_matrix(matrice_rara mat);

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Add more arguments!\n");
		return -1;
	}

	FILE *in = fopen(argv[1], "rb");
	if (!in) {
		fprintf(stderr, "Can't open file \n");
		return -1;
	}
	int nr_mat, i;
	matrice_rara mat;
	mat.term = NULL;
	nr_mat = line_cnt(in);

	fseek(in, 0, SEEK_SET);
	for (i = 2; i < argc; i++) {
		int ord = atoi(argv[i]);

		if (ord > nr_mat || ord < 1) {
			printf("Matrice inexistenta\n");
			continue;
		}

		search_ord(ord, in);
		add_values(in, &mat);
		show_matrix(mat);

		fseek(in, 0, SEEK_SET);
	}

	if (mat.term) {
		free(mat.term);
	}
	fclose(in);
	return 0;
}

// Functie ce numara cate matrice sunt in fisier
int line_cnt(FILE *in)
{
	int cnt = 0;
	int nl, nc, nn;

	fread(&nl, sizeof(int), 1, in);

	while (1) {
		if (feof(in) != 0) {
			break;
		}

		cnt++;
		fread(&nc, sizeof(int), 1, in);
		fread(&nn, sizeof(int), 1, in);
		fseek(in, nn * sizeof(termen), SEEK_CUR);
		fread(&nl, sizeof(int), 1, in);
	}

	return cnt;
}

// Functie ce duce cursorul in fisier la matricea cu numarul de ordine dat
void search_ord(int ord, FILE *in)
{
	int cnt = 0;
	int nl, nc, nn;

	fread(&nl, sizeof(int), 1, in);

	while (1) {
		if (feof(in) != 0) {
			break;
		}

		cnt++;
		// Ne intoarcem la nl-ul matricei pe care ne dorim sa o afisam
		if (cnt == ord) {
			fseek(in, -1 * sizeof(int), SEEK_CUR);
			break;
		}

		fread(&nc, sizeof(int), 1, in);
		fread(&nn, sizeof(int), 1, in);
		fseek(in, nn * sizeof(termen), SEEK_CUR);
		fread(&nl, sizeof(int), 1, in);
	}
}

// Functie ce aloca memorie si citeste matricea din pozitia curenta
void add_values(FILE *in, matrice_rara *mat)
{
	fread(&mat->nl, sizeof(int), 1, in);
	fread(&mat->nc, sizeof(int), 1, in);
	fread(&mat->nn, sizeof(int), 1, in);

	int nr = mat->nn;

	termen *aux = (termen *)realloc(mat->term, nr * sizeof(termen));
	if (!aux) {
		fprintf(stderr, "realloc failed!\n");
		return;
	}

	mat->term = aux;

	for (int i = 0; i < nr; i++) {
		fread(&mat->term[i].lin, sizeof(int), 1, in);
		fread(&mat->term[i].col, sizeof(int), 1, in);
		fread(&mat->term[i].val, sizeof(double), 1, in);
	}
}

// Functie ce afiseaza matricea rara
void show_matrix(matrice_rara mat)
{
	int num_l, num_col, i, j;

	num_l = mat.nl;
	// Alocam cu functia calloc deoarece avem o matrice sparse, deci multe 0-uri
	double **mat_s = (double **)calloc(num_l, sizeof(double *));
	if (!mat_s) {
		fprintf(stderr, "calloc failed!\n");
		return;
	}

	num_col = mat.nc;
	for (i = 0; i < num_l; i++) {
		mat_s[i] = (double *)calloc(num_col, sizeof(double));
		if (!mat_s[i]) {
			fprintf(stderr, "calloc failed!\n");
			return;
		}
	}

	for (i = 0; i < mat.nn; i++) {
		mat_s[mat.term[i].lin - 1][mat.term[i].col - 1] = mat.term[i].val;
	}

	for (i = 0; i < num_l; i++) {
		for (j = 0; j < num_col; j++) {
			printf("%.1lf ", mat_s[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < num_l; i++) {
		free(mat_s[i]);
	}
	free(mat_s);
}
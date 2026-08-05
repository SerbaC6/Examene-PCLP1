#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	double c;
	int exp;
} termen;

typedef struct {
	int ntn;
	int grd;
	termen *t;
} polinom_rar;

void allocate_and_read_pol(FILE *in, polinom_rar *polin_curr);
void show_poly(polinom_rar polin_curr);
int search(char *ord_nr, FILE *in);

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Add more arguments!\n");
		return -1;
	}

	FILE *in = fopen(argv[1], "rb");
	if (!in) {
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		return -1;
	}

	polinom_rar polin_curr;
	polin_curr.t = NULL;
	polin_curr.ntn = 0;

	for (int i = 2; i < argc; i++) {
		// Daca gasim polinomul, il stocam si il afisam, altfel continuam
		if (search(argv[i], in)) {
			allocate_and_read_pol(in, &polin_curr);
			show_poly(polin_curr);
		} else {
			continue;
		}
	}

	fclose(in);
	if (polin_curr.t != NULL) {
		free(polin_curr.t);
	}
	return 0;
}

// Functie ce cauta si verifica polinomul la care trebuie sa ajungem
int search(char *ord_nr, FILE *in)
{
	int ord = atoi(ord_nr), ntn_curr;
	// Ne pozitionam la inceput
	fseek(in, 0, SEEK_SET);

	// Parcurgem toate polinoamele din spatele celui la care vrem sa ajungem
	for (int i = 1; i < ord; i++) {
		// Verificam ca nu am ajuns la capatul fisierului
		if (fread(&ntn_curr, sizeof(int), 1, in)) {
			// Sarim peste grd si peste toti termenii
			// Aici nu am folosit sizeof(termen) deoarece, din cauza padding-ului,
			// am fi avut 16 octeti in loc de 12
			fseek(in, sizeof(int) + ntn_curr * (sizeof(double) + sizeof(int)), SEEK_CUR);
		} else {
			printf("Polinomul %d nu exista!\n", ord);
			return 0;
		}
	}
	// Verificam si cazul in care ajungem la polinomul dorit, dar acesta nu exista
	// De exemplu, daca avem 4 polinoame in fisier si noi il vrem pe al 5-lea
	if (fread(&ntn_curr, sizeof(int), 1, in)) {
		// Ne intoarcem la inceputul polinomului vrut
		fseek(in, (-1) * 4, SEEK_CUR);
		return 1;
	} else {
		printf("Polinomul %d nu exista!\n", ord);
		return 0;
	}
}

// Functie ce aloca memorie pentru polinomul cerut
void allocate_and_read_pol(FILE *in, polinom_rar *polin_curr)
{
	fread(&polin_curr->ntn, sizeof(int), 1, in);

	termen *aux = (termen *)realloc(polin_curr->t,
									 polin_curr->ntn * sizeof(termen));
	if (!aux) {
		fprintf(stderr, "calloc failed\n");
		return;
	}
	polin_curr->t = aux;
	memset(polin_curr->t, 0, polin_curr->ntn * sizeof(termen));
	
	fread(&polin_curr->grd, sizeof(int), 1, in);

	for (int i = 0; i < polin_curr->ntn; i++) {
		int curr_exp;
		double curr_c;

		fread(&curr_c, sizeof(double), 1, in);
		fread(&curr_exp, sizeof(int), 1, in);

		polin_curr->t[i].c = curr_c;
		polin_curr->t[i].exp = curr_exp;
	}
}

// Functie ce afiseaza polinomul cerut
void show_poly(polinom_rar polin_curr)
{
	int i, nr_termeni = polin_curr.ntn;
	printf("P(x) = ");
	// Vom imparti problema afisarii in 3 ramuri: semnul, coeficientul si exponentul
	for (i = 0; i < nr_termeni; i++) {
		double c = polin_curr.t[i].c;
		int exp = polin_curr.t[i].exp;

		// Mai intai semnul
		if (i == 0) {
			if (c < 0) {
				printf("-");
			}
		} else {
			if (c < 0) {
				printf(" - ");
			} else {
				printf(" + ");
			}
		}

		// Apoi coeficientul. Il vom afisa in modul deoarece deja am rezolvat semnul
		// fabs = abs pentru double sau float
		if (c != 1 && c != -1) {
			printf("%.0lf", fabs(c));
		} else if (exp == 0) {
			// Vom afisa 1 doar daca este termen liber
			printf("%.0lf", fabs(c));
		}
		
		// Apoi exponentul
		if (exp == 0) {
			continue;
		} else if (exp == 1) {
			printf("x");
		} else {
			printf("x^%d", exp);
		}
	}
	printf("\n");
}

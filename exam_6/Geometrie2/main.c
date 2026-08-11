#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	double x, y;
} punct;

typedef struct {
	double a, b;
} dreapta;

void verif_drepte(dreapta d1, dreapta d2, punct *p, short *bool_var)
{
	// Daca gasim drepte concurente, aflam punctul de intersectie.
	// Deoarece folosim double, avem nevoie sa conditionam astfel egalitatea
	// dintre doua double-uri, pentru a fi siguri ca nu avem probleme in cazul
	// im care avem aproape acelasi numar, dar erori la zecimale foarte mici
	if (fabs(d1.a - d2.a) > 0.0001) {
		(*bool_var) = 1;
		
		// Aceste formule se afla din relatia d1.a * x + d1.b = d2.a * x + d2.b
		// (Adica egalam ordonatele) 
		p->x = (d2.b - d1.b) / (d1.a - d2.a);
		p->y = d1.a * p->x + d1.b;
	} else {
		*bool_var = 0;
	}
}

int reallocate_struct(dreapta **dr, int len);
void show_pairs(dreapta *dr, int n);

int main(void)
{
	dreapta *dr;
	int n, i, j;

	scanf("%d", &n);
	dr = (dreapta *)malloc(n * sizeof(dreapta));
	if (!dr) {
		fprintf(stderr, "malloc failed\n");
		return -1;
	}

	// Citim dreptele
	for (i = 0; i < n; i++) {
		scanf("%lf %lf", &dr[i].a, &dr[i].b);
	}

	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			// Eliminam dublurile
			if ((fabs(dr[j].a - dr[i].a) < 0.0001) &&
				(fabs(dr[j].b - dr[i].b) < 0.0001)) {
				for (int k = j; k < n - 1; k++) {
					dr[k].a = dr[k + 1].a;
					dr[k].b = dr[k + 1].b;
				}
				n--;
				j--;
			}
		}
	}

	// Realocam si verificam daca realocarea a reusit
	if (reallocate_struct(&dr, n) == -1) {
		return -1;
	}

	// Afisam dreptele
	show_pairs(dr, n);
	free(dr);

	return 0;
}

// Functie ce realoca structura dupa eliminarea dublurilor
int reallocate_struct(dreapta **dr, int len)
{
	dreapta *aux = (dreapta *)realloc(*dr, len * sizeof(dreapta));
	if (!aux) {
		fprintf(stderr, "realloc failed!\n");
		return -1;
	}

	*dr = aux;
	return 1;
}

void show_pairs(dreapta *dr, int n)
{
	int i, j;

	// Verificam fiecare pereche de drepte si afisam relatia ei cu celelalte drepte
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			short curr_check;
			punct intersect;

			verif_drepte(dr[i], dr[j], &intersect, &curr_check);
			if (curr_check == 0) {
				printf("Drepte paralele:\n");
				printf("dr1: y = %.2lf * x + %.2lf\n", dr[i].a, dr[i].b);
				printf("dr2: y = %.2lf * x + %.2lf\n", dr[j].a, dr[j].b);
				printf("\n");
			} else {
				printf("Drepte concurente:\n");
				printf("dr1: y = %.2lf * x + %.2lf\n", dr[i].a, dr[i].b);
				printf("dr2: y = %.2lf * x + %.2lf\n", dr[j].a, dr[j].b);
				printf("punct comun: x = %.2lf y = %.2lf\n", intersect.x, intersect.y);
				printf("\n");
			}
		}
	}
}
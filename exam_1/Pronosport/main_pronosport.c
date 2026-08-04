#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUME_MAX 30
#define NR_MECIURI 13
#define MECIURI_INPUT 30

typedef struct {
	char *nume, *prenume, meciuri[NR_MECIURI + 1];
	int egal;
} inregistrare;

// Functie ce calculeaza numarul de meciuri egale intre cei doi jucatori
int calc_meciuri(char mec_juc[NR_MECIURI], char *juc_curent)
{
	int i, nr_meciuri = 0;

	for (i = 0; i < NR_MECIURI; i++) {
		if (mec_juc[i] == juc_curent[i]) {
			nr_meciuri++;
		}
	}

	return nr_meciuri;
}


void sort_arr(inregistrare *jucatori, int n)
{
	int i, j;

	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if (jucatori[i].egal < jucatori[j].egal) {
				inregistrare aux = jucatori[i];
				jucatori[i] = jucatori[j];
				jucatori[j] = aux;
			}
		}
	}
}

// Functie ce realiza alocarea dinamica a unui string
char *allocate_string(int str_size, int data_size)
{
	char *aux = (char *)malloc(str_size * data_size);

	if (!aux) {
		fprintf(stderr, "malloc failed!\n");
		return NULL;
	}

	return aux;
}

int main(void)
{
	int n, i, cnt_meci, j, len_meci;
	char meci[MECIURI_INPUT], mec_juc[NR_MECIURI + 1];
	inregistrare *jucatori;

	scanf("%d", &n);
	jucatori = (inregistrare *)malloc(n * sizeof(inregistrare));
	if (!jucatori) {
		fprintf(stderr, "calloc failed\n");
		return -1;
	}
	
	// Incepem citirea fiecarui jucator si retinerea informatiilor despre acestia
	for (i = 0; i < n; i++) {
		jucatori[i].nume = allocate_string(NUME_MAX + 1, sizeof(char));
		jucatori[i].prenume = allocate_string(NUME_MAX + 1, sizeof(char));

		scanf("%s%s%s", jucatori[i].prenume, jucatori[i].nume, meci);
		
		cnt_meci = 0;
		// Extragem doar 1, 2 sau x. Daca aveam ceva in plus in string-ul dat,
		// il vom ignora
		len_meci = strlen(meci);
		// Pentru a nu apela functia strlen in for, vom lua o variabila len_meci
		// care retine lungimea
		for (j = 0; j < len_meci; j++) {
			if (meci[j] == '1' || meci[j] == '2' || meci[j] == 'x') {
				jucatori[i].meciuri[cnt_meci++] = meci[j];
			}
		}
		jucatori[i].meciuri[cnt_meci] = '\0';
	}

	// Citim meciurile de referinta, cu care vom compara rezultatele jucatorilor
	scanf("%s", meci);
	cnt_meci = 0;
	len_meci = strlen(meci);
	for (j = 0; j < len_meci; j++) {
		if (meci[j] == '1' || meci[j] == '2' || meci[j] == 'x') {
			mec_juc[cnt_meci++] = meci[j];
		}
	}
	mec_juc[cnt_meci] = '\0';
	
	// Calculam si afisam cate meciuri corecte a ghicit fiecare jucator
	for (i = 0; i < n; i++) {
		jucatori[i].egal = calc_meciuri(mec_juc, jucatori[i].meciuri);

		printf("%s %s - %d\n", jucatori[i].prenume, jucatori[i].nume, jucatori[i].egal);
	}
	
	// Sortam lista dupa numarul de pronosticuri exacte indicate
	sort_arr(jucatori, n);
	for (i = 0; i < n - 1; i++) {
		printf("%s %s, ", jucatori[i].prenume, jucatori[i].nume);
	}
	printf("%s %s\n", jucatori[i].prenume, jucatori[i].nume);
	
	for (i = 0; i < n; i++) {
		free(jucatori[i].nume);
		free(jucatori[i].prenume);
	}
	free(jucatori);
	
	return 0;
}

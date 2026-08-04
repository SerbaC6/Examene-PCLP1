#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUME_MAX 30
#define NR_MECIURI 13
#define MECIURI_INPUT 26

typedef struct {
	char *nume, *prenume, *meciuri;
	int egal;
} inregistrare;

int calc_meciuri(char mec_juc[NR_MECIURI], char *juc_curent)
{
	int i, nr_mec = 0;
	for (i = 0; i < NR_MECIURI; i++) {
		if (mec_juc[i] == juc_curent[i]) {
			nr_mec++;
		}
	}
	return nr_mec;
}

void swap_strings(char *str_1, char *str_2)
{
	char *str_aux = (char *)calloc(NUME_MAX, sizeof(char));
	if (!str_aux) {
		fprintf(stderr, "calloc failed\n");
		return;
	}
	strcpy(str_aux, str_1);
	strcpy(str_1, str_2);
	strcpy(str_2, str_aux);
	free(str_aux);
}

void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

void sort_arr(inregistrare **jucatori, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if ((*jucatori)[i].egal < (*jucatori)[j].egal) {
				swap_strings((*jucatori)[i].nume, (*jucatori)[j].nume);
				swap_strings((*jucatori)[i].prenume, (*jucatori)[j].prenume);
				swap_int(&(*jucatori)[i].egal, &(*jucatori)[j].egal);
				swap_strings((*jucatori)[i].meciuri, (*jucatori)[j].meciuri);
			}
		}
	}
}

int main(void)
{
	int n, i, cnt_meci, j;
	char meci[MECIURI_INPUT], mec_juc[NR_MECIURI];
	inregistrare *jucatori;
	scanf("%d", &n);
	jucatori = (inregistrare *)calloc(n, sizeof(inregistrare));
	if (!jucatori) {
		fprintf(stderr, "calloc failed\n");
		return -1;
	}
	for (i = 0; i < n; i++) {
		jucatori[i].nume = (char *)calloc(NUME_MAX, sizeof(char));
		if (!jucatori[i].nume) {
			fprintf(stderr, "calloc failed\n");
			return -1;
		}
		jucatori[i].prenume = (char *)calloc(NUME_MAX, sizeof(char));
		if (!jucatori[i].prenume) {
			fprintf(stderr, "calloc failed\n");
			return -1;
		}
		jucatori[i].meciuri = (char *)calloc(NR_MECIURI, sizeof(char));
		if (!jucatori[i].prenume) {
			fprintf(stderr, "calloc failed\n");
			return -1;
		}
		scanf("%s%s%s", jucatori[i].prenume, jucatori[i].nume, meci);
		cnt_meci = 0;
		for (j = 0; j < strlen(meci); j++) {
			if (meci[j] == '1' || meci[j] == '2' || meci[j] == 'x') {
				jucatori[i].meciuri[cnt_meci++] = meci[j];
			}
		}
		jucatori[i].meciuri[cnt_meci] = '\0';
	}
	scanf("%s", meci);
	cnt_meci = 0;
	for (j = 0; j < strlen(meci); j++) {
		if (meci[j] == '1' || meci[j] == '2' || meci[j] == 'x') {
			mec_juc[cnt_meci++] = meci[j];
		}
	}
	mec_juc[cnt_meci] = '\0';
	printf("%s\n", mec_juc);
	for (i = 0; i < n; i++) {
		jucatori[i].egal = calc_meciuri(mec_juc, jucatori[i].meciuri);
		printf("%s %s - %d\n", jucatori[i].prenume, jucatori[i].nume, jucatori[i].egal);
	}
	sort_arr(&jucatori, n);
	for (i = 0; i < n - 1; i++) {
		printf("%s %s, ", jucatori[i].prenume, jucatori[i].nume);
	}
	printf("%s %s\n", jucatori[n - 1].prenume, jucatori[n - 1].nume);
	for (i = 0; i < n; i++) {
		free(jucatori[i].nume);
		free(jucatori[i].prenume);
		free(jucatori[i].meciuri);
	}
	free(jucatori);
	return 0;
}

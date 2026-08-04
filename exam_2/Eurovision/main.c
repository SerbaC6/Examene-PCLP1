#include <stdio.h>
#include <string.h>
#define STR_MAX 17
#define MAX_COUNTRY 50
#define MAX_LINE 222


int search(char table[MAX_COUNTRY][STR_MAX], char country[STR_MAX], int cnt_table);
void swapi(int *a, int *b);
void swapc(char *a, char *b);
void sort(char table[MAX_COUNTRY][STR_MAX], int p, int n, int points[MAX_COUNTRY]);

int main(void)
{
	char table[MAX_COUNTRY][STR_MAX], country[STR_MAX], filename[STR_MAX];
	int points[MAX_COUNTRY] = {0}, descalificat[MAX_COUNTRY] = {0};
	int n, cnt_table = 0, cnt_points = 0, i;
	
	scanf("%d", &n);
	scanf("%s", filename);
	FILE *in = fopen(filename, "rt");
	if (!in) {
		fprintf(stderr, "Can't open %s\n", filename);
		return -1;
	}
		
	char line[MAX_LINE], sep[] = "\n ";
	// Mai intai verificam tarile care vor fi descalificate (NU VOR MAI PUTEA VOTA)
	while (fgets(line, MAX_LINE, in)) {
		char verify_country[STR_MAX];
		char *curr_country;

		curr_country = strtok(line, sep);
		strcpy(verify_country, curr_country);

		for (i = 1; i <= 12; i++) {
			curr_country = strtok(NULL, sep);

			if (strcmp(curr_country, verify_country) == 0) {
				printf("Juriu descalificat: %s\n", verify_country);
				// Marcam faptul ca tara a fost descalificata, deci nu ii vom lua voturile in considerare
				descalificat[cnt_table] = 1; 
				break;
			}
		}
		strcpy(table[cnt_table], verify_country);
		cnt_table++;
	}

	// Ne intoarcem la inceputul fisierului, pentru a face clasamentul
	fseek(in, 0, SEEK_SET);
	while (fgets(line, MAX_LINE, in)) {
		char *curr_country = strtok(line, sep);

		strcpy(country, curr_country);
		// Cautam fiecare tara
		int idx_juriu = search(table, country, n);
		
		// Verificam daca luam in considerare clasamentul tarii
		if (idx_juriu != -1 && descalificat[idx_juriu] != 1) {
			for (i = 0; i < 12; i++) {
				curr_country = strtok(NULL, sep);
				cnt_points = search(table, curr_country, n);

				if (cnt_points != -1) {
					points[cnt_points] += (12 - i);
				}
			}
		}
	}

	printf("Tari fara vot:\n");
	for (i = 0; i < cnt_table; i++) {
		if (points[i] == 0) {
			printf("%s\n", table[i]);
		}
	}

	sort(table, 10, n, points);

	printf("Primele 10 tari:\n");
	for (i = 0; i < 10; i++) {
		printf("%s\n", table[i]);
	}
	fclose(in);
	return 0;
}

// Functie ce cauta un sir de caractere intr-o matrice de caractere, si intoarce
// linia pe care se afla sirul
int search(char table[MAX_COUNTRY][STR_MAX], char country[STR_MAX], int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (strcmp(table[i], country) == 0) {
			return i;
		}
	}
	return -1;
}

// Functie ce realizeaza interschimbarea a doua int-uri
void swapi(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// Functie ce realizeaza interschimbarea a doua siruri de caractere
void swapc(char *a, char *b)
{
	char aux[STR_MAX];

	strcpy(aux, a);
	strcpy(a, b);
	strcpy(b, aux);
}

void sort(char table[MAX_COUNTRY][STR_MAX], int p, int n, int points[MAX_COUNTRY])
{
	int i, j;

	// Aducem in primele p cele mai bune din toate cele n tari
	for (i = 0; i < p; i++) {
		for (j = i + 1; j < n; j++) {
			if (points[i] < points[j]) {
				swapc(table[i], table[j]);
				swapi(&points[i], &points[j]);
			}
		}
	}
}

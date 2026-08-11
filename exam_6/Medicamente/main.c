#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_MAX 20

typedef struct {
	char denumire[STR_MAX + 1];
	float cantitate, pret;
} medicament;

typedef struct {
	char nume[STR_MAX + 1];
	int n;
	medicament *lista;
} reteta;

void citeste_reteta(reteta *ret, FILE *in);
float gaseste_DCI(reteta *ret, int *ok, char *fisier_ret);
void creeaza_bon(reteta ret, FILE *out, float pret);

int main(int argc, char **argv)
{
	if (argc < 4) {
		fprintf(stderr, "Not enough arguments\n");
		return -1;
	}

	FILE *in = fopen(argv[1], "rb");
	if (!in) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	FILE *out = fopen(argv[3], "wb");
	if (!out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	reteta ret_curenta;
	float pret = 0;

	// Incepem citirea fisierului de retete
	while (fread(ret_curenta.nume, sizeof(char), 21, in)) {
		ret_curenta.lista = NULL;
		// Citim reteta si o retinem
		citeste_reteta(&ret_curenta, in);

		int ok = 0;
		pret = gaseste_DCI(&ret_curenta, &ok, argv[2]);

		if (ok == 1) {
			creeaza_bon(ret_curenta, out, pret);
		} else {
			fprintf(stderr, "Nu s-a gasit produs romanesc pentru %s\n", ret_curenta.nume);
		}

		free(ret_curenta.lista);
		pret = 0;
		ok = 0;
	}

	fclose(in);
	fclose(out);

	return 0;
}

void citeste_reteta(reteta *ret, FILE *in)
{
	fread(&ret->n, sizeof(int), 1, in);

	ret->lista = (medicament *)malloc(ret->n * sizeof(medicament));
	if (!ret->lista) {
		fprintf(stderr, "malloc failed\n");
		return;
	}

	for (int i = 0; i < ret->n; i++) {
		fread(ret->lista[i].denumire, sizeof(char), 21, in);
		fread(&ret->lista[i].cantitate, sizeof(float), 1, in);
		ret->lista[i].pret = 0;
	}
}

// Functie ce inlocuieste fiecare DCI cu numele echivalent romanesc si returneaza
// pretul final
float gaseste_DCI(reteta *ret, int *ok, char *fisier_ret)
{
	FILE *echiv = fopen(fisier_ret, "rb");
	if (!echiv) {
		fprintf(stderr, "Can't open file!\n");
		return 0;
	}

	int nr_med = ret->n;
	int i;
	float pret = 0;

	// incepem lista de retete DCI
	for (i = 0; i < nr_med; i++) {
		fseek(echiv, 0, SEEK_SET);

		char DCI[STR_MAX + 1], curr_med[STR_MAX + 1], min_med[STR_MAX + 1];
		float curr_pret, min_pret = 1000;
		int nr_curr, nr, gaseste_echiv = 0;

		while (fread(DCI, sizeof(char), STR_MAX + 1, echiv)) {
			// Daca gasim medicamentul care are aceeasi denumire, il vom citi
			// si apoi ii vom compara pretul cu cel minim
			if (strcmp(DCI, ret->lista[i].denumire) == 0) {
				gaseste_echiv = 1;

				fread(&nr_curr, sizeof(int), 1, echiv);
				for (int j = 0; j < nr_curr; j++) {
					fread(curr_med, sizeof(char), 21, echiv);
					fread(&curr_pret, sizeof(float), 1, echiv);

					if (min_pret > curr_pret) {
						min_pret = curr_pret;
						strcpy(min_med, curr_med);
					}
				}
				// Iesim din while, fiindca am gasit echivalentul
				break;
			} else {
				// Daca nu am gasit echivalentul, vom sari peste reteta
				fread(&nr, sizeof(int), 1, echiv);
				// Din cauza posibilului padding, nu vom pune sizeof(medicament) direct,
				// fiindca ar putea adauga in plus
				fseek(echiv, nr * ((STR_MAX + 1) * sizeof(char) + sizeof(float)), SEEK_CUR);
			}
		}

		// Daca nu am gasit echivalentul, iesim din functie, pentru ca nu vom
		// scrie bonul
		if (gaseste_echiv == 0) {
			fclose(echiv);
			return 0;
		} else {
			strcpy(ret->lista[i].denumire, min_med);
			ret->lista[i].pret = min_pret;
			pret += (min_pret * ret->lista[i].cantitate);
		}
	}

	*ok = 1;
	fclose(echiv);
	return pret;
}

void creeaza_bon(reteta ret, FILE *out, float pret)
{
	int i;

	fwrite(ret.nume, sizeof(char), 21, out);
	fwrite(&ret.n, sizeof(int), 1, out);

	for (i = 0; i < ret.n; i++) {
		float pret_total_medicament = ret.lista[i].cantitate * ret.lista[i].pret;

		fwrite(ret.lista[i].denumire, sizeof(char), 21, out);
		fwrite(&pret_total_medicament, sizeof(float), 1, out);
	}

	fwrite(&pret, sizeof(float), 1, out);
}
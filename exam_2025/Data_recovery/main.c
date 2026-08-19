#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#define uc unsigned char
#define us unsigned short

typedef struct {
	uc d_nume;
	double calitate;
	char *nume, *pic;
	us lin, col;
	uc R, G, B, A;
} gpeg_t;

// Functie ce valideaza poza
int is_valid_gpeg(gpeg_t *gpeg)
{
	us l = gpeg->lin, c = gpeg->col;
	uc sum_R = 0, sum_G = 0, sum_B = 0, sum_A = 0;
	int i;
	
	// Facem sumele de control
	for(i = 0; i < 4 * l * c; i += 4) {
		sum_R ^= gpeg->pic[i];
	}

	for(i = 1; i < 4 * l * c; i += 4) {
		sum_G ^= gpeg->pic[i];
	}

	for(i = 2; i < 4 * l * c; i += 4) {
		sum_B ^= gpeg->pic[i];
	}

	for(i = 3; i < 4 * l * c; i += 4) {
		sum_A ^= gpeg->pic[i];
	}
	
	if(sum_R == gpeg->R && sum_B == gpeg->B && sum_G == gpeg->G &&
	   sum_A == gpeg->A) {
		return 1;
	}

	return 0;
}

int dimensiune_fisier(FILE *in);
int verif_citire(FILE *in, gpeg_t *poza);
void scrie_in_fisier(gpeg_t poza);
char *aloca_sir(int len);

int main(void)
{
	FILE *in = fopen("ssd.bin", "rb");
	if(!in) {
		fprintf(stderr, "Can't open file!\n");
		return -1;
	}
	
	gpeg_t poza;
	poza.nume = NULL;
	poza.pic = NULL;

	// Obtinem dimensiunea fisierului, pentru a putea pune
	// conditia de oprire a buclei
	int dim = dimensiune_fisier(in);
	
	while(1) {
		int cursor = ftell(in);
		
		if (cursor == -1 || cursor >= dim) {
			break;
		}
		
		if (!verif_citire(in, &poza)) {
			// In cazul in care nu s-a putut citi poza in totalitate, vom
			// elibera spatiul alocat in functie si vom trece mai departe
			if (poza.pic) {
				free(poza.pic);
				poza.pic = NULL;
			}

			if (poza.nume) {
				free(poza.nume);
				poza.nume = NULL;
			}

			fseek(in, cursor + 1, SEEK_SET);
		} else {
			// Daca poza a fost citita in intregime, o validam
			int corect = is_valid_gpeg(&poza);
			// Daca nu este valida, eliberam memoria si trecem mai departe
			if (corect == 0) {
				// Daca nu este valida, eliberam memoria si trecem mai departe
				free(poza.pic);
				free(poza.nume);
				poza.pic = NULL;
				poza.nume = NULL;

				fseek(in, cursor + 1, SEEK_SET);
			} else {
				// Daca este, o scriem in fisier
				scrie_in_fisier(poza);
				
				free(poza.pic);
				free(poza.nume);
				poza.pic = NULL;
				poza.nume = NULL;
			}
		}
	}
	
	// Verificam daca mai avem spatiu care nu a fost eliberat
	if (poza.pic) {
		free(poza.pic);
	}

	if (poza.nume) {
		free(poza.nume);
	}

	fclose(in);
	return 0;
}

// Functie ce obtine dimensiunea fisierului
int dimensiune_fisier(FILE *in)
{
	int cursor_curent = ftell(in);
	fseek(in, 0, SEEK_END);

	int dim = ftell(in);
	fseek(in, cursor_curent, SEEK_SET);
	
	return dim;
}

// Functie ce verifica daca citirea pozei s-a facut in totalitate
int verif_citire(FILE *in, gpeg_t *poza)
{
	int dim = dimensiune_fisier(in);
	
	// Se va verifica fiecare citire, iar in cazul in care aceasta nu se
	// realizeaza, se va iesi din functie
	if (!fread(&poza->d_nume, sizeof(uc), 1, in)) {
		return 0;
	}

	if (poza->d_nume == 0 || poza->d_nume > dim) {
		return 0;
	}
	
	poza->nume = aloca_sir((int)poza->d_nume);
	if (!fread(poza->nume, sizeof(char), poza->d_nume, in)) {
		return 0;
	}
	
	if (!fread(&poza->calitate, sizeof(double), 1, in)) {
		return 0;
	}

	if (poza->calitate < 0 || poza->calitate > 1) {
		return 0;
	}
	
	if (!fread(&poza->lin, sizeof(us), 1, in)) {
		return 0;
	}

	if (!fread(&poza->col, sizeof(us), 1, in)) {
		return 0;
	}

	if (poza->lin == 0 || poza->col == 0) {
		return 0;
	}
	
	int cursor = ftell(in);
	if((cursor + 4 * poza->col * poza->lin) >= dim) {
		return 0;
	}
	
	poza->pic = aloca_sir(4 * poza->col * poza->lin);
	fread(poza->pic, sizeof(char), 4 * poza->col * poza->lin, in);
	
	if (!fread(&poza->R, sizeof(uc), 1, in) ||
		!fread(&poza->G, sizeof(uc), 1, in) || 
		!fread(&poza->B, sizeof(uc), 1, in) ||
		!fread(&poza->A, sizeof(uc), 1, in)) {
			return 0;
	}

	return 1;
}

char *aloca_sir(int len)
{
	char *str = (char *)calloc(len + 1, sizeof(char));
	if (!str) {
		fprintf(stderr, "calloc failed!\n");
		return NULL;
	}

	return str;
}

// Functie ce scrie informatiile despre o poza intr-un fisier binar
void scrie_in_fisier(gpeg_t poza)
{
	FILE *out = fopen(poza.nume, "wb");

	if(!out) {
		fprintf(stderr, "Can't open file!\n");
		return;
	}
	
	char antet[4] = {'G', 'P', 'E', 'G'};
	fwrite(antet, sizeof(char), 4, out);
	
	fwrite(&poza.calitate, sizeof(double), 1, out);
	fwrite(&poza.lin, sizeof(us), 1, out);
	fwrite(&poza.col, sizeof(us), 1, out);
	
	fwrite(poza.pic, sizeof(char), 4 * poza.col * poza.lin, out);
	
	fwrite(&poza.R, sizeof(uc), 1, out);
	fwrite(&poza.G, sizeof(uc), 1, out);
	fwrite(&poza.B, sizeof(uc), 1, out);
	fwrite(&poza.A, sizeof(uc), 1, out);
	
	fclose(out);
}
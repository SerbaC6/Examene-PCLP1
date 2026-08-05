#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 17 // 15 litere + '\n' + '\0'
#define MAX_LINE 81
#define CUV_LEN 15

typedef struct {
	int l;
	char cuv[CUV_LEN];
	int freq;
} art_cuv;

int verify_word(char text[MAX_LEN], FILE *f2);
int search_word(FILE *f1, char text[MAX_LEN]);

int main(int argc, char **argv)
{
	if (argc != 5) {
		fprintf(stderr, "Add more arguments!\n");
		return -1;
	}

	int n, m, p;
	char file_in[MAX_LEN], file_out[MAX_LEN + 4], *fp, delim[] = " .,:;-\n/";
	art_cuv cuvant;

	strcpy(file_in, argv[1]);
	fp = strtok(argv[1], delim);

	strcpy(file_out, fp);
	strcat(file_out, ".idx");
	n = atoi(argv[2]);
	m = atoi(argv[3]);
	p = atoi(argv[4]);

	FILE *in = fopen(file_in, "rt");
	// scriem si citim din fisiere
	FILE *f1 = fopen("file_1", "w+t");
	FILE *f2 = fopen("file_2", "w+t");
	if (!in || !f1 || !f2) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, in)) {
		char *word;
		// Scoatem '\n'
		line[strlen(line) - 1] = '\0';
		// Punem cuvintele din "in" in "f1"
		word = strtok(line, delim);
		while (word != NULL) {
			int curr_len = strlen(word);

			if (curr_len >= m && curr_len <= n) {
				fprintf(f1, "%s\n", word);
			}

			word = strtok(NULL, delim);
		}
	}

	fseek(f1, 0, SEEK_SET); //ne intoarcem la inceput ca sa recitim ce am scris
	char text[MAX_LEN];
	while (fgets(text, MAX_LEN, f1)) {
		text[strlen(text) - 1] = '\0';
		// Verificam daca cuvantul curent din f1 a fost deja prelucrat in f2, caz
		// in care sarim peste el
		if (verify_word(text, f2) == 1) {
			continue;
		}

		// Dupa ce aflam numarul de aparitii, scriem in f2 rezultatul
		int app = search_word(f1, text);
		fprintf(f2, "%s %d\n", text, app);
	}

	FILE *out = fopen(file_out, "wb");
	if (!out) {
		fprintf(stderr, "Can't open file!\n");
		return -1;
	}

	// Punem in fisierul de iesire informatia despre fiecare cuvant care respecta
	// numarul de aparitii cerut
	fseek(f2, 0, SEEK_SET);
	while (fgets(line, MAX_LINE, f2)) {
		line[strlen(line) - 1] = '\0';
		
		char *word = strtok(line, delim);
		strcpy(cuvant.cuv, word);
		word = strtok(NULL, delim);
		
		cuvant.freq = atoi(word); 
		cuvant.l = strlen(cuvant.cuv);
		if (cuvant.freq >= p) {
			fwrite(&cuvant, sizeof(art_cuv), 1, out);
		}
	}

	fclose(in);
	fclose(f1);
	fclose(f2);
	fclose(out);
	return 0;
}

// Functie ce verifica daca un sir de caractere a fost deja prelucrat si scris in f2
int verify_word(char text[MAX_LEN], FILE *f2)
{
	long curr_pos = ftell(f2);

	fseek(f2, 0, SEEK_SET);
	char line[MAX_LINE], sep[] = " \n";

	while (fgets(line, MAX_LINE, f2)) {
		line[strlen(line) - 1] = '\0';

		char *word = NULL;
		word = strtok(line, sep);

		if (word) {
			if (strcmp(word, text) == 0) {
				fseek(f2, curr_pos, SEEK_SET);
				return 1;
			}
		}
		continue;
	}
	fseek(f2, curr_pos, SEEK_SET);
	return 0;
}

// Numara aparitiile unui sir de caractere in tot f1
int search_word(FILE *f1, char text[MAX_LEN])
{
	long curr_pos = ftell(f1);
	int nr_ap = 0;

	fseek(f1, 0, SEEK_SET);

	char line[MAX_LEN];
	while (fgets(line, MAX_LEN, f1)) {
		line[strlen(line) - 1] = '\0';

		if (strcmp(line, text) == 0) {
			nr_ap++;
		}
	}

	fseek(f1, curr_pos, SEEK_SET);
	return nr_ap;
}
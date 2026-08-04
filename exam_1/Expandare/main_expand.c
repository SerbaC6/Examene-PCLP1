#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN_MAX 81
#define FILE_MAX 100

void insert_file_content(char *word, FILE *out);
void exclude_delim(char *word);

int main(int argc, char *argv[])
{
	// Facem verificarea de start
	if (argc != 2) {
		fprintf(stderr, "Eroare, nu sunt introduse destule argumente!\n");
	}

	// Deschidem fisierul introdus
	FILE *in = fopen(argv[1], "rt");
	if (!in) {
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		return -1;
	}

	char in_line[LEN_MAX], new_file[FILE_MAX], *p, sep[] = " ";;

	// Formam noul fisier in care vom adauga, pe langa continutul din fisierul
	// dat, expandarea fiecarui include
	p = strtok(argv[1], ".");
	strcpy(new_file, p);
	strcat(new_file, ".inc");

	// Deschidem fisierul in care adaugam continutul
	FILE *out = fopen(new_file, "wt");
	if (!out) {
		fprintf(stderr, "Can't open file %s\n", new_file);
		return -1;
	}

	// incepem sa citim din fisierul introdus
	while (fgets(in_line, LEN_MAX, in)) {
		char line_mask[LEN_MAX], *word;

		// salvam o copie a liniei curente pentru a nu strica continutul acesteia
		strcpy(line_mask, in_line);
		word = strtok(line_mask, sep);
		
		// Daca gasim o linie care are directiva include, incepem
		// extractia continutului fisierului
		if (strcmp(word, "#include") == 0) {
			// Luam "numefisier"
			word = strtok(NULL, "\n");
			// Scoatem ghilimelele
			exclude_delim(word);
			// Inseram continutul in fisierul out
			insert_file_content(word, out);
		} else {
			fprintf(out, "%s", in_line);
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}

// Functie ce adauga continutul unui fisier/"librarii" in fisierul final 
void insert_file_content(char *word, FILE *out)
{
	FILE *lib = fopen(word, "rt");
	if (!lib) {
		fprintf(stderr, "Can't open file %s\n", word);
		return;
	}

	char aux_line[LEN_MAX];
	while (fgets(aux_line, LEN_MAX, lib)) {
		fprintf(out, "%s", aux_line);
	}
	
	fclose(lib);
}

// Functie ce scoate primul si ultimul caracter dintr-un string
void exclude_delim(char *word)
{
	char aux[FILE_MAX];

	strcpy(aux, word + 1);
	strcpy(word, aux);
	
	word[strlen(word) - 1] = '\0';
}

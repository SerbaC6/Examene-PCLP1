#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINE_MAX 81
#define WORD_MAX 70
#define FILE_MAX 101

void begin_extraction(FILE *in, FILE *out, int *files);
void write_lines(FILE *out, FILE *file_in, int start, int nr_lines);
int search_file(char file_mat[FILE_MAX][WORD_MAX], char curr_file[WORD_MAX], int cnt_file);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Can't open file!\n");
		return -1;
	}

	char infile[WORD_MAX], outfile[WORD_MAX], mask[WORD_MAX];

	// Facem formatul .cpl
	strcpy(infile, argv[1]);
	strcpy(mask, argv[1]);
	// Putem avea formatul catalog.txt sau catalog
	char *p = strtok(mask, ".\0");
	strcpy(outfile, p);
	strcat(outfile, ".cpl");

	FILE *in = fopen(infile, "rt");
	if (!in) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	FILE *out = fopen(outfile, "wt");
	if (!out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	int nr_files = 0;
	begin_extraction(in, out, &nr_files);

	fclose(in);
	fclose(out);
	printf("%d\n", nr_files);

	return 0;
}

// Functie ce parcurge fisierul catalog si adauga in fisierul compilatie liniile
// De asemenea, functia calculeaza si numarul de fisiere folosite pentru alcatuirea fisierului
void begin_extraction(FILE *in, FILE *out, int *nr_files)
{
	char line[LINE_MAX], file_mat[FILE_MAX][WORD_MAX];
	int cnt_file = 0;

	while (fgets(line, LINE_MAX, in)) {
		int start, nr_lines;
		char curr_file[WORD_MAX], *str;
		// Extragem informatia din fisierul catalog
		str = strtok(line, " ");
		strcpy(curr_file, str);

		str = strtok(NULL, " ");
		start = atoi(str);

		str = strtok(NULL, "\n");
		nr_lines = atoi(str);

		FILE *file_in = fopen(curr_file, "rt");
		if (!file_in) {
			fprintf(stderr, "Can't open file!\n");
			return;
		}

		// Scriem liniile in fisierul compilatie
		write_lines(out, file_in, start, nr_lines);

		// Verificam daca am mai fost pana acum in fisierul pe care tocmai ce l-am parcurs
		if (search_file(file_mat, curr_file, cnt_file)) {
			strcpy(file_mat[cnt_file++], curr_file);
		}

		fclose(file_in);
	}

	(*nr_files) = cnt_file;
}

// Functie ce scrie liniile din fisierul curent in fisierul compilatie
void write_lines(FILE *out, FILE *file_in, int start, int nr_lines)
{
	char line[LINE_MAX];
	int curr_line = 0;

	while (curr_line != start) {
		fgets(line, LINE_MAX, file_in);
		curr_line++;
		if (feof(file_in) != 0) {
			return;
		}
	}

	// Verificam si daca inca putem citi din fisier
	while (nr_lines > 0 && fgets(line, LINE_MAX, file_in)) {
		fprintf(out, "%s", line);
		nr_lines--;
	}
}

// Functie ce returneaza 0 daca fisierul introdus a fost parcurs anterior
int search_file(char file_mat[FILE_MAX][WORD_MAX], char curr_file[WORD_MAX], int cnt_file)
{
	for (int i = 0; i < cnt_file; i++) {
		if (!strcmp(file_mat[i], curr_file)) {
			return 0;
		}
	}

	return 1;
}
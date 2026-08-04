#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN_MAX 81
#define FILE_MAX 100

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

void exclude_delim(char *word)
{
	char *aux;
	strcpy(aux, word + 1);
	strcpy(word, aux);
	word[strlen(word) - 1] = '\0';
}

int main(int argc, char *argv[])
{
	FILE *in = fopen(argv[1], "rt");
	if (!in) {
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		return -1;
	}
	char in_line[LEN_MAX], new_file[FILE_MAX], *p;
	p = strtok(argv[1], ".");
	strcpy(new_file, p);
	strcat(new_file, ".inc");
	FILE *out = fopen(new_file, "wt");
	if (!out) {
		fprintf(stderr, "Can't open file %s\n", new_file);
		return -1;
	}
	char sep[] = " ";
	while (fgets(in_line, LEN_MAX, in)) {
		char line_mask[LEN_MAX], *word;
		strcpy(line_mask, in_line);
		word = strtok(line_mask, sep);
		if (strcmp(word, "#include") == 0) {
			word = strtok(NULL, "\n");
			exclude_delim(word);
			insert_file_content(word, out);
		} else {
			fprintf(out, "%s", in_line);
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILE_MAX 51
#define LINE_MAX 128

void verify_file(FILE *in, FILE *out, int *valid, int *invalid);
int verify_expr(char c);


int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Not enough arguments!\n");
		return -1;
	}

	// Extragem numele fisierului in care vom pune expresiile corecte
	char filename[FILE_MAX];
	strcpy(filename, "corect_");
	strcat(filename, argv[1]);

	FILE *in = fopen(argv[1], "rt");
	if (!in) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	FILE *out = fopen(filename, "wt");
	if (!out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	int valid = 0, invalid = 0;
	verify_file(in, out, &valid, &invalid);
	printf("%d expresii valide\n", valid);
	printf("%d expresii invalide\n", invalid);
	fclose(in);
	fclose(out);

	return 0;
}

void verify_file(FILE *in, FILE *out, int *valid, int *invalid)
{
	char line[LINE_MAX];

	while (fgets(line, LINE_MAX, in)) {
		int len, ok = 1;

		// Vom verifica corectitudinea unei paranteze printr-un singur contor,
		// care va creste de fiecare data cand intalnim o paranteza deschisa si
		// va scadea daca intalnim o paranteza inchisa. Daca acest contor ajunge
		// mai mic decat 0, expresia nu va fi valida. 
		int valid_expr = 0;

		line[strlen(line) - 1] = '\0';
		len = strlen(line);

		if (line[0] == '#') {
			continue;
		}

		// Vom verifica fiecare caracter din linie
		for (int i = 0; i < len; i++) {
			if (verify_expr(line[i])) {
				if (line[i] == '(') {
					valid_expr++;
				} else if (line[i] == ')') {
					valid_expr--;
				}

				// Vom verifica cazul important in care avem paranteze de genul:
				// '())' 
				if (valid_expr < 0) {
					ok = 0;
					break;
				}
			} else {
				ok = 0;
			}
		}

		// Trebuie NEAPARAT ca la final valid_expr sa fie 0
		if (ok == 0 || valid_expr != 0) {
			(*invalid)++;
			continue;
		} else {
			(*valid)++;
			fprintf(out, "%s\n", line);
		}
	}
}

int verify_expr(char c)
{
	if (c >= '0' && c <= '9') {
		return 1;
	}
	if (c >= 'a' && c <= 'z') {
		return 1;
	}
	if (c >= 'A' && c <= 'Z') {
		return 1;
	}
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ' ') {
		return 1;
	}

	return 0;
}
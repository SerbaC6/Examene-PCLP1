#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 1024

void parse(char *file_path, int k)
{
	FILE *in = fopen(file_path, "r");
	if (!in) {
		fprintf(stderr, "Can't open file!\n");
		return;
	}

	char line[NMAX];
	if (k > 0) {
		// Daca k > 0, afisam doar primele k linii
		int cnt = 0;
		while (cnt < k && fgets(line, NMAX, in)) {
			printf("%s", line);
			cnt++;
		}
	} else if (k < 0) {
		k = -k;
		// Cazul k < 0: citim ultimele k linii din fisier
		// Mai intai aflam numarul total de linii
		int line_tot = 0;
		while (fgets(line, NMAX, in)) {
			line_tot++;
		}

		// cursor_line = linia de la care trebuie sa afisam pana la finalul fisierului
		int cursor_line = line_tot - k, cnt = 0;

		fseek(in, 0, SEEK_SET);
		while (fgets(line, NMAX, in)) {
			if (cnt >= cursor_line) {
				printf("%s", line);
			}

			cnt++;
		}
	}

	fclose(in);
}
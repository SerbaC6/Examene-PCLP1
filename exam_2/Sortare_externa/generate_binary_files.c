// Fisier care ajuta la generarea fisierelor binare pentru a testa codul

// Folosire:
// gcc generate_binary_files.c -o gen
// ./gen f1.bin 2 3 4

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *out = fopen(argv[1], "wb");
	if (!out) {
		fprintf(stderr, "Can't open %s!\n", argv[1]);
		return -1;
	}

	for (int i = 2; i < argc; i++) {
		long nr = atol(argv[i]);
		fwrite(&nr, sizeof(long), 1, out);
	}

	fclose(out);
	return 0;
}
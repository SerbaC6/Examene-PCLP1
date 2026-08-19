#include <stdio.h>
#include <string.h>

void write_num(FILE *out, char *str)
{
	int len = strlen(str);

	fwrite(&len, sizeof(int), 1, out);
	fwrite(str, sizeof(char), len, out);
}

int main(void)
{
	FILE *out = fopen("numere.bin", "wb");
	if (!out) {
		fprintf(stderr, "Eroare la crearea fisierului!\n");
		return 1;
	}

	// Suma totala ar trebui să fie: 15 + 99 + 1 + 9999 = 10114
	write_num(out, "15");
	write_num(out, "99");
	write_num(out, "1");
	write_num(out, "9999");

	fclose(out);
	return 0;
}

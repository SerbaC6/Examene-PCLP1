// Cod ce ajuta la crearea fisierului binar polinoame.bin
#include <stdio.h>

static void scrie_pol(FILE *out, int ntn, int grd, double *c, int *e)
{
	fwrite(&ntn, sizeof(int), 1, out);
	fwrite(&grd, sizeof(int), 1, out);
	for (int i = 0; i < ntn; i++) {
		fwrite(&c[i], sizeof(double), 1, out);
		fwrite(&e[i], sizeof(int), 1, out);
	}
}

int main(void)
{
	FILE *out = fopen("polinoame.bin", "wb");
	if (!out) { 
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	double c1[] = {1.0, -2.0, 2013.0};
	int e1[] = {2012, 1, 0};
	scrie_pol(out, 3, 2012, c1, e1);

	double c2[] = {5.0, 1.0};
	int e2[] = {3, 0};
	scrie_pol(out, 2, 3, c2, e2);

	double c3[] = {-1.0, 2.0, 3.0};
	int e3[] = {5, 3, 0};
	scrie_pol(out, 3, 5, c3, e3);

	double c4[] = {2.0, 1.0, -1.0};
	int e4[] = {1, 7, 0};
	scrie_pol(out, 3, 7, c4, e4);

	double c5[] = {42.0};
	int e5[] = {0};
	scrie_pol(out, 1, 0, c5, e5);

	fclose(out);
	return 0;
}
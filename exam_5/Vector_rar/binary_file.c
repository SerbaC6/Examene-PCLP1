#include <stdio.h>

int main() {
	FILE *f = fopen("nf1.bin", "wb");
	if (!f) return -1;

	int v1_n = 5, v1_nn = 2, v1_x[] = {10, 20}, v1_poz[] = {1, 4};
	fwrite(&v1_n, sizeof(int), 1, f); fwrite(&v1_nn, sizeof(int), 1, f);
	fwrite(v1_x, sizeof(int), 2, f);  fwrite(v1_poz, sizeof(int), 2, f);

	int v2_n = 5, v2_nn = 2, v2_x[] = {5, -5}, v2_poz[] = {1, 3};
	fwrite(&v2_n, sizeof(int), 1, f); fwrite(&v2_nn, sizeof(int), 1, f);
	fwrite(v2_x, sizeof(int), 2, f);  fwrite(v2_poz, sizeof(int), 2, f);

	int v3_n = 10, v3_nn = 1, v3_x[] = {100}, v3_poz[] = {5};
	fwrite(&v3_n, sizeof(int), 1, f); fwrite(&v3_nn, sizeof(int), 1, f);
	fwrite(v3_x, sizeof(int), 1, f);  fwrite(v3_poz, sizeof(int), 1, f);

	int v4_n = 5, v4_nn = 1, v4_x[] = {15}, v4_poz[] = {3};
	fwrite(&v4_n, sizeof(int), 1, f); fwrite(&v4_nn, sizeof(int), 1, f);
	fwrite(v4_x, sizeof(int), 1, f);  fwrite(v4_poz, sizeof(int), 1, f);

	fclose(f);
	return 0;
}
#include <stdio.h>

int main(void)
{
	FILE *f = fopen("bonuri.bin", "rb");

	char nume[21];
	int n;
	
	// Citim cate bonuri exista in fisier
	while(fread(nume, sizeof(char), 21, f)) {
		fread(&n, sizeof(int), 1, f);
		
		printf("Bon pentru: %s\n", nume);
		printf("Medicamente eliberate: %d\n", n);
		
		for(int i = 0; i < n; i++) {
			char med[21];
			float val;
			fread(med, sizeof(char), 21, f);
			fread(&val, sizeof(float), 1, f);
			printf(" - %s: %.2f RON\n", med, val);
		}
		
		float total;
		fread(&total, sizeof(float), 1, f);
		printf("TOTAL DE PLATA: %.2f RON\n", total);
	}
	
	fclose(f);
	return 0;
}
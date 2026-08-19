#include <stdio.h>

int main(void)
{
	// Generam fisierul binar
	FILE *f_bin = fopen("space.bin", "wb");
	
	// Explicatie valori:
	// 0xAA = 10101010 (contine '101' de 3 ori)
	// 0xFF = 11111111 (contine '101' de 0 ori)
	// 0x2D = 00101101 (contine '101' de 2 ori)
	// 0x00 = 00000000 (contine '101' de 0 ori)
	unsigned char bytes[] = {0xAA, 0xFF, 0x2D, 0xAA, 0x00};
	fwrite(bytes, sizeof(unsigned char), 5, f_bin);
	fclose(f_bin);

	// Generam fisierul text
	FILE *f_txt = fopen("analysis.txt", "wt");
	
	// Codul de analizat: 5 (101 in binar)
	fprintf(f_txt, "5\n");
	fprintf(f_txt, "0 - 3\n");  
	fprintf(f_txt, "5 - -2\n"); 
	fprintf(f_txt, "1 - 1\n");  
	
	fclose(f_txt);
	return 0;
}

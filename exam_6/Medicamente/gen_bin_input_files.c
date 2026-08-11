#include <stdio.h>
#include <string.h>

int main(void)
{
	// Generam fisierul cu echivalente
	FILE *f_echiv = fopen("echiv.bin", "wb");
	
	// DCI 1: Paracetamol (2 variante)
	char dci1[21] = "Paracetamol";
	int nr_echiv1 = 2;
	char med1_1[21] = "Para_Terapia", med1_2[21] = "Para_Zentiva";
	float pret1_1 = 5.5, pret1_2 = 4.0;

	fwrite(dci1, sizeof(char), 21, f_echiv);
	fwrite(&nr_echiv1, sizeof(int), 1, f_echiv);

	fwrite(med1_1, sizeof(char), 21, f_echiv);
	fwrite(&pret1_1, sizeof(float), 1, f_echiv);
	
	fwrite(med1_2, sizeof(char), 21, f_echiv);
	fwrite(&pret1_2, sizeof(float), 1, f_echiv);

	// DCI 2: Ibuprofen (2 variante)
	char dci2[21] = "Ibuprofen";
	int nr_echiv2 = 2;
	char med2_1[21] = "Nurofen", med2_2[21] = "Ibu_Arena";
	float pret2_2 = 10.0, pret2_1 = 15.0;

	fwrite(dci2, sizeof(char), 21, f_echiv);
	fwrite(&nr_echiv2, sizeof(int), 1, f_echiv);

	fwrite(med2_1, sizeof(char), 21, f_echiv);
	fwrite(&pret2_1, sizeof(float), 1, f_echiv);

	fwrite(med2_2, sizeof(char), 21, f_echiv);
	fwrite(&pret2_2, sizeof(float), 1, f_echiv);

	fclose(f_echiv);

	// Generam fisierul cu retete
	FILE *f_retete = fopen("retete.bin", "wb");

	// Reteta 1 (Valida): 2 cutii de Paracetamol, 1 de Ibuprofen
	char nume_pacient1[21] = "Ion Popescu";
	int n_meds1 = 2;
	char r1_m1[21] = "Paracetamol"; 
	char r1_m2[21] = "Ibuprofen";
	float r1_c2 = 1.0, r1_c1 = 2.0;   

	fwrite(nume_pacient1, sizeof(char), 21, f_retete);
	fwrite(&n_meds1, sizeof(int), 1, f_retete);

	fwrite(r1_m1, sizeof(char), 21, f_retete);
	fwrite(&r1_c1, sizeof(float), 1, f_retete);
	
	fwrite(r1_m2, sizeof(char), 21, f_retete);
	fwrite(&r1_c2, sizeof(float), 1, f_retete);

	// Reteta 2 (Invalida - contine un DCI inexistent)
	char nume_pacient2[21] = "Maria Ionescu";
	int n_meds2 = 1;
	char r2_m1[21] = "Vitamina_C"; float r2_c1 = 3.0; 

	fwrite(nume_pacient2, sizeof(char), 21, f_retete);
	fwrite(&n_meds2, sizeof(int), 1, f_retete);
	
	fwrite(r2_m1, sizeof(char), 21, f_retete);
	fwrite(&r2_c1, sizeof(float), 1, f_retete);

	fclose(f_retete);
	return 0;
}
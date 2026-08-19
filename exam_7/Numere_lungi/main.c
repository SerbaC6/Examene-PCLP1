#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILE_MAX 25

typedef struct {
	int n;
	char *digits;
} LongInt;

void reallocate_str(char **str, int len);
void calculate_all(LongInt *num, FILE *in);
void search_num(LongInt *x, FILE *in, char *ord);

// x = numarul curent pe care vrem sa il introducem in suma
// sum = pointer catre structura care tine suma
void addLI(LongInt *sum, LongInt x)
{
	// Vom vedea, in prima instanta, daca este nevoie sa realocam sum sau nu.
	// In cazul in care x este mai mare, va trebuia realocam sum la dimensiunea
	// lui x
	int max_len = sum->n;
	if (sum->n < x.n) {
		reallocate_str(&sum->digits, x.n);
		max_len = x.n;
	}
	
	// Vom rezolva cazul in care numerele n-au aceeasi lungime prin
	// verificarea constanta a indiciilor de pe x si sum.
	// In cazul in care unul din ei scade sub 0, pur si simplu vom aduna
	// cu 0 ce punem in suma.
	int cnt_sum = sum->n - 1, cnt_x = x.n - 1, nr_sum, nr_x, cnt_max = max_len - 1;
	int carriage = 0, curr_digit, curr_sum_digit;

	while (cnt_max >= 0) {
		// Verificam daca am trecut de finalul numarului
		if (cnt_sum < 0) {
			nr_sum = 0;
		} else {
			nr_sum = sum->digits[cnt_sum] - '0';
		}

		if (cnt_x < 0) {
			nr_x = 0;
		} else {
			nr_x = x.digits[cnt_x] - '0';
		}
		
		// Suma totala pentru pozitia curenta
		curr_sum_digit = nr_sum + nr_x + carriage;

		// Carriage va fi folosit la retinerea unui 1 in cazul in care
		// suma celor doua cifre depaseste 9
		carriage = curr_sum_digit / 10;
		curr_digit = curr_sum_digit % 10;
		sum->digits[cnt_max] = curr_digit + '0';

		cnt_max--;
		cnt_x--;
		cnt_sum--;
	}

	// Vom verifica si cazul in care avem carriage ramas(9999 + 1 = 10000)
	if (carriage) {
		max_len++;
		reallocate_str(&sum->digits, max_len);

		// Ii facem loc pe prima pozitie lui 1
		for (int i = max_len - 1; i > 0; i--) {
			sum->digits[i] = sum->digits[i - 1];
		}
		sum->digits[0] = '1';
	}

	sum->n = max_len;
}

void printLI(LongInt x)
{
	for (int i = 0; i < x.n; i++) {
		printf("%c", x.digits[i]);
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Load more args\n");
		return -1;
	}

	// scoatem sufixul "-f"
	char file_name[FILE_MAX];
	strcpy(file_name, argv[1] + 2);

	FILE *in = fopen(file_name, "rb");
	if (!in) {
		fprintf(stderr, "Can't open file %s!\n", file_name);
		return -1;
	}

	LongInt num;
	num.n = 0;
	num.digits = NULL;

	if (argc == 2) {
		calculate_all(&num, in);
	} else {
		for (int i = 2; i < argc; i++) {
			LongInt x;
			x.digits = NULL;

			search_num(&x, in, argv[i]);
			// Daca nu a fost gasit, continuam cu urmatorul
			if (!x.digits) {
				continue;
			}
			addLI(&num, x);
			free(x.digits);
			fseek(in, 0, SEEK_SET);
		}
		
		printLI(num);
	}

	if (num.digits) {
		free(num.digits);
	}

	fclose(in);

	return 0;
}

void reallocate_str(char **str, int len)
{
	char *aux = (char *)realloc(*str, len * sizeof(char));
	if (!aux) {
		fprintf(stderr, "relloc failed\n");
		return;
	}

	*str = aux;
}

char *allocate_str(int len)
{
	char *aux = (char *)malloc(len * sizeof(char));
	if (!aux) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	return aux;
}

// Cauta numarul in fisierul binar si il si retine
void search_num(LongInt *x, FILE *in, char *ord)
{
	int nr_fin = atoi(ord);
	int cnt = 1;

	// Vom verifica mai intai daca inca mai avem ce sa citim din fisier
	while (fread(&x->n, sizeof(int), 1, in)) {
		// Daca am ajuns la numarul cerut, il retinem si iesim din bucla
		if (cnt == nr_fin) {
			reallocate_str(&x->digits, x->n);
			fread(x->digits, sizeof(char), x->n, in);
			break;
		}

		fseek(in, sizeof(char) * x->n, SEEK_CUR);
		cnt++;
	}
	// Daca fisierul s-a incheiat inainte sa ajungem la numarul vrut, inseamna ca
	// acesta nu a fost gasit
	if (cnt != nr_fin) {
		x->digits = NULL;
	}
}

// Functie ce calculeaza toate numerele, in cazul in care avem doar comanda
// "add -fnume"
void calculate_all(LongInt *num, FILE *in)
{
	LongInt x;

	while (fread(&x.n, sizeof(int), 1, in)) {
		x.digits = allocate_str(x.n);
		fread(x.digits, sizeof(char), x.n, in);

		addLI(num, x);
		free(x.digits);
	}

	printLI(*num);
}

// Pentru aceasta functie, vom folosi 'x' pentru a citi din ea si a introduce
// rezultatul in 'res', care va fi realocat dupa cum cere 'op'
void procLI(LongInt *res, LongInt x, unsigned int k, char op)
{	
	int max_len;
	// Daca avem inmultire, vom creste 'res' la dimensiunea anterioara(x.n)
	// plus k
	if (op == '*') {
		max_len = x.n + k;
		res->digits = allocate_str(max_len);
		// Punem mai intai 'x' in 'res'
		for (int i = 0; i < x.n; i++) {
			res->digits[i] = x.digits[i];
		}

		// Completam cu zerouri ce a ramas(de la x.n la x.n + k)
		for (int i = x.n; i < max_len; i++) {
			res->digits[i] = '0';
		}
	} else if (op == '/') {
		// Aceeasi idee ca la '*', doar ca in loc sa adaugam, scadem
		max_len = x.n - k;

		// Verificam daca nu cumva k > x.n, caz in care numarul de face 0
		if (max_len <= 0) {
			res->digits = allocate_str(1);
			res->digits[0] = '0';
			res->n = 1;
			return;
		}

		res->digits = allocate_str(max_len);

		// Punem cat putem din 'x' in 'res'
		for (int i = 0; i < max_len; i++) {
			res->digits[i] = x.digits[i];
		}
	}

	res->n = max_len;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Subpunctul a: definirea structurilor
typedef struct {
	int n;
	char *digits;
} LongInt;

typedef struct {
	LongInt num;
	int row, col;
} element;

typedef struct {
	int n;
	int nn;
	element *el_nn;
} rare_matrix;

int check_smaller_LongInt(LongInt a, LongInt b);
void addLI(LongInt *sum, LongInt x);
void procLI(LongInt *res, LongInt x, unsigned int k, char op);
void convLI(LongInt *prod, LongInt x);
void reallocate_str(char **str, int len);
char *allocate_str(int len);
int read_rare_matrix(rare_matrix *mat, FILE *in);
void write_rare_matrix(rare_matrix mat, FILE *out);

// Subpunctul b: opeartia y = y + A * x
void operation(rare_matrix *y, rare_matrix A, rare_matrix x)
{
	// Parcurgem matricea A
	for (int i = 0; i < A.nn; i++) {
		int r = A.el_nn[i].row, c = A.el_nn[i].col;

		// Cautam valoarea corespunzatoare in vectorul x
		int curr_x = -1;
		for (int j = 0; j < x.nn; j++) {
			// Daca coloana din A are valoare corespunzatoare in x(adica exista
			// element nenul pe linia aceea, vom retine indicele elementului)
			if (x.el_nn[j].row == c) {
				curr_x = j;
				break;
			}
		}

		// Daca nu gasim elementul, nu adunam nimic, fiindca e 0
		if (curr_x == -1) {
			continue;
		}

		// Calculam produsul intr-o variabila temporara
		LongInt product;
		product.n = A.el_nn[i].num.n;
		product.digits = allocate_str(product.n);

		for (int k = 0; k < product.n; k++) {
			product.digits[k] = A.el_nn[i].num.digits[k];
		}

		// Aici apelam functia din observatiile cerintei
		convLI(&product, x.el_nn[curr_x].num);

		// Cautam pozitia corespunzatoare(linia) in vectorul y
		int curr_y = -1;
		for (int j = 0; j < y->nn; j++) {
			// Daca gasim, retinem indicele elementului
			if (y->el_nn[j].row == r) {
				curr_y = j;
				break;
			}
		}

		if (curr_y != -1) {
			// Daca elementul exista in y, doar adunam valoarea
			addLI(&y->el_nn[curr_y].num, product);

			// Curatam memoria, deoarece am introdus valoarea si nu mai avem nevoie
			free(product.digits);
		} else {
			// Daca elementul nu exista, il adaugam noi la final
			element *aux = (element *)realloc(y->el_nn, (y->nn + 1) * sizeof(element));
			if (!aux) {
				fprintf(stderr, "realloc failed\n");
				return;
			}
			
			y->el_nn = aux;
			// Atribuim valorile pentru noul element
			y->el_nn[y->nn].row = r;
			y->el_nn[y->nn].col = 0;
			// Facem shallow copy, deci doar mutam pointerul catre product
			y->el_nn[y->nn].num = product;
			y->nn++;
		}
	}
}

// Subpunctul c: verificarea matricilor si scrierea acestora in fisierul cerut
void check_matrixes(FILE *in, LongInt x)
{
	FILE *out = fopen("output.bin", "wb");
	if (!out) {
		fprintf(stderr, "Can't open file!\n");
		return;
	}
	fseek(in, 0, SEEK_SET);

	// Incepem sa citim din fisierul de intrare, dat ca parametru
	rare_matrix curr_mat;
	curr_mat.el_nn = NULL;
	while (read_rare_matrix(&curr_mat, in)) {
		// Verificam daca fiecare numar din matricea rara e mai mare decat
		// numarul dat 
		int ok = 1; // Luam o variabila care contorizeaza daca verificarea a reusit

		for (int i = 0; i < curr_mat.nn; i++) {
			// Daca gasim vreun numar mai mare, inseamna ca verificarea a esuat
			if (!check_smaller_LongInt(curr_mat.el_nn[i].num, x)) {
				ok = 0;
				break;
			}
		}

		// Daca verificarea a reusit, scriem matricea in fisier
		if (ok) {
			write_rare_matrix(curr_mat, out);
		}
	}
	fclose(out);

	if (curr_mat.el_nn) {
		for (int i = 0; i < curr_mat.nn; i++) {
			free(curr_mat.el_nn[i].num.digits);
		}
		free(curr_mat.el_nn);
	}
	fclose(in);
}

// Subpunctul d: impartirea matricilor la 10^k
void divide(FILE *in, unsigned int k)
{
	FILE *out = fopen("divide.bin", "wb");
	if (!out) {
		fprintf(stderr, "Can't open file!\n");
		return;
	}
	fseek(in, 0, SEEK_SET);

	// Incepem sa citim matricile din n
	rare_matrix mat;
	mat.el_nn = NULL;
	// Luam o variabila LongInt, pentru a putea retine numarul inainte de
	// schimbare
	LongInt x;
	x.digits = NULL;

	while (read_rare_matrix(&mat, in)) {
		// Facem schimbarile necesare pentru fiecare numar din matricea curenta
		for (int i = 0; i < mat.nn; i++) {
			// Copiem in x numarul curent
			x.n = mat.el_nn[i].num.n;
			reallocate_str(&x.digits, x.n);

			for (int j = 0; j < x.n; j++) {
				x.digits[j] = mat.el_nn[i].num.digits[j];
			}

			// Eliberam digits, fiindca va fi alocat din nou in functie
			free(mat.el_nn[i].num.digits);

			// Executam operatia
			procLI(&mat.el_nn[i].num, x, k, '/');
		}

		// Apoi scriem matricea
		write_rare_matrix(mat, out);
	}
	fclose(out);

	if (mat.el_nn) {
		for (int i = 0; i < mat.nn; i++) {
			free(mat.el_nn[i].num.digits);
		}
		free(mat.el_nn);
	}

	if (x.digits) {
		free(x.digits);
	}
	fclose(in);
}

// Returneaza 1 daca primul numar e mai mic decat al doilea
int check_smaller_LongInt(LongInt a, LongInt b)
{
	// LongInt fiind un intreg pozitiv, nu vom verifica daca numerele sunt pozitive
	// sau negative
	if (a.n < b.n) {
		return 1;
	}

	if (a.n > b.n) {
		return 0;
	}

	if (a.n == b.n) {
		for (int i = 0; i < a.n; i++) {
			// Cum nu putem folosi functii din string.h, fiindca nu avem
			// terminator de sir, vom recrea strcmp
			if (a.digits[i] < b.digits[i]) {
				return 1;
			} else if (a.digits[i] > b.digits[i]) {
				return 0;
			}
		}
	}
	// Daca numerele sunt egale, returnam 0
	return 0;
}

// Functie ce citeste dintr-un fisier binar o matrice rara
int read_rare_matrix(rare_matrix *mat, FILE *in)
{
	// Verificam daca avem ceva ce nu a fost eliberat din structuri
	if (mat->el_nn) {
		// Eliberam memoria pentru urmatorul numar
		for (int i = 0; i < mat->nn; i++) {
			if (mat->el_nn[i].num.digits) {
				free(mat->el_nn[i].num.digits);
			}
		}

		free(mat->el_nn);
	}

	// Daca am ajuns la capatul fisierului si nu mai avem ce sa citim,
	// inseamna ca citirea a esuat, deci returnam 0
	if (!fread(&mat->n, sizeof(int), 1, in)) {
		return 0;
	}
	fread(&mat->nn, sizeof(int), 1, in);

	mat->el_nn = (element *)calloc(mat->nn, sizeof(element));
	if (!mat->el_nn) {
		fprintf(stderr, "Calloc failed\n");
		return 0;
	}

	// Citim numerele din fisier
	for (int i = 0; i < mat->nn; i++) {
		fread(&mat->el_nn[i].row, sizeof(int), 1, in);
		fread(&mat->el_nn[i].col, sizeof(int), 1, in);
		fread(&mat->el_nn[i].num.n, sizeof(int), 1, in);

		int len = mat->el_nn[i].num.n;
		mat->el_nn[i].num.digits = allocate_str(len);
		fread(mat->el_nn[i].num.digits, sizeof(char), len, in);
	}
	return 1;
}

// Functie ce scrie o matrice rara intr-un fisier binar
void write_rare_matrix(rare_matrix mat, FILE *out)
{
	fwrite(&mat.n, sizeof(int), 1, out);
	fwrite(&mat.nn, sizeof(int), 1, out);

	for (int i = 0; i < mat.nn; i++) {
		fwrite(&mat.el_nn[i].row, sizeof(int), 1, out);
		fwrite(&mat.el_nn[i].col, sizeof(int), 1, out);
		fwrite(&mat.el_nn[i].num.n, sizeof(int), 1, out);

		for (int j = 0; j < mat.el_nn[i].num.n; j++) {
			fwrite(&mat.el_nn[i].num.digits[j], sizeof(char), 1, out);
		}
	}
}




// BONUS:

// Functie ce verifica daca exista zbor intre cele doua orase
int search_flight(rare_matrix mat, int start, int dest)
{
	for (int i = 0; i < mat.nn; i++) {
		if (mat.el_nn[i].row == start && mat.el_nn[i].col == dest) {
			return 1;
		}
	}

	return 0;
}

// Functie ce inmultesc doua matrici rare: C = A * B
rare_matrix multiply_matrixes(rare_matrix A, rare_matrix B)
{
	rare_matrix C;
	C.n = A.n;
	C.nn = 0;
	C.el_nn = NULL;
	LongInt x;
	for (int i = 0; i < A.nn; i++) {
		// Vom retine coloana si linia elementului curent, pentru a vedea daca
		// gasim corespondent in B pentru coloana
		int row_A = A.el_nn[i].row, col_A = A.el_nn[i].col;

		for (int j = 0; j < B.nn; j++) {
			int row_B = B.el_nn[j].row, col_B = B.el_nn[j].col;

			// Daca se leaga drumurile(destinatia lui A e startul lui B)
			if (row_B == col_A) {
				// Vom calcula produsul numarului de drumuri dintre elementul
				// curent de pe A si cel de pe B
				x.n = A.el_nn[i].num.n;
				x.digits = allocate_str(x.n);
				for (int k = 0; k < x.n; k++) {
					x.digits[k] = A.el_nn[i].num.digits[k];
				}

				// Inmultim numerele gasite in A si B
				convLI(&x, B.el_nn[j].num);

				// Cautam daca ruta exista deja in matricea C
				int curr_C = -1;
				for (int cnt = 0; cnt < C.nn; cnt++) {
					if (C.el_nn[cnt].row == row_A && C.el_nn[cnt].col == col_B) {
						curr_C = cnt;
						break;
					}
				}

				// Avem doua optiuni:
				if (curr_C != -1) {
					// Daca ruta exista deja, adunam noile drumuri gasite
					addLI(&C.el_nn[curr_C].num, x);

					free(x.digits);
				} else {
					// Daca ruta nu exista, o adaugam in el_nn al matricei rare C
					element *aux = (element *)realloc(C.el_nn,
													(C.nn + 1) * sizeof(element));
					if (!aux) {
						fprintf(stderr, "realloc failed\n");
						return C;
					}
					C.el_nn = aux;
					// Am dat realloc cu un singur element, ci nu cu un buffer dublu
					// fiindca aceasta operatie nu se va executa de foarte multe ori,
					// iar dublarea array-ului ar insemna o memorie ocupata
					// destul de mare

					C.el_nn[C.nn].row = row_A;
					C.el_nn[C.nn].col = col_B;
					// Facem shallow copy
					C.el_nn[C.nn].num = x;
					C.nn++;
				}
			}
		}
	}

	return C;
}

int minimum_flight_segments(rare_matrix A_orig, int start, int dest)
{
	// Verificam daca avem zbor direct
	if (search_flight(A_orig, start, dest)) {
		return 1;
	}

	int p = 1; // contor care ne va spune numarul minim de segmente de zbor

	// Pentru a putea calcula puterile, avem nevoie de un "A curent" 
	// Facem un deep copy initial ca sa protejam A_orig
	rare_matrix curr_mat;
	curr_mat.n = A_orig.n;
	curr_mat.nn = A_orig.nn;
	curr_mat.el_nn = (element *)calloc(curr_mat.nn, sizeof(element));
	
	for (int i = 0; i < curr_mat.nn; i++) {
		curr_mat.el_nn[i].row = A_orig.el_nn[i].row;
		curr_mat.el_nn[i].col = A_orig.el_nn[i].col;

		curr_mat.el_nn[i].num.n = A_orig.el_nn[i].num.n;
		curr_mat.el_nn[i].num.digits = allocate_str(curr_mat.el_nn[i].num.n);

		for (int j = 0; j < curr_mat.el_nn[i].num.n; j++) {
			curr_mat.el_nn[i].num.digits[j] = A_orig.el_nn[i].num.digits[j];
		}
	}

	// Cautam pana gasim drum.
	// Daca p depaseste numarul de orase, inseamna ca graful 
	// e deconectat si riscam o bucla infinita
	while (!search_flight(curr_mat, start, dest) && p <= A_orig.n) {
		
		// Calculam matematic A^p = A^(p-1) * A_orig
		rare_matrix next_mat = multiply_matrixes(curr_mat, A_orig);

		// Eliberam memoria pentru vechiul A^(p-1) inainte sa il suprascriem
		if (curr_mat.el_nn) {
			for (int i = 0; i < curr_mat.nn; i++) {
				free(curr_mat.el_nn[i].num.digits);
			}
			free(curr_mat.el_nn);
		}

		// Trecem la urmatoarea putere calculata
		curr_mat = next_mat;
		p++;
	}

	// Salvam starea(sa vedem daca ne-am oprit pentru ca l-am gasit
	// sau pentru ca am depasit p)
	int found = search_flight(curr_mat, start, dest);

	// Curatam absolut toata memoria ramasa
	if (curr_mat.el_nn) {
		for (int i = 0; i < curr_mat.nn; i++) {
			free(curr_mat.el_nn[i].num.digits);
		}

		free(curr_mat.el_nn);
	}

	// Daca am gasit drumul, intoarcem p. Daca nu, inseamna ca avem o
	// ruta imposibila
	if (found) {
		return p;
	}
	
	return -1; 
}

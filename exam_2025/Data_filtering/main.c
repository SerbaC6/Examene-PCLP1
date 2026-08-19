#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 31
#define MAX_LINE_SIZE 81
#define MAX_TEXT_SIZE 151

int calc_freq(char *line, char *key);
char *allocate_string(int len);
void calc_final_freq(char *text, int argc, char **argv, FILE *key);
void reallocate_string(char **str, int len);

int main(int argc, char **argv)
{
	// Facem verificarea parametrilor
	if (argc == 1) {
		fprintf(stderr, "No keys added!\n");
		return -1;
	} else if (argc > 25) {
		fprintf(stderr, "Too many keys added!\n");
		return -1;
	}

	char line[MAX_LINE_SIZE], *aggregate_text = NULL;
	int i;
	// Incepem citirea liniilor textului
	while(fgets(line, MAX_LINE_SIZE, stdin)) {
		// Scoatem endline-ul de la finalul fiecarei linii
		if(line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}
		
		// Daca suntem pe ultima linie, iesim din executie
		if(strcmp(line, ".") == 0) {
			break;
		}
		
		int freq[25] = {0}, total_freq = 0;
		// Luam un array care retine frecventa fiecarei key
		for(i = 1; i < argc; i++) {
			freq[i] = calc_freq(line, argv[i]);
			total_freq += freq[i];
		}
		
		// Suntem in cazul in care linia nu continue nicio cheie
		if (total_freq == 0) {
			// Punem "at" fiindca vom da append in fisier, si astfel
			// se deschide si se pune in el DOAR daca este nevoie
			FILE *no_key = fopen("data.txt", "at");

			if (!no_key) {
				fprintf(stderr, "Can't open file\n");
				return -1;
			}

			fprintf(no_key, "%s\n", line);
			fclose(no_key);
		} else {
			// Daca am gasit macar o cheie pe linia curenta 
			if (!aggregate_text) {
				// Daca textul agregat nu era initializat, il initializam
				aggregate_text = allocate_string(strlen(line) + 1);
				strcpy(aggregate_text, line);
			} else {
				// Daca era, incepem verificarile necesare:
				// Daca depaseste lungimea maxima, se scrie in fisierul cerut
				if (strlen(aggregate_text) + strlen(line) > MAX_TEXT_SIZE) {
					// La fel ca mai sus, informatia se pune la finalul fisierului,
					// deci vom da append
					FILE *key = fopen("filtered_data.txt", "at");
					if (!key) {
						fprintf(stderr, "Can't open file!\n");
						return -1;
					}
					// Scriem in fisier informatia ceruta
					// Mai intai scriem textul
					fprintf(key, "%s\n", aggregate_text);
					// Apoi scriem cheile si frecventele de aparitie ale acestora
					// in textul curent
					calc_final_freq(aggregate_text, argc, argv, key);
					fclose(key);
					
					free(aggregate_text);
					// Resetam textul cu linia curenta, dupa ce am pus
					// totul in fisierul cerut
					aggregate_text = allocate_string(strlen(line) + 1);
					strcpy(aggregate_text, line);
				} else {
					// Daca nu depasim lungimea, doar realocam si concatenam linia curenta
					reallocate_string(&aggregate_text, strlen(aggregate_text) + strlen(line) + 1);
					strcat(aggregate_text, line);
				}
			}
		}
	}
	
	// Scriem toate datele ramase in memorie in fisier
	if (aggregate_text != NULL) {
		FILE *key = fopen("filtered_data.txt", "at");
		if (!key) {
			fprintf(stderr, "Can't open file!\n");
			return -1;
		}

		fprintf(key, "%s\n", aggregate_text);
		calc_final_freq(aggregate_text, argc, argv, key);

		fclose(key);
		free(aggregate_text);
	}
	
	return 0;
}

// Functie ce calculeaza frecventa de aparitie a unei chei intr-o linie
int calc_freq(char *line, char *key)
{
	int cnt = 0;
	
	while (strstr(line, key)) {
		cnt++;
		// Vom sari peste cuvantul curent si vom retine partea de text ce se
		// afla dupa el
		line = strstr(line, key) + strlen(key);
	}

	return cnt;
}

char *allocate_string(int len)
{
	char *str = (char *)malloc(len * sizeof(char));

	if (!str) {
		fprintf(stderr, "malloc failed!\n");
		return NULL;
	}

	return str;
}

void reallocate_string(char **str, int len)
{
	char *aux = (char *)realloc(*str, len * sizeof(char));
	if (!aux) {
		fprintf(stderr, "realloc failed!\n");
		return;
	}

	*str = aux;
}

// Functie ce afiseaza in fisierul "filtered_data.txt" fiecare cheie si
// frecventa de aparitie a acesteia in textul dat
void calc_final_freq(char *text, int argc, char **argv, FILE *key)
{
	int i, freq[25] = {0};
	for(i = 1; i < argc; i++) {
		freq[i] = calc_freq(text, argv[i]);
		fprintf(key, "%s %d\n", argv[i], freq[i]);
	}
}
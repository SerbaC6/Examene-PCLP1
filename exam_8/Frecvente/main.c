#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 81
#define MAX_HEX 10

typedef struct {
	int ssize;
	char *seq;
	int freq;
} sequence;

void start_research(FILE *in_txt, FILE *in_bin, FILE *out);
int check_pair(FILE *in_bin, int cursor, int size);
void reallocate_char(char **str, int len);
void find_sequence(sequence *seqv, FILE *in_bin, int cursor, int size);
int find_frequency(sequence seqv, char cod);

int main(void)
{
	// Deschidem fisierele
	FILE *in_txt = fopen("analysis.txt", "rt");
	if (!in_txt) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	FILE *in_bin = fopen("space.bin", "rb");
	if (!in_bin) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	// Vom si scrie in el, dar vom si citi pentru afisare
	FILE *out = fopen("seqs.bin", "w+b");
	if (!out) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}

	// Incepem prelucrarea fisierelor 
	start_research(in_txt, in_bin, out);

	fclose(in_txt);
	fclose(out);
	fclose(in_bin);
	return 0;
}

// Functia ce se ocupa mai intai cu partea de cautare a fiecarui sir din analysis.txt,
// apoi de afisare
void start_research(FILE *in_txt, FILE *in_bin, FILE *out)
{
	char line[LINE_MAX];
	sequence curr_seq;
	int max_freq = -1;
	char bin_cod;

	curr_seq.seq = NULL;
	fgets(line, LINE_MAX, in_txt);
	line[strlen(line) - 1] = '\0';

	// Scoatem codul de analiza din analysis.txt
	bin_cod = (char)atoi(line);

	while (fgets(line, LINE_MAX, in_txt)) {
		int cursor, size;
		// Vom citi direct din linie, cu formatul dat in enunt
		sscanf(line, "%d - %d", &cursor, &size);
		// Ne vom folosi de size pentru a vedea daca mergem fata sau spate(- sau +)
		// iar de ssize pentru operatiile pe structura
		curr_seq.ssize = abs(size);

		// Validam perechea 'start - size'
		if (check_pair(in_bin, cursor, size) == 0) {
			continue;
		}
		
		fseek(in_bin, 0, SEEK_SET);

		reallocate_char(&curr_seq.seq, curr_seq.ssize);
		find_sequence(&curr_seq, in_bin, cursor, size);

		int curr_freq = find_frequency(curr_seq, bin_cod);
		curr_seq.freq = curr_freq;
		if (curr_freq > max_freq) {
			max_freq = curr_freq;
		}

		fwrite(&curr_seq.ssize, sizeof(int), 1, out);
		fwrite(curr_seq.seq, sizeof(char), curr_seq.ssize, out);
		fwrite(&curr_seq.freq, sizeof(int), 1, out);
	}

	fseek(out, 0, SEEK_SET);

	while (fread(&curr_seq.ssize, sizeof(int), 1, out)) {
		// Citim din 'out' tot ce am introdus
		reallocate_char(&curr_seq.seq, curr_seq.ssize);
		fread(curr_seq.seq, sizeof(char), curr_seq.ssize, out);
		fread(&curr_seq.freq, sizeof(int), 1, out);

		// Verificam daca afisam
		if (curr_seq.freq == max_freq) {
			for (int i = 0; i < curr_seq.ssize; i++) {
				// 02X ne ajuta sa afisam in hexazecimal, cu padding de 0-uri in spate,
				// cate 2 caractere hexazecimale per byte, maximul fiind FF(255 in decimal)
				// Vom vrea sa tratam output-ul ca un numar natural, fiindca un numar negativ
				// ar insemna de fapt ca bit-ul de semn este 1, deci vom avea un numar foarte mare
				// asa ca vom casta numarul la un unsigned char, care ia valori intre 0 si 255
				printf("0X%02X ", (unsigned char)curr_seq.seq[i]);
			}
			printf("\n");
		}
	}

	free(curr_seq.seq);
}

// Functie ce valideaza integritatea perechii 'start - size'
int check_pair(FILE *in_bin, int cursor, int size)
{
	if (cursor < 0) {
		return 0;
	}

	fseek(in_bin, 0, SEEK_END);
	int num_bytes = ftell(in_bin);

	if (cursor > num_bytes) {
		return 0;	
	}

	// Verficam, matematic, toate celelalte posibilitati
	if (size < 0) {
	   // Daca ne ducem in stanga, vom verifica cu 0
	   if (cursor + size < 0) {
			return 0;
	   }
	} else {
		// Daca ne ducem in dreapta, vom verifica cu marginea superioara
		if (cursor + size > num_bytes) {
			return 0;
		}
	}

	return 1;
}

// Functie ce realoca un array de char-uri
void reallocate_char(char **str, int len)
{
	char *aux = (char *) realloc(*str, len * sizeof(char));
	if (!aux) {
		fprintf(stderr, "realloc failed!\n");
		return;
	}

	*str = aux;
}

void find_sequence(sequence *seqv, FILE *in_bin, int cursor, int size)
{
	fseek(in_bin, cursor, SEEK_SET);

	// Gasim secventa si o citim din fisier
	if (size > 0) {
		fread(seqv->seq, sizeof(char), seqv->ssize, in_bin);
	} else {
		fseek(in_bin, (-1) * seqv->ssize, SEEK_CUR);
		fread(seqv->seq, sizeof(char), seqv->ssize, in_bin);
	}

}


int find_frequency(sequence seqv, char cod)
{
	int cnt_freq = 0;

	// Pentru a izola cei 3 biti, ne vom folosi de operatii pe biti
	for (int i = 0; i < seqv.ssize; i++) {
		// Vom lua fiecare octet si vom extrage din el secvente de cate 3 biti
		unsigned char curr_byte = seqv.seq[i];

		// In 8 biti intra 6 perechi de 3 biti
		int num_pairs = 6;
		while (num_pairs > 0) {
			// Extragem cate 3 biti folosind 7 (111 in binar)
			char curr_seq = curr_byte & 7;
			
			if (curr_seq == cod) {
				cnt_freq++;
			}

			num_pairs--;
			curr_byte = curr_byte >> 1;
		}
	}

	return cnt_freq;
}
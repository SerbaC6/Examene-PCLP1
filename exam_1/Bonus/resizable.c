#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int buffer = 10, *arr, cnt_arr = 0, nr = 0;

	arr = (int *)calloc(buffer, sizeof(int));
	if (!arr) {
		fprintf(stderr, "calloc failed\n");
		return -1;
	}

	while (1) {
		scanf("%d", &nr);
		if (nr < 0) {
			break;
		}

		if (cnt_arr == buffer) {
			buffer *= 2;
			int *aux = (int *)realloc(arr, buffer * sizeof(int));
			if (!aux) {
				fprintf(stderr, "calloc failed\n");
				return -1;
			}
			arr = aux;
			memset(&arr[buffer / 2], 0, (buffer - (buffer / 2)) * sizeof(int));
		}

		arr[cnt_arr++] = nr;
	}

	for (int i = 0; i < cnt_arr; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	free(arr);
	return 0;
}

// Cresterea cu o unitate:
// Avantaje:
// - nu risipim memorie, deoarece alocam strictul necesar
// - util cand elementele sunt foarte mari sau memoria e limitata, deoarece nu avem spatiu nefolosit
//
// Dezavantaje:
// - se vor face mult mai multe realocari, ceea ce inseamna crestea enorma a apelurilor catre functia realloc, care
// 	 duce la incercarea de a gasi un bloc de memorie nou in care VOM COPIA iarasi toate elementele
// - fragmenteaza heap-ul prin realocari repetate
//
//
// Cresterea dublata:
// Avantaje:
// - se vor face mult mai putine apeluri catre functia realloc, ceea ce inseamna ca nu vom cauta constant
//	 un bloc de memorie nou in care sa copiem toate valorile
//
// Dezavantaje:
// - sanse mari sa risipim memorie si sa alocam un bloc de memorie extrem de mare, sau chiar sa nu gasim un bloc
// 	 destul de mare pentru cresteri 
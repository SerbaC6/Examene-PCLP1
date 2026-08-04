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

//cea cu inmultirea buffer-ului este o metoda mult mai buna decat cea cu realocarea ori de cate ori inseram un nou element in vector, deoarece
//vom face operatia de reallocate de mult mai putine ori, asa ca programul nu va sta sa caute de fiecare data un bloc nou in memorie
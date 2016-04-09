#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

FILE *fp;

void generator(int size){
	srand(time(NULL));
	fp = fopen("input.txt","w");
	int i;
	for(i = 0; i < size; i++){
		int r = rand() % 4294967296;
		fprintf(fp, "%d ", r);
	}
	fclose(fp);
}

int main(){
	generator(50);
	return;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fout(char filename[], char* knot, int** edge, int knotnum, int del) {
	char filenamegv[100];
	strcpy(filenamegv, filename);
	strcat(filenamegv, ".gv");

	FILE* file = fopen(filenamegv, "w");
	fputs("graph ahherewegoagain{\n", file);
	int a = 0;
	for (int i = 0; i < knotnum; i++){
		if(knot[i] != del) {
			for (int j = i; j <= knotnum; j++) {
				if (edge[i][j] == 1 && knot[j] != del){
					a = 1;
					fprintf(file, "\t%c -- %c;\n", knot[i], knot[j]);
				}
			}
			if (a == 0)
				fprintf(file, "\t%c;\n", knot[i]);
		}
		a = 0;
	}
	fputs("}\n", file);
	fclose(file);
	char terminal[100] = "dot -Tpng ";
	strcat(terminal, filenamegv);
	strcat(terminal, " -o");
	strcat(terminal, filename);
	strcat(terminal, ".png");
	system(terminal);	
}

int main() {

	char del;
	int knotnum;
	puts("Number of knots:");
	scanf("%d", &knotnum);

	//'это будет 1массив названий вершин, выделить под него динамическую память'
	char* knot = (char*)malloc(knotnum * sizeof(char));
	int* stepen = (int*)malloc(knotnum * sizeof(int));

	//сделать 2массив под связи и выделить память edge int
	int** edge = (int**)malloc(knotnum * sizeof(int*));
	for (int i = 0; i < knotnum; ++i) {
		edge[i] = (int*)calloc(knotnum, sizeof(int));
	}

	FILE* file = fopen("startgraph.txt", "r"); //считать матрицу смежности
	for (int j = 0; j < knotnum; j++) {
		fscanf(file, "%c", &knot[j]);
		if (j != knotnum-1);
		fgetc(file);
	}

	for (int i = 0; i < knotnum; i++) {//считать связи 
		for (int j = 0; j < knotnum; j++) {
			fscanf(file, "%d", &edge[i][j]);
		}
		fgetc(file);
		fgetc(file);
	}
	fclose(file);


	//ВЫВЕЛИ СТАРЫЙ ГРАФ
	fout("startgraph", knot, edge, knotnum, -1);

	puts("Delete:");
	scanf("%s", &del);

	//НОВЫЙ ГРАФ
	fout("newgraph", knot, edge, knotnum, del);


		//поиск степени вершин
	for (int i = 0; i < knotnum; i++) {
		for (int j = 0; j <= knotnum; j++) {
			if (edge[i][j] == 1 && knot[j] != del) {
				stepen[i] = stepen[i] + 1;
			}
		}
	}

	//сортировка выбором
	int max, frststep, maxj;
	char frstknot;
	for (int i = 0; i < knotnum; i++) {
		max = 0;
		for (int j = i; j < knotnum; j++) {
			if (stepen[j] >= max) {
				max = stepen[j];
				maxj = j;
			}
		}
		frstknot = knot[i];//жоский свап всего чего только можно
		frststep =stepen[i];
		knot[i] = knot[maxj];
		stepen[i] = stepen[maxj];
		knot[maxj] = frstknot;
		stepen[maxj] = frststep;
	}

	puts("Sorted knots:");
	for (int i = 0; i <= knotnum; i++){
		if (knot[i] != del){
			printf ("%c ", knot[i]);
		}
	}
	puts("");

	free(knot);
	free(stepen);
	for (int i = 0; i < knotnum; ++i) {
			free(edge[i]);
		}
		free(edge);
	return 0;
}

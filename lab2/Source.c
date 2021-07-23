/*
 Задание 2.
 Вариант 36.
	Из входного потока вводится непрямоугольная матрица целых чисел [a_ij], i = 1,...,m, j = 1,...,n_i.
	Значения m и n_i заранее неизвестны и вводятся из входного потока.
	Сформировать вектор, упорядочив по возрастанию значения той строки матрицы, в которой сумма элементов максимальна.
Если таких строк несколько, упорядочить одну (любую) строку.
	В дополнительных функциях доступ к элементам массивов реализовать с помощью указателей.
Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<time.h>
#include <locale.h>


//int** InputArray(int m) {
//	int** x;
//	x = (int**)malloc(m * sizeof(int*));
//	for (int i = 0; i < m; i++) {
//		printf("Введите кол-во элементов в строке: ");
//		int n = Input();
//		x[i] = (int*)malloc((n + 1) * sizeof(int)); //кол-во элементов n + 1
//		x[i][0] = n;  //нулевой индекс массива хранит кол-во элементов в нем
//		printf("Введите элементы строки: \n");
//		for (int j = 1; j < n + 1; j++)	{
//			x[i][j] = Input();
//		}
//	}
//	printf("\n");
//	return x;
//}


//Функция ввода матрицы высотой m с заполением псевдорандомными числами от min до max
int** InputArray(int m) {
	int** x;
	x = (int**)malloc(m * sizeof(int*));
	for (int i = 0; i < m; i++) {
		printf("Введите кол-во элементов в строке: ");
		int n;
		scanf("%d", &n);
		x[i] = (int*)malloc((n + 1) * sizeof(int)); //кол-во элементов n + 1
		x[i][0] = n;  //нулевой индекс массива хранит кол-во элементов в нем
		int min = 10, max = 30;
		for (int j = 1; j < n + 1; j++) x[i][j] = rand() % (max - min + 1) + min;
	}
	return x;
}


//Функция вывода матрицы x высотой m
void OutputArray(int** x, int m) {
	for (int i = 0; i < m; i++) {
		for (int j = 1; j < x[i][0] + 1; j++) printf("%6d", x[i][j]);
		printf("\n");
	}
}


//Функция высвобождения памяти, которую занимает непрямоугольная матрица
void FreeArray(int** x, int m) {
	for (int i = 0; i < m; i++) free(x[i]);
	free(x);
}


//Функция нахождения строки с максимальной суммой элементов (начиная с нулевого индекса)
int FindMax(int **x, int m, int *n_res) {
	int max = 0, sum = 0;
	for (int i = 1; i < x[0][0] + 1; i++) max += x[0][i]; //нахождение суммы первой строки для эталона
	int res = 0;
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < x[i][0] + 1; j++) sum += x[i][j];
		if (sum >= max) {
			res = i;
			max = sum;
		}
		sum = 0;
	}
	*n_res = x[res][0];
	return res;
}


//Функция формирования вектора из строки 
int* ResArray(int** x, int n, int n_res) {
	int* a = (int*)malloc((n_res) * sizeof(int));
	for (int i = 0; i < n_res; i++) {
		a[i] = x[n][i + 1];
	}
	return a;
}

//Функция сортировки вектора по возрастанию
void SortBubble(int *x, int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i -1; j++) {
			if (x[j] > x[j + 1]) {
				int tmp = x[j];
				x[j] = x[j + 1];
				x[j + 1] = tmp;
			}
		}
	}
}

//Функция вывода результирующего вектора
void OutputResArray(int* x, int n_res) {
	for (int i = 0; i < n_res; i++) printf("%6d", x[i]);
	printf("\n");
}


int main() {
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	printf("Введите количество строк: \n");
	int m;
	scanf("%d", &m);
	int** x = InputArray(m);
	OutputArray(x, m);
	int n_res = 0; //кол-во элементов в строке с максимальной суммой элементов
	int n = FindMax(x, m, &n_res); //номер строки с максимальной суммой элементов
	int* res = ResArray(x, n, n_res);
	FreeArray(x, m); //очистка двумерного массива
	SortBubble(res, n_res);
	printf("Результирующий вектор, отсортированный по возрастанию: \n");
	OutputResArray(res, n_res);
	free(res); //очистка результирующего вектора
	return 0;
}

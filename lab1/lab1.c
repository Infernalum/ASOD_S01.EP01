/*
Задание 1.
Вариант 19.
   Дана последовательность из n натуральных чисел.
   Для каждого числа исходной последовательности попарно поменять местами рядом стоящие цифры, начиная с младших разрядов,
и определить младшую цифру в записи числа. Для цифры, не имеющей пары, использовать 0. 
   Сформировать новую последовательность, поместив в нее преобразованные числа исходной последовательности. 
Упорядочить полученную последовательность в порядке убывания младших цифр в записи числа. Вывести исходную и полученную последовательности на экран.
   В программе использовать функцию, которая выполняет преобразование числа и возвращает значение младшей цифры в записи исходного числа.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<math.h>

//Функция преобразования числа и нахождения младшей цифры
int ChangeNumber(int *a) {
	int res = 0, deg = 0 /*Степень числа*/, tmp = *a /* Переменная для хранения цифры*/, min = tmp % 10 /* МИнимальная цифра*/;
	do {
		min = (min > (tmp % 10)) ? tmp % 10 : min;
		tmp /= 10;
		deg++;
	} while (tmp != 0);
	tmp = *a;
	while (tmp > 9) {
		int now = *a;
		for (int i = 0; i <= deg; i += 2) {
			now = tmp - ((tmp / 10) * 10);
			res = res + now * pow(10, i + 1);
			tmp /= 10;
			now = tmp - ((tmp / 10) * 10);
			res = res + now * pow(10, i);
			tmp /= 10;
		}
	}
	/*while (temp != 0) {
		last = tmp % 10;
		if (last != min) {
			res += last * deg;
			deg *= 10;
			count++;
		}
		tmp /= 10;
	}*/
	*a = res;
	return min;
}

//int Sorting(int *count, int *out, int n) {
//	for (int i = 0; i < n - 1; i++)
//	{
//		for (int j = 0; j < n - 1; j++) {
//			if (count[j] >= count[j + 1]) {
//				int tmp = count[j];
//				count[j] = count[j + 1];
//				count[j + 1] = tmp;
//				tmp = out[j];
//				out[j] = out[j + 1];
//				out[j + 1] = tmp;
//			}
//		}
//	}
//}

int main() {
	srand(time(NULL));
	int n;
	printf("%s", "Enter the number N:  ");
	scanf("%d", &n);
	int *Mas_In = (int*)malloc(n * sizeof(int)), *Mas_Out = (int*)malloc(n * sizeof(int)), *Mas_Count = (int*)malloc(n * sizeof(int));
	printf("Enter the sequence:\n");
	for (int i = 0; i < n; i++) {
		//scanf("%d", &Mas_In[i]);
		Mas_In[i] = rand();
		Mas_Out[i] = Mas_In[i];
		Mas_Count[i] = ChangeNumber(&Mas_Out[i]);
	}

	//Sorting(&Mas_Count, &Mas_Out, n);
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (Mas_Count[j] <= Mas_Count[j + 1]) {
				int tmp = Mas_Count[j];
				Mas_Count[j] = Mas_Count[j + 1];
				Mas_Count[j + 1] = tmp;
				tmp = Mas_Out[j];
				Mas_Out[j] = Mas_Out[j + 1];
				Mas_Out[j + 1] = tmp;
			}
		}
	}


	printf("Source Sequence:  ");
	for (int i = 0; i < n; i++) {
		printf("%d;   ", Mas_In[i]);
	}
	printf("\nNew Sequence:  ");
	for (int i = 0; i < n; i++) {
		printf("%d - %d; ", Mas_Out[i], Mas_Count[i]);
	}
	return 0;
}

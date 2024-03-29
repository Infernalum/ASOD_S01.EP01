﻿/********************************************************************************************************************************************************************************************************/
/* Лаба 4(а).
  Вариант 10**.
	Написать программу для работы с просматриваемой таблицей по запросам оператора. Просматриваемая таблица организована вектором; каждый элемент таблицы имеет следующую структуру:
  struct Item {
	int busy;      // Признак занятости элемента
	int key;       // Ключ элемента
	int release;   // Номер версии элемента
	char *info;    // Указатель на информацию
  };
	Максимальный размер таблицы ограничен (для создания максимального размера таблицы использовать константу - например, const int SIZE = ...; ).
  Предусмотреть следующие операции:
	- Включение нового элемента в таблицу при условии, что в таблице могут находиться несколько элементов с одинаковыми ключами и разными номерами версий (номер версии элемента формируется как
порядковый номер элемента в последовательности элементов с одинаковыми ключами, определяемый при включении элемента в таблицу);
	- Удаление из таблицы всех версий элемента, заданного ключом, или конкретной (заданной) версии элемента, также заданного своим ключем, без реорганизации таблицы;
	- Поиск в таблице всех версий элемента, заданного ключом, или конкретной (заданной) версии элемента, также заданного своим ключом. Результатом поиска должна быть новая таблица, содержащая
найденные элементы;
	- Вывод содержимого таблицы на экран;
  a) и сама таблица, и информация, относящаяся к элементу таблицы, хранятся в основной памяти;
	Примечания:
  1. Программа должна содержать несколько функций; функция main() должна выполнять: вывод меню, ввод и анализ ответа, вызов на выполнение требуемой функции;
  2. В программе нужно предусмотреть проверку правильности ввода данных;
*/
/********************************************************************************************************************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "lab3_a.h"

#define SIZE 10             // Размер таблицы
#define map ptab->first[i]  // Тупо для выпендрежа и экономии времени - переход в i-тый в элемент таблицы

typedef char *Type;         // Новый тип

typedef struct Item {
	int busy;             // Признак занятости элемента
	int key;              // Ключ элемента
	int release;          // Номер версии элемента
	Type info;            // Указатель на информацию
} Item;


typedef struct Table {      // Тип - посматриваемая таблица - вектор
	int n;                  // Текущий размер таблицы
	Item first[SIZE];       // Таблица - вектор фиксированного размера
} Table;

// Альтерантивы меню для организации диалога
const Type msgs[] = { "0. Выйти", "1. Добавить", "2. Найти", "3. Удалить", "4. Вывести таблицу" };

// Массив диагностических сообщений об ошибках
const Type errmsgs[] = { "Ok", "Таблица переполнена", "Таблица пуста", "Ключ не найден" };

// Кол-во альтернатив (Для вывода в for)
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

/**************************************************************************************************************************************************************/
/*Прототипы функций*/

/* Чет по мелочи */
void clean_stdin();              // Очистка входного потока
int getInt(int*);              // Ввод целого числа из входного потока
Type getStr();                  // Ввод строки произвольной длины из входного потока

int dialog(const Type msgs[], int);      // Диалог

/* Диалоговые функции */
int D_Add(Table *);              // Диалоговая функция включения в таблицу нового элемента
int D_Find(Table *);            // Диалоговая функция поиска всех элементов по ключу или конкретной версии ключа
int D_Delete(Table *);              // Диалоговая функция удаления всех элементов по ключу или конкретной версии ключа
int D_Show(Table *);            // Диалоговая функцяи вывода всей таблицы

/* Табличные функции */
int find_rel(Table *, int);            // Поиск дубликатов и максимальной версии; возвращает номер нужной версии ключа
int insert(Table *, int, Type, int);    // Табличная функция включения в таблицу нового элемента
int find(Table *, int, int);        // Поиск всех элементов по ключу или конкретной версии ключа
int delete(Table *, int, int);          // Табличная функция удаления элемента по ключу и/или его версии
int output(Table *);            // Табличная функция вывода таблицы (ваааще не тавтология)

/**************************************************************************************************************************************************************/


// Массив указателей на функции - для реализации выбора функции; порядок перечисления функций в списке инициализации должен соответсвовать порядку указания альтернатив в списке альернатив
int(*fptr[])(Table *) = { NULL, D_Add, D_Find, D_Delete, D_Show };


/* Выбор номера альтернативы (Функция для выбора номера альтернативы. Возвращает правильный номер альтернативы. В строке может быть указан только номер альтернативы; если в строке
после номера альтернативы есть что-то еще, весь остаток строки удаляется из буфера) */
int dialog(const Type msgs[], int N) {
	Type errmsg = "";
	int rc, i, n;
	do {
		puts(errmsg);
		errmsg = "Ошибка! Повторите ввод:!";
		for (i = 0; i < N; ++i)       // вывод списка альтернатив
			puts(msgs[i]);
		printf("Выберите: --> ");
		n = getInt(&rc);              // Ввод номера альтернативы
		if (n == 0)                   // Конец файла - конец работы
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}


/* Очистка потока ввода */
void clean_stdin() {
	int c;
	do
		c = getchar();    // Присваивание c символа из входного потока
	while (c != '\n' && c != EOF);
}
/* Ввод целого числа из входного потока. Если введенная строка не имеет числа(или переж них находится мусор), юзера просят ввести число еще раз. */
int getInt(int *a) {
	int n;
	do {
		n = scanf("%d", a, sizeof(int));
		if (n < 0) // Обнаружен конец файла
			return 0;
		if (n == 0) { // Обнаружен некорректный символ - ошибка
			printf("%s\n", "Ошибка! Повторите ввод:");
			clean_stdin();
		}
	} while (n == 0);
	clean_stdin();
	return 1;
}

/* Ввод строки произвольной длины из входного потока */
Type getStr() {
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf("%*c");
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}



/* Диалоговая функция включения в таблицу нового элемента. Требует ввести ключ и информацию, при этом эта информация должна быть введена в новой строке, в ответ на приглашение.
Если ключ задан неправильно, вся строка игнорируется.*/
int D_Add(Table *ptab) {
	int k, rc;
	Type info = NULL;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	if (n == 0)
		return 0;   // Обнаружен конец файла
	printf("Введите информация: --> ");
	info = getStr();
	if (info == NULL)
		return 0;  // Обнаружен конец файла
	rc = insert(ptab, k, info, 0); // Вставка элемента в таблицу
	printf("%s: %d   %s\n", errmsgs[rc], k, info);
	return 1;
}

/* Диалоговая функция поиска всех элементов по ключу или конкретной версии ключа. */
int D_Find(Table *ptab) {
	int k, rel, rc;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	printf("Хотите ввести конкретную версию ключа?\n1 - Да; Другой номер - Нет.\n--> ");
	getInt(&rel);
	if (rel == 1) {
		printf("Введите версию: --> ");
		getInt(&rel);
		rc = find(ptab, k, rel);
	}
	else
		rc = find(ptab, k, 0);
	printf("%s: %d\n", errmsgs[rc], k);
	return 1;
}


/* Диалоговая функция удаления всех элементов по ключу или конкретной версии ключа. */
int D_Delete(Table *ptab) {
	int k, rel, rc;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	printf("Хотите ввести конкретную версию ключа?\n1 - Да; Другой номер - Нет.\n--> ");
	getInt(&rel);
	if (
		rel == 1) {
		printf("Введите версию: --> ");
		getInt(&rel);
		rc = delete(ptab, k, rel);
	}
	else
		rc = delete(ptab, k, 0);
	printf("%s: %d\n", errmsgs[rc], k);
	return 1;
}

/* Диалоговая функцяи вывода всей таблицы */
int D_Show(Table *ptab) {
	int rc;
	rc = output(ptab);
	printf("%s\n", errmsgs[rc]);
	return 1;
}



/* Поиск дубликатов и максимальной версии; возвращает номер нужной версии ключа. */
int find_rel(Table *ptab, int k) {
	int rc = 0;
	for (int i = 0; i < SIZE; ++i)
		if (map.busy == 1 && map.key == k && (rc <= map.release))
			rc = map.release;
	return ++rc;
}




/* Табличная функция включения в таблицу нового элемента. В таблицу включается копия передаваемой информации. */
int insert(Table *ptab, int k, Type str, int rel) {
	if (ptab->n == SIZE)
		return 1;                    // Таблица заполнена
	for (int i = 0; i < SIZE; ++i)
		if (map.busy == 0) {
			map.key = k;                          // Запись ключа
			map.info = str;                       // Запись информационной части
			if (rel == 0)
				map.release = find_rel(ptab, k);  // Нахождение версии ключа
			else
				map.release = rel;
			map.busy = 1;                         // Изменения поля занятости на 1
			ptab->n++;                            // Увеличение размера таблицы
			return 0;                             // Элемент успешно добавлен
		}
	return 0;
}

/* Табличная функция нахождения элементов по ключу или конкретной версии ключа. При этом создается новая таблица со всеми найденными элементами */
int find(Table *ptab, int k, int rel) {
	int rc = 3;
	Table result = { 0, NULL };
	for (int i = 0; i < SIZE; ++i)
		if (map.busy == 1 && map.key == k && (map.release == rel || rel == 0)) {
			insert(&result, map.key, map.info, map.release);
			rc = 0;
		}
	output(&result);
	return rc;
}

/* Табличная функция удаления элемента по ключу и/или его версии. */
int delete(Table *ptab, int k, int rel) {
	int rc = 3;
	for (int i = 0; i < SIZE; ++i) {
		if (map.busy == 1 && map.key == k && (map.release == rel || rel == 0)) {
			map.busy = 0;
			ptab->n--;
			free(map.info);
			rc = 0;
		}
	}
	return rc;
}


/* Табличная функция вывода таблицы (ваааще не тавтология). */
int output(Table *ptab) {
	if (ptab->n == 0)
		return 2;         // Таблица пустая
	printf("\n    Ключ   | Версия |  Информация\n");
	for (int i = 0; i < SIZE; ++i)
		if (map.busy == 1)
			printf("%8d  %8d  %7s\n", map.key, map.release, map.info);
	return 0;            // Или не пустая
}

int main() {
	setlocale(LC_ALL, "Russian");
	Table table = { 0 , NULL };
	int rc;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&table))         // Вызывается диалоговая функция, соответствующая номеру альтернативы
			break;
	printf("Спасибо за внимание!\n");
	return 0;
}

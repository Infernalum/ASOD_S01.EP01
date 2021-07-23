#define _CRT_SECURE_NO_WARNINGS

#include "lab4.h"


// Массив диагностических сообщений об ошибках
const Type errmsgs[] = { "Ok", "Дубликат ключа (ключей) в одном из пространств", "Таблица переполнена", "Таблица пуста", "Ключ/ключи не найден/ы" };


// Альтерантивы меню для организации диалога
const Type msgs[] = { "0. Выйти", "1. Добавить", "2. Найти", "3. Удалить", "4. Вывести таблицу" };


// Кол-во альтернатив (Для вывода в for)
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

/**************************************************************************************************************************************************************/
/* Чет по мелочи */


/* Хеш-функция для первого пространства ключей */
int I1(int SZ, int k) {
	return k % SZ;
}

/* Хеш-функция для второго пространства ключей */
int I2(int SZ, int k) {
	return k % SZ;
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

/* Ввод пространства ключей */
void input(int *k1, int* k2) {
	int n, ans;
	Type errmsg = "";
	do {
		puts(errmsg);
		errmsg = "Пространств всего два; используйте числа 1 или 2 для ответа.";
		printf("Введите кол-во ключей, по которым искать элемент: --> ");
		n = getInt(&ans);
	} while ((ans < 1) || (ans > 2));
	errmsg = "";
	if (ans == 1) {
		do {
			puts(errmsg);
			errmsg = "Пространств всего два; используйте числа 1 или 2 для ответа.";
			printf("Введите номер пространства ключа: --> ");
			n = getInt(&ans);
			if (n == 0)
				return 0;
		} while ((ans < 1) || (ans > 2));
		if (ans == 1) {
			printf("Введите ключ из первого пространства ключей: --> ");
			n = getInt(k1);
			if (n == 0)
				return 0;
			return 0;
		}
		else {
			printf("Введите ключ из второго пространства ключей: --> ");
			n = getInt(k2);
			if (n == 0)
				return 0;
			return 0;
		}
	}
	else {
		printf("Введите ключ из первого пространства ключей: --> ");
		n = getInt(k1);
		if (n == 0)
			return 0;
		printf("Введите ключ из второго пространства ключей: --> ");
		n = getInt(k2);
		if (n == 0)
			return 0;
		return 0;
	}
}

/* Добавление элемента в пространство space1/space2 на место с индексом path */
void add(Item *path, int k1, int k2, Type str, int i1, int i2, FILE* fd) {
	path->key1 = k1;
	path->key2 = k2;
	path->ind1 = i1;
	path->ind2 = i2;
	path->len = strlen(str) + 1;				 // Запись в таблицу длины информации (включая нуль-байт)
	fseek(fd, 0, SEEK_END);					     // Позиционирование на конец файла
	path->offset = ftell(fd);					 // Запись в таблицу смещения (относительно начала файла)
}

/* Удаление пространств из таблицы */
void delTable(Table *ptab) {
	free(ptab->space1);
	free(ptab->space2);
}

/**************************************************************************************************************************************************************/
/* Диаллговые функции */

/* Диалоговая функция загрузки таблицы из файла  */
int D_Load(Table *ptab) {
	int SZ;
	Type fname = NULL;
	printf("Введите имя файла: --> ");
	fname = getStr();
	if (fname == NULL)
		return 0;
	if (load(ptab, fname) == 0) {
		printf("Введите потенциальный размер таблицы: --> ");
		if (getInt(&SZ) == 0)
			return 0;
		create(ptab, fname, SZ);
	}
	free(fname);
	return 1;
}



/* Диалоговая функция включения в таблицу нового элемента. Требует ввести ключ и информацию, при этом эта информация должна быть введена в новой строке, в ответ на приглашение.
Если ключ задан неправильно, вся строка игнорируется.*/
int D_Add(Table *ptab) {
	int k1, k2, rc;
	Type info = NULL;
	printf("Введите первый ключ: --> ");
	int n = getInt(&k1);
	if (n == 0)
		return 0;   // Обнаружен конец файла
	printf("Введите второй ключ: --> ");
	n = getInt(&k2);
	if (n == 0)
		return 0;   // Обнаружен конец файла
	printf("Введите информацию: --> ");
	info = getStr();
	if (info == NULL)
		return 0;  // Обнаружен конец файла
	rc = insert(ptab, k1, k2, info); // Вставка элемента в таблицу
	printf("%s: %d - %d %s\n", errmsgs[rc], k1, k2, info);
	free(info);						 // Если элемент вставляется в таблицу - вставляется его копия
	return 1;
}


/* Диалоговая функция поиска всех элементов по ключу или конкретной версии ключа. */
int D_Find(Table *ptab) {
	int k1 = -1, k2 = -1, rc;
	input(&k1, &k2);
	rc = find(ptab, k1, k2);
	if (rc == -1) {
		printf("%s:\n", errmsgs[4]);
		return 1;
	}
	Type info = findInfo(ptab->space1[rc];
	printf("\n|  Ключ 1  |  Ключ 2  |  ind 1  |  ind 2  |       Информация\n");
	printf("|%10d|%10d|%9d|%9d|   %s\n", ptab->space1[rc].key1, ptab->space1[rc].key2, ptab->space1[rc].ind1, ptab->space1[rc].ind2, info , ptab->fd));
	free(info);
	return 1;
}


/* Диалоговая функция удаления по ключу из одного или обоих пространств */
int D_Delete(Table *ptab) {
	int k1 = -1, k2 = -1, rc;
	input(&k1, &k2);
	rc = delete(ptab, k1, k2);
	printf("%s\n", errmsgs[rc]);
	return 1;
}

/* Диалоговая функцяи вывода всей таблицы */
int D_Show(Table *ptab) {
	int rc;
	rc = output(ptab->space1, ptab->SZ, ptab->fd);      // Вывести можно по любому пространству ключей (например, по 1-ому)
	printf("%s\n", errmsgs[rc]);
	return 1;
}


/**************************************************************************************************************************************************************/
/* Табличные фенкции */


/* Табличная функция включения в таблицу нового элемента. В таблицу включается копия передаваемой информации. */
int insert(Table *ptab, int k1, int k2, Type str) {
	int strt1, i1, strt2, i2;      // Для вставки нового элемента производится предварительный поиск места; если в одной из таблиц нашелся введенный ключ - ошибка
	strt1 = i1 = I1(ptab->SZ, k1);
	strt2 = i2 = I2(ptab->SZ, k2);
	while (path1[i1].ind1 > 0) {
		if (path1[i1].key1 == k1)
			return 1;		     // Элемент с заданным ключом есть в таблице
		i1 = (i1 + h1) % (ptab->SZ);    // Следующая позиция
		if (i1 == strt1)
			return 2;		     // Вернулись в исходную позицию - таблица полна
	}
	while (path2[i2].ind2 > 0) {
		if (path2[i2].key2 == k2)
			return 1;
		i2 = (i2 + h2) % (ptab->SZ);
		if (i2 == strt2)
			return 2;
	}
	// Если элемент возможно добавить в оба пространства, то добавляем (хениально)
	add(&ptab->space1[i1], k1, k2, str, i1 + 1, i2 + 1, ptab->fd);
	add(&ptab->space2[i2], k1, k2, str, i1 + 1, i2 + 1, ptab->fd);
	fwrite(str, sizeof(char), ptab->space1[i1].len, ptab->fd);	 // Запись информации в файл
	return 0;
}


/* Табличная функция нахождения элементов по ключу или конкретной версии ключа. При этом создается новая таблица со всеми найденными элементами */
int find(Table *ptab, int k1, int k2) {
	int i1, strt1, i2, strt2;
	strt1 = i1 = I1(ptab->SZ, k1);
	strt2 = i2 = I2(ptab->SZ, k2);
	if (k2 < 0) {
		do {
			if (path1[i1].ind1 > 0 && path1[i1].key1 == k1)
				return i1;               // Возвращаем индекс элемента из первого пространства
			i1 = (i1 + h1) % (ptab->SZ);
			if (i1 == strt1)
				break;
		} while (i1 != strt1);
	}
	if (k1 < 0) {
		do {
			if (path2[i2].ind1 > 0 && path2[i2].key2 == k2)
				return (path2[i2].ind1 - 1);
			i2 = (i2 + h2) % (ptab->SZ);
			if (i2 == strt2)
				break;
		} while (i2 != strt2);
	}
	else {
		do {
			if (path1[i1].ind1 > 0 && path1[i1].key1 == k1 && path1[i2].key2 == k2)
				return i1;
			i1 = (i1 + h1) % (ptab->SZ);
			if (i1 == strt1)
				break;
		} while (i1 != strt1);
	}
	return -1;				// Нужный индекс не найден
}

/* Поиск информации в файле данных по ключу */
Type findInfo(Item map, FILE *fd) {
	Type info = NULL;
	info = (Type)malloc(map.len);
	fseek(fd, map.offset, SEEK_SET);
	fread(info, sizeof(char), map.len, fd);
	return info;
}


/* Табличная функция удаления элемента по ключу. */
int delete(Table *ptab, int k1, int k2) {
	int rc = 4;
	int i1, strt1, i2, strt2;
	strt1 = i1 = I1(ptab->SZ, k1);
	strt2 = i2 = I2(ptab->SZ, k2);
	if (k2 < 0) {
		do {
			if (path1[i1].ind1 == 0)
				return 4;				// Ключ не найден
			else {
				if ((path1[i1].ind1 != -1) && (path1[i1].key1 == k1)) {
					path2[path1[i1].ind2 - 1].ind2 = -1;
					path2[path1[i1].ind2 - 1].ind2 = -1;
					path1[i1].ind1 = -1;
					path1[i1].ind2 = -1;
					return 0;
				}
				else
					i1 = (i1 + h1) % (ptab->SZ);
			}
			if (i1 == strt1)
				return 4;
		} while (i1 != strt1);
	}
	if (k1 < 0) {
		do {
			if (path2[i2].ind2 == 0)
				return 4;				// Ключ не найден
			else {
				if ((path2[i2].ind2 != -1) && (path2[i2].key2 == k2)) {
					path1[path2[i2].ind1 - 1].ind1 = -1;
					path1[path2[i2].ind1 - 1].ind2 = -1;
					path2[i2].ind1 = -1;
					path2[i2].ind2 = -1;
					return 0;
				}
				else
					i2 = (i2 + h2) % (ptab->SZ);
			}
			if (i2 == strt2)
				return 4;
		} while (i2 != strt2);
	}
	else {
		do {
			if (path1[i1].ind2 == 0)
				return 4;				// Ключ не найден
			else {
				if ((path1[i1].ind1 != -1) && (path1[i1].key1 == k1) && (path1[i1].key2 == k2)) {
					path2[path1[i1].ind1 - 1].ind1 = -1;
					path2[path1[i1].ind1 - 1].ind2 = -1;
					path1[i1].ind1 = -1;
					path1[i1].ind2 = -1;
					return 0;
				}
				else
					i1 = (i1 + h1) % (ptab->SZ);
			}
			if (i1 == strt1)
				return 4;
		} while (i1 != strt1);
	}
	return 1;
}


/* Табличная функция вывода таблицы (ваааще не тавтология). */
int output(Item *ptab, int SZ, FILE *fd) {
	int rc = 3;    // Изначально инициализируем таблицу как пустую
	printf("\n|  Ключ 1  |  Ключ 2  |  ind 1  |  ind 2  |       Информация\n");
	for (int i = 0; i < SZ; ++i)
		if (ptab[i].ind1 > 0) {
			Type info = findInfo(ptab[i]);
			printf("|%10d|%10d|%9d|%9d|   %s\n", ptab[i].key1, ptab[i].key2, ptab[i].ind1, ptab[i].ind2, info , fd));
			free(info);
			rc = 0;
		}
	return rc;            // Или не пустую)
}

/**************************************************************************************************************************************************************/
/* Для работы с файлом данных */

/* Функция загрузки существующего файла (если таковой имеется). */
int load(Table *ptab, Type fname) {
	(ptab->fd) = fopen(fname, "r+b");	// Открываем существующий файл на чтение и запись
	if (ptab->fd == NULL)
		return 0;
	fread(&ptab->SZ, sizeof(int), 1, ptab->fd); // Файл открыт; считываем размер вектора
	ptab->space1 = (Item *)calloc((ptab->SZ), sizeof(Item));  // Выделяем место под оба пространства
	ptab->space2 = (Item *)calloc((ptab->SZ), sizeof(Item));
	fread(ptab->space1, sizeof(Item), ptab->SZ, ptab->fd); // Считываем оба пространства
	fread(ptab->space2, sizeof(Item), ptab->SZ, ptab->fd);
	return 1;
}


/* Функция создания нового файла. */
int create(Table *ptab, Type fname, int sz) {
	ptab->SZ = sz;
	ptab->fd = fopen(fname, "w+b");
	if (!(ptab->fd)) {
		ptab->space1 = NULL;
		ptab->space2 = NULL;
		return 0;
	}
	ptab->space1 = (Item *)calloc(ptab->SZ, sizeof(Item));  // Выделяем память под пространство и сразу инициализируем ее нулями;
	ptab->space2 = (Item *)calloc(ptab->SZ, sizeof(Item));
	fwrite(&ptab->SZ, sizeof(int), 1, ptab->fd);		   // Записываем в начало файла размер вектора
	fwrite(ptab->space1, sizeof(Item), ptab->SZ, ptab->fd); // Записываем сами пространства
	fwrite(ptab->space2, sizeof(Item), ptab->SZ, ptab->fd);
	return 1;
}

/*Функция созранения файла данных. */
int save(Table *ptab) {
	fseek(ptab->fd, sizeof(int), SEEK_SET);      // Пропускаем в начале файла поле для задания длины вектора
	fwrite(ptab->space1, sizeof(Item), ptab->SZ, ptab->fd); // Записываем в файл таюлицу
	fwrite(ptab->space2, sizeof(Item), ptab->SZ, ptab->fd); // Записываем в файл таюлицу
	fclose(ptab->fd); // Закрываем файл
	ptab->fd = NULL;
	return 1;
}

/**************************************************************************************************************************************************************/

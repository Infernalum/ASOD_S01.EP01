#include "lab_5.h"

#define Line printf("--------------------------------------------------------------------------------------\n");

// Массив диагностических сообщений об ошибках
const Type errmsgs[] = { "Ок", "Найден дубликат ключа", "Ключ не найден", "Нет свободной памяти" };

// Альтернативы меню для организации диалога
const Type msgs[] = { "0. Выйти", "1. Добавить новый элемент", "2. Найти информацию по ключу", "3. Удалить элемент", "4. Вывести дерево в порядке следования ключей", "5. Заполнить дерево случайными значениями.", "6. Таймирование дерева" };

// Кол-во альтернтатив (для вывода в for)
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);



/**************************************************************************************************************************************/
/* Чет по мелочи */


/* Хеш-функция */
int I(int SZ, int k) {
	return k % SZ;
}


/* Очистка потока ввода */
void clean_stdin() {
	int c;
	do c = getchar();			// Присваивание с символа из входного потока
	while (c != '\n' && c != EOF);
}


/* Ввод целого числа из входного потока. Если введенная строка не имеет числа (или перед ним находится мусор), юзера просят ввести число еще раз. */
int getInt(int *a) {
	int n;
	do {
		n = scanf("%d", a, sizeof(int));
		if (n < 0)			// Обнаружен конец файла
			return 0;
		if (n == 0) {		// Обнаружен некорректный символ - ошибка
			printf("%s\n", "Ошибка! Повторите ввод:");
			clean_stdin();
		}
	} while (n == 0);
	clean_stdin();
	return 1;
}


/* Ввод строки произвольной длины из входного потока */
Type getStr() {
	Type ptr = (Type)malloc(1);
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
			ptr = (Type)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}


/* Конвертер инта в строку. */
Type ItCh(int k) {
	int i = 1;
	int p = k, c = 0;
	do {
		++c;
		p /= 10;
	} while (p > 0);
	Type res = (Type)malloc(c);
	do {
		res[c - i] = (k % 10) + 48;
		k /= 10;
		++i;
	} while (k > 0);
	res[c] = '\0';
	return res;
}


/* Выбор номера альтернативы (Функция для выбора номера альтернативы. Возвращается правильный номер альтернативы. В строке может быть указан
только номер альтернативы; если в строке после номера альтернативы есть что-то еще, весь остаток строки удаляется из буфера) */
int dialog(const Type msgs[], int N) {
	Type errmsgs = "";
	int rc, i, n;
	do {
		puts(errmsgs);
		errmsgs = "Ошибка! Повторите ввод:!";
		for (i = 0; i < N; ++i)					// Вывод списка альтернатив
			puts(msgs[i]);
		printf("Выберите: --> ");
		n = getInt(&rc);						// Ввод номера альтернативы
		if (n == 0)								// Конец файла - конец работы
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}


/**************************************************************************************************************************************/
/* Диалоговая функция */

/* Диалоговая функция включения в дерево нового узла. Требует ввести ключ и информацию; при этом эта информация должна быть введена в новой строке, в ответ на приглашение.
Если ключ задан неправильно, вся строка игнорируется. */
int D_Add(RB_Tree *ptree) {
	int k, rc = 0;
	Type info = NULL;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	if (n == 0)
		return 0;
	printf("Введите информацию: --> ");
	info = getStr();
	if (info == NULL)
		return 0;
	int len = strlen(info) + 1;
	fseek(ptree->fd_data, 0, SEEK_END);
	int offset = ftell(ptree->fd_data);
	rc = insert(ptree, k, len, offset);
	fwrite(info, sizeof(char), len, ptree->fd_data);
	free(info);								// Вставляется копия информации
	printf("%s: %d\n", errmsgs[rc], k);
	Line;
	return 1;
}


/* Диалоговая фунция поиска узла дерева по условию лабы. */
int D_Find(RB_Tree *ptree) {
	Line;
	int k, rc;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	if (n == 0)
		return 0;
	rc = find(*ptree, k, 1);
	printf("%s: %d\n", errmsgs[rc], k);
	Line;
	return 1;
}


/* Диалоговая функция удаления узла дерева по значению ключа. */
int D_Delete(RB_Tree *ptree) {
	int k, rc;
	Line;
	printf("Введите ключ: --> ");
	int n = getInt(&k);
	if (n == 0)
		return 0;
	rc = RB_Remove(&ptree->root, k);
	int i = I(ptree->buffer.SZ, k);
	if (!rc && ptree->buffer.hash[i].k == k) {
		ptree->buffer.hash[i].link = NULL;
		ptree->buffer.hash[i].k = 0;
	}
	printf("%s: %d\n", errmsgs[rc], k);
	Line;
	return 1;
}


/* Диалоговая функция вывода всего дерева в парвильном порядке следования ключей. (Центрированный обход: ЛКП) */
int D_Show(RB_Tree *ptree) {
	/* Вывод кэша. */
	Line;
	if (!ptree->root == &EList)
		printf("%s", errmsgs[2]);
	printf("Данные кэша:\n   |  Ключ  |   Информация\n");
	for (int i = 0; i < ptree->buffer.SZ; ++i) {
		if (ptree->buffer.hash[i].link)
			if (ptree->buffer.hash[i].link->len != 0) {
				Type info = (Type)malloc(ptree->buffer.hash[i].link->len);
				fseek(ptree->fd_data, ptree->buffer.hash[i].link->offset, SEEK_SET);
				fread(info, sizeof(char), ptree->buffer.hash[i].link->len, ptree->fd_data);
				printf("%d   %6d    %s\n", i, ptree->buffer.hash[i].k, info);
				free(info);
			}
	}
	Line;
	/* Вывод дерева. */
	RB_Preorder(ptree->root, ptree->fd_data);
	Line;
	return 1;
}


/* Диалоговая функция заполнения дерева случайными ключами. */
int D_Random(RB_Tree *ptree) {
	srand(time(NULL));
	int n = 0;
	printf("Введите число добавляемых элементов: --> ");
	getInt(&n);
	for (int k = 0; k < n; ++k) {
		int key = rand() % (1000 - 1 + 1) + 1;
		int len = rand() % (10 - 10 + 1) + 10;
		Type info = (Type)malloc(len);
		for (int i = 0; i < len - 1; ++i)
			info[i] = rand() % (122 - 97 + 1) + 97;
		info[len - 1] = '\0';
		fseek(ptree->fd_data, 0, SEEK_END);
		int offset = ftell(ptree->fd_data);
		if (!insert(ptree, key, len, offset));
		fwrite(info, sizeof(char), len, ptree->fd_data);
		free(info);
	}
}



/* Диалоговая функция таймирования дерева. (Аргумент функции не нужен, т.к. создается новое дерево)*/
int D_Timing(RB_Tree *p) {
	RB_Tree tree = { NULL, NULL, &EList, {10, NULL} };
	tree.buffer.hash = (Node *)malloc(sizeof(Node) * tree.buffer.SZ);
	int n = 10, key[10000], k, cnt = 1000000, i, m, t;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		for (i = 0; i < 10000; ++i)
			key[i] = rand() * rand();
		for (i = 0; i < cnt; ) {
			k = rand() * rand();
			t = insert(&tree, k, 0, 0);
			if (t == 4) {
				printf("Не удалось выделить память.");
				exit;
			}
			else if (!t)
				++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
			if (!find(tree, key[i], 0))
				++m;
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n)*cnt, (last - first));
	}
	deltree(&tree.root);
	return 1;
}


/**************************************************************************************************************************************/
/* Функции для работы с деревом. */


/* Вставка ключа в таблицу. */
int insert(RB_Tree *ptree, int k, int len, int offset) {
	/* Вставляем элемент в кэш буфер */
	int i = I(ptree->buffer.SZ, k);
	Node *link = RB_Insert(&ptree->root, k, len, offset);
	if (link == 4)
		return 4;
	if (!link)
		return 1;
	ptree->buffer.hash[i].k = k;
	ptree->buffer.hash[i].link = link;
	return 0;
}


/* Вставка ключа в дерево. */
Node *RB_Insert(Node **proot, int k, int len, int offset) {
	Node *x = *proot;					// Текущий узел
	Node *par = &EList;					// Родитель икса
	while (x != &EList) {
		if (x->key == k)
			return NULL;			// Найден дубликат
		par = x;
		x = k < x->key ? x->left : x->right;
	}
	x = (Node *)malloc(sizeof(Node));
	if (!x)
		return 4;
	x->key = k;
	x->parent = par;
	x->left = x->right = &EList;
	x->len = len;
	x->offset = offset;
	x->color = RED;
	if (x->parent == &EList)
		*proot = x;
	else {
		if (x->key < par->key)
			par->left = x;
		else
			par->right = x;
	}
	RB_Insert_Fixup(proot, x);
	return x;
}


/* Левый поворот. */
void left_rotate(Node **root, Node *x) {
	Node *y = x->right;  // Старший сын икса
	Node *p = x->parent; // Родитель икса
	/* 1. Формирование связи x - младший сын y */
	/* Переустановить старшего сына x */
	x->right = y->left;
	/* Переустановить родительский узел левого поддерева y (если он есть) */
	if (y->left != &EList)
		y->left->parent = x;
	/* 2. Формирование связи p - y */
	/* Переустановить родительский узел y */
	y->parent = p;
	/* Возможно, x был корнем дерева */
	if (p == &EList)
		*root = y;
	else {
		/* Переустановить левое или правое поддерево родительского узла x */
		if (p->left == x)
			p->left = y;
		else
			p->right = y;
	}
	/* 3. Формирование связи y - x */
	/* Переустановить лквое поддерево y */
	y->left = x;
	/* Переустановить родительский узел для x */
	x->parent = y;
	/* Время выполнения: O(1) */
}


/* Правый поворот. */
void right_rotate(Node **root, Node *x) {
	Node *y = x->left;	  // Младший сын икса
	Node *p = x->parent;  // Родитель икса
	/* 1. Формирование связи x - старший сын y */
	/* Переустановить младшего сына x */
	x->left = y->right;
	/* Переустановить родителя младшего сына y (если он есть) */
	if (y->right != &EList)
		y->right->parent = x;
	/* 2. Формирование связи p - y */
	/* Переустановить родителя узел y */
	y->parent = p;
	/* Возможно, x был корнем дерева */
	if (p == &EList)
		*root = y;
	else {
		/* Переустановить одного из сыновей родителя x */
		if (p->right == x)
			p->right = y;
		else
			p->left = y;
	}
	/* 3. Формирование связи y - x */
	/* Переустановить лквое поддерево y */
	y->right = x;
	/* Переустановить родительский узел для x */
	x->parent = y;
	/* Время выполнения: O(1) */
}


/* Балансировака дерева при добавлении узла */
/* Анализ эффективности: вставка в дерево - О(h); Коррекция - для 1 случая О(h), для 2-3 максимум два поворота; т.к. h <= 2lg(n + 1) -> вставка O(h) */
void RB_Insert_Fixup(Node **proot, Node *x) {
	/* Если мы добавили корень, то необходимо его перекрасить */
	if (x == *proot) {
		x->color = BLACK;
		return;
	}
	/* Пока нарушается свойство 3 (Любой красный узел имеет тлько черные поддеревья), выполняем */
	while (x->parent->color) {
		Node *pa = x->parent;			// Родитель икса
		Node *grandpa = pa->parent;     // "Прародитель" икса
			/* Родитель - младший сын */
		if (pa == grandpa->left) {
			Node *p2 = grandpa->right;
			/* Если есть дядя и он красный */
			if (p2->color == RED) { /*АААААААААААААААААААААААААААААААААААА БЛЯЯЯЯЯЯЯЯЯЯЯЯЯТЬ*/
				/* Случай 1: просто перекрашиваем узлы */
				pa->color = BLACK;
				p2->color = BLACK;
				grandpa->color = RED;
				x = grandpa;
			}
			/* Или случай, когда нет дяди */
			else
				/* Случай 2: Если x - старший сын */
				if (pa->right == x) {
					x = pa;
					left_rotate(proot, x);
					pa = x->parent;
				}
			/* Случай 3: перекрашиваем нормально вершины */
				else {
					pa->color = BLACK;
					grandpa->color = RED;
					right_rotate(proot, grandpa);
				}
		}
		/* Если же родитель - старший сын */
		else {
			/* Если есть дядя - 1 случай*/
			Node *p2 = grandpa->left;
			if (p2->color == RED) {
				/* Случай 1: просто перекрашиваем узлы */
				pa->color = BLACK;
				p2->color = BLACK;
				grandpa->color = RED;
				x = grandpa;
			}
			/* Или случай, когда нет дяди */
			else
				/* Если x - младший сын, осуществляем правый поворот относительно родителя икса */
				if (pa->left == x) {
					x = pa;
					right_rotate(proot, x);
					pa = x->parent;
				}
				else {
					pa->color = BLACK;
					grandpa->color = RED;
					left_rotate(proot, grandpa);
				}
		}
	}
	(*proot)->color = BLACK; /* Восстанавливаем свойство корня */
}


/* Нахождение ключа в дереве. */
int find(RB_Tree tree, int k, int ind) {
	if (!tree.buffer.SZ)
		return 2;
	/* Поиск в кэше */
	int i = I(tree.buffer.SZ, k);
	if (tree.buffer.hash[i].k == k && tree.buffer.hash[i].link) {
		Type info = (Type)malloc(tree.buffer.hash[i].link->len);
		if (ind) {
			fseek(tree.fd_data, tree.buffer.hash[i].link->offset, SEEK_SET);
			fread(info, sizeof(char), tree.buffer.hash[i].link->len, tree.fd_data);
			printf("%d  %6d   %s\n", i + 1, tree.buffer.hash[i].k, info);
			free(info);
		}
		return 0;
	}
	/* Поиск в дереве */
	else {
		Node *res = RB_Find(tree.root, k);
		if (res == &EList)
			return 2;
		if (ind) {
			Type info = (Type)malloc(res->len);
			fseek(tree.fd_data, res->offset, SEEK_SET);
			fread(info, sizeof(char), res->len, tree.fd_data);
			printf("%d - %s\n", res->key, info);
			free(info);
		}
		return 0;
	}
	return 3;    // Ключ не найден
}


/* Поиск узла в дереве */
Node *RB_Find(Node *root, int k) {
	if ((root == &EList) || (root->key == k))
		return root;
	if (k < root->key)
		return RB_Find(root->left, k);
	else
		return RB_Find(root->right, k);
}


/* Вывод всего дерева в прямом порядке следования ключей. */
void RB_Preorder(Node *root, FILE *fd_data) {
	if (root != &EList) {
		Type info = (Type)malloc(root->len);
		fseek(fd_data, root->offset, SEEK_SET);
		fread(info, sizeof(char), root->len, fd_data);
		printf("%d - %s\n", root->key, info);
		free(info);
		RB_Preorder(root->left, fd_data);
		RB_Preorder(root->right, fd_data);
	}
}



/* Удаление узла из дерева. */
int RB_Remove(Node **proot, int k) {
	Node *x = &EList;
	Node *y = &EList, *p = &EList;
	/* Находим удаляемый узел */
	x = RB_Find(*proot, k);
	if (x == &EList)
		return 2;
	/* 1. Если x имеет детей */
	if ((x->left == &EList) || (x->right == &EList))
		y = x;
	else {
		y = x->right;
		if (x->right->left != &EList)
			y = y->left;
		while (y->left != &EList)
			y = y->left;
	}
	if (y->left != &EList)
		p = y->left;
	else
		p = y->right;
	p->parent = y->parent;
	/* Если удаляется корень дерева. */
	if (y->parent == &EList)
		*proot = p;
	else if (y->parent->left == y)
		y->parent->left = p;
	else
		y->parent->right = p;
	if (y != x) {
		x->key = y->key;
		x->len = y->len;
		x->offset = y->offset;
	}
	if (y->color == BLACK)
		RB_Remove_Fixup(proot, p);
	return 0;
}



void RB_Remove_Fixup(Node **proot, Node *x) {
	while (x != *proot && x->color == BLACK) {
		Node *p = x->parent;
		if (x == p->left) {
			Node *w = p->right; // Второй потомок родителя (Брат)
			/* 1 случай: брат красный */
			if (w->color == RED) {
				w->color = BLACK; // Поменять цвета у брата и родителя
				p->color = RED;
				left_rotate(proot, p);
				w = p->right;
			}
			/* 2 случай: у брата оба потомка черные. */
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;	    	// Забрать черную окраску у брата
				x = p;				    // ПЕреместиться вверх по дереву
			}
			else {
				/* Случай 3: правый потомок брата черный */
				if (w->right->color == BLACK) {
					w->color = RED;				// Меняем цвета у w и w->left
					w->left->color = BLACK;
					right_rotate(proot, w);		// Правый поворот вокруг w
					w = p->right;
				}
				/* Случай 4: у x есть дополнительный красный предок. */
				w->color = p->color;			// Перекрашиваем узлы
				p->color = BLACK;
				w->right->color = BLACK;
				left_rotate(*proot, p);			// Левый поворот вокруг p
				x = *proot;
			}
		}
		else {
			Node *w = p->left; // Второй потомок родителя (Брат)
			/* 1 случай: брат красный */
			if (w->color == RED) {
				w->color = BLACK; // Поменять цвета у брата и родителя
				p->color = RED;
				right_rotate(proot, p);
				w = p->left;
			}
			/* 2 случай: у брата оба потомка черные. */
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;	    	// Забрать черную окраску у брата
				x = p;				    // ПЕреместиться вверх по дереву
			}
			else {
				/* Случай 3: правый потомок брата черный */
				if (w->left->color == BLACK) {
					w->color = RED;				// Меняем цвета у w и w->left
					w->right->color = BLACK;
					left_rotate(proot, w);		// Левый поворот вокруг w
					w = p->left;
				}
				/* Случай 4: у x есть дополнительный красный предок. */
				w->color = p->color;			// Перекрашиваем узлы
				p->color = BLACK;
				w->left->color = BLACK;
				right_rotate(*proot, p);		// Левый поворот вокруг p
				x = *proot;
			}
		}
	}
	x->color = BLACK;
}


/* Рекурсивная функция удаления поддерева из оперативной памяти. */
void deltree(Node **proot) {
	if (*proot != &EList) {
		deltree(&(*proot)->left);
		deltree(&(*proot)->right);
		free(*proot);
	}
}


/**************************************************************************************************************************************/
/* Функции для работы с файлом данных */


/* Функция загрузки таблицы из файла */
int D_Load(RB_Tree *ptree, Type *fname) {
	Line;
	printf("Введите общее имя для файлов\n(без типа файла; файлы будут созданы вастоматически:\n'Имяфайла.bin' , 'Имяфайла_data.bin' и 'Имяфайла.dot'): --> ");
	*fname = getStr();
	if (fname == NULL)
		return 0;
	if (!load(ptree, *fname)) {
		if (!create(ptree, *fname)) {	
			printf("Возникла непредвиденная ошибка при моздании файлов. Попробуйте еще раз.");
			return 0;
		}
	}
	return 1;
}


/* Функции загрузки существующего файла (если таковой имеется). */
int load(RB_Tree *ptree, Type fname) {
	/* Открываем существующие файлы на чтение и запись; в случае не нахождеия одного из нужных файлов, выдется сообщение об ошибке */
	Type fname_keys = (Type)malloc(strlen(fname) + 5);
	strcpy(fname_keys, fname);
	strcat(fname_keys, ".bin");
	ptree->fd_keys = fopen(fname_keys, "r+b");
	free(fname_keys);
	if (ptree->fd_keys == NULL)
		return 0;
	Type fname_data = (Type)malloc(strlen(fname) + 10);
	strcpy(fname_data, fname);
	strcat(fname_data, "_data.bin");
	ptree->fd_data = fopen(fname_data, "r+b");
	free(fname_data);
	if (ptree->fd_data == NULL)
		return 0;
	printf("Файлы успешно найдены и загружены.\n");
	Line;
	if (!fread(&ptree->buffer.SZ, sizeof(int), 1, ptree->fd_keys))   // Считываем размер буфера
		return 0;
	ptree->buffer.hash = (Item *)calloc(ptree->buffer.SZ, sizeof(Item));
	// Считываем данные из файла, пока программа не считает EOF в k
	int k;
	while (fread(&k, sizeof(int), 1, ptree->fd_keys)) {
		int len;
		fread(&len, sizeof(int), 1, ptree->fd_keys);
		int offset;
		fread(&offset, sizeof(int), 1, ptree->fd_keys);
		int color;
		fread(&color, sizeof(int), 1, ptree->fd_keys);
		int i = I(ptree->buffer.SZ, k);
		ptree->buffer.hash[i].k = k;
		ptree->buffer.hash[i].link = insertfromfile(&ptree->root, k, len, offset, color);
	}
	return 1;
}


/* Загрузка узла с сопутствующими данными в оперативную память непосредственно из файла с ключами */
Node *insertfromfile(Node **proot, int k, int len, int offset, int color) {
	Node *par = &EList;
	while (*proot != &EList) {
		if ((*proot)->key == k)
			return 0;
		par = (*proot);
		proot = k < (*proot)->key ? &(*proot)->left : &(*proot)->right;
	}
	(*proot) = (Node *)malloc(sizeof(Node));
	(*proot)->left = (*proot)->right = &EList;
	(*proot)->parent = par;
	(*proot)->key = k;
	(*proot)->len = len;
	(*proot)->offset = offset;
	(*proot)->color = color;
	return *proot;
}

/* Создание и открытие обоих файлов для записи. */
int create(RB_Tree *ptree, Type fname) {
	printf("Файл (или файл) с указанным именем не найден/ы. Будут созданы новые файлы.\n");
	Line;
	/* fname_data и fname_keys - два дескриптора файлов */
	Type fname_keys = (Type)malloc(strlen(fname) + 5);
	strcpy(fname_keys, fname);
	strcat(fname_keys, ".bin");
	ptree->fd_keys = fopen(fname_keys, "w+b");
	free(fname_keys);
	if (!ptree->fd_keys)
		return 0;
	Type fname_data = (Type)malloc(strlen(fname) + 10);
	strcpy(fname_data, fname);
	strcat(fname_data, "_data.bin");
	ptree->fd_data = fopen(fname_data, "w+b");
	free(fname_data);
	if (!ptree->fd_data)
		return 0;
	printf("Введите размер кэша: --> ");
	int n = getInt(&(ptree->buffer.SZ));
	if (n == 0)
		return 0;
	fwrite(&ptree->buffer.SZ, sizeof(int), 1, ptree->fd_keys);
	ptree->buffer.hash = (Item *)calloc(ptree->buffer.SZ, sizeof(Item));
}


/* Рекурсивное создание файла .dot с деревом. */
void createdot(RB_Tree tree, Type fname) {
	Type name = (Type)malloc(strlen(fname) + 5);
	strcpy(name, fname);
	strcat(name, ".dot");
	FILE *fd_dot = fopen(name, "w+");
	fwrite("digraph G {", sizeof(char), 11, fd_dot);
	inserttofile(tree.root, fd_dot, tree.fd_data);
	fwrite("}", sizeof(char), 1, fd_dot);
	free(name);
	fclose(fd_dot);
	fd_dot = NULL;
}


/* Рекурсивная вставка узла в файл. */
int inserttofile(Node *root, FILE *fd_dot, FILE *fd_data) {
	//Если поддерево не лист
	if (root != &EList) {
		// Заполняем строку с узлом поддерева
		/*------------------------------------------------*/
		Type digit = ItCh(root->key);
		Type info = (Type)malloc(1);
		Type infol = NULL;
		Type infor = NULL;
		*info = '\0';
		Type data = (Type)malloc(root->len);
		fseek(fd_data, root->offset, SEEK_SET);
		fread(data, sizeof(char), root->len, fd_data);
		info = (Type)realloc(info, strlen(digit) + strlen(data) + 6); // '\"', ' ', '-', ' ', '\"' и '\0'
		strcat(info, "\"");
		strcat(info, digit);
		strcat(info, " - ");
		strcat(info, data);
		free(data);
		strcat(info, "\"");
		if (root->color) {
			fwrite(info, sizeof(char), strlen(info), fd_dot);
			fwrite("[color=red, fontcolor=red];", sizeof(char), 27, fd_dot);
		}
		/*------------------------------------------------*/
		info = (Type)realloc(info, strlen(info) + 6); // " -> \"" и '\0'
		strcat(info, " -> ");
		/* Дальше саму строку info не трогаем; создадим infol и infor и закинем в файл эти строки, склеенные с info */
		/*------------------------------------------------*/
		// Для левого потомка:
		if (root->left != &EList) {
			digit = ItCh(root->left->key);
			infol = (Type)malloc(1);
			*infol = '\0';
			data = (Type)malloc(root->left->len);
			fseek(fd_data, root->left->offset, SEEK_SET);
			fread(data, sizeof(char), root->left->len, fd_data);
			infol = (Type)realloc(infol, strlen(digit) + strlen(data) + 6);
			strcat(infol, "\"");
			strcat(infol, digit);
			strcat(infol, " - ");
			strcat(infol, data);
			free(data);
			strcat(infol, "\"");
		}
		// Для правого потомка:
		if (root->right != &EList) {
			digit = ItCh(root->right->key);
			infor = (Type)malloc(1);
			*infor = '\0';
			data = (Type)malloc(root->right->len);
			fseek(fd_data, root->right->offset, SEEK_SET);
			fread(data, sizeof(char), root->right->len, fd_data);
			infor = (Type)realloc(infor, strlen(digit) + strlen(data) + 6);
			strcat(infor, "\"");
			strcat(infor, digit);
			strcat(infor, " - ");
			strcat(infor, data);
			free(data);
			strcat(infor, "\"");
		}
		if (infol) {
			Type res = (Type)malloc(strlen(info) + strlen(infol) + 1);
			*res = '\0';
			strcat(res, info);
			strcat(res, infol);
			fwrite(res, sizeof(char), strlen(res), fd_dot);
			free(res);
			fwrite(";", sizeof(char), 1, fd_dot);
		}
		if (infor) {
			Type res = (Type)malloc(strlen(info) + strlen(infor) + 1);
			*res = '\0';
			strcat(res, info);
			strcat(res, infor);
			fwrite(res, sizeof(char), strlen(res), fd_dot);
			free(res);
			fwrite(";", sizeof(char), 1, fd_dot);
		}
		free(info);
		free(infol);
		free(infor);
		inserttofile(root->left, fd_dot, fd_data);
		inserttofile(root->right, fd_dot, fd_data);
	}
}


/* Рекурсивная выгрузка всегт дерева с ключами в файл (с помощью прямого обхода). */
int savekeys(Node *root, FILE *fd_keys) {
	if (root != &EList) {
		fwrite(&root->key, sizeof(int), 1, fd_keys);
		fwrite(&root->len, sizeof(int), 1, fd_keys);
		fwrite(&root->offset, sizeof(int), 1, fd_keys);
		fwrite(&root->color, sizeof(int), 1, fd_keys);
		savekeys(root->left, fd_keys);
		savekeys(root->right, fd_keys);
	}
	return 1;
}

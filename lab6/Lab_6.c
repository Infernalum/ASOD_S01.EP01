#include "lab_6.h"

#define Line printf("--------------------------------------------------------------------------------------\n");

// Бесконечное расстояние
const double INF = 10000000;

// Массив диагностических сообщений об ошибках
const Type errmsgs[] = { "Ок", "Вершина с соответсвующим именем не найдена", "Найдено идентичное имя или координаты", "Такое ребро уже существует. Поробуйте еще раз.", "Граф пустой" };

// Альтернативы меню для организации диалога
const Type msgs[] = { "0. Выйти", "1. Добавить новую вершину", "2. Добавить новое ребро", "3. Удалить вершину", "4. Вывод графа", "5. Заполнить граф случайными вершинами и ребрами", "6. Применить алгоритм Дейкстры для конкретной вершины", "7. Таймирование алгоритма Дейкстры" };

// Кол-во альтернтатив (для вывода в for)
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);



/**************************************************************************************************************************************/
/* Чет по мелочи */


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


/* Вычисление веса ребра. */
double count_wd(int x1, int y1, int x2, int y2) {
	return (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

/**************************************************************************************************************************************/
/* Диалоговаые функции */


/* Диалоговая функция вставки новой вершины в граф. */
int D_Add_Graph(Graph *pgraph) {
	int rc = 0, x, y;
	Type name = NULL;
	printf("Введите уникальное имя вершины: --> ");
	name = getStr();
	if (!name)
		return 0;
	printf("Введите координату x: --> ");
	int n = getInt(&x);
	if (!n)
		return 0;
	printf("Введите координату y: --> ");
	n = getInt(&y);
	if (!n)
		return 0;
	rc = Insert_Graph(pgraph, name, x, y, -1);
	printf("%s\n", errmsgs[rc]);
	return 1;
}


/* Диалоговая функция вставки нового ребра в граф. */
int D_Add_Edge(Graph *pgraph) {
	int ind_out, ind_in, rc;
	Type name_out = NULL;
	printf("Введите имя вершины, из которой выходит ребро: --> ");
	name_out = getStr();
	if (!name_out)
		return 0;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_out, pgraph->vertex[i].Name)) {
			ind_out = i;			// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1) {
			free(name_out);
			printf("%s", errmsgs[1]);
			return 1;				// Вершина не найдена
		}
	}
	Type name_in = NULL;
	printf("Введите имя вершины, куда идет ребро: --> ");
	name_in = getStr();
	if (!name_in)
		return 0;
	rc = Insert_Edge(pgraph, ind_out, name_in, 1);
	free(name_out);
	printf("%s", errmsgs[rc]);
	return 1;
}


/* Диалоговая функция удаления вершины из графа вместе со всеми ребрами. */
int D_Delete(Graph *pgraph) {
	int rc;
	Line;
	printf("Введите имя удаляемой вершины: --> ");
	Type name = getStr();
	if (!name)
		return 0;
	rc = delete(pgraph, name);
	printf("%s\n", errmsgs[rc]);
	free(name);
	Line;
	return 1;
}


/* Вывод графа (как списки смежности) */
int D_Show(Graph *pgraph) {
	for (int i = 0; i < pgraph->size; ++i) {
		printf("  Из вершины \"%d - %d %s\" идут ребра в: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
		Edge *p = pgraph->vertex[i].root;
		while (p) {
			int ind;
			for (int j = 0; j < pgraph->size; ++j)
				if (!strcmp(p->Name, pgraph->vertex[j].Name)) {
					ind = j;
					break;
				}
			printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
			p = p->next;
		}
		printf("\n");
	}
	return 1;
}


/* Ранжомное создание графа. */
int D_Random(Graph *pgraph) {
	srand(time(NULL));
	int n = 0;
	printf("Введите кол-во вершин: --> ");
	getInt(&n);
	for (int i = 0; i < n; ++i) {
		int x = rand() % (1000 - 1 + 1) + 1;
		int y = rand() % (1000 - 1 + 1) + 1;
		Type name = (Type)malloc(4);
		for (int i = 0; i < 3; ++i)
			name[i] = rand() % (122 - 97 + 1) + 97;
		name[3] = '\0';
		Insert_Graph(pgraph, name, x, y, -1);
	}
	int ed;
	printf("Введите кол-во ребер: --> ");
	getInt(&ed);
	for (int i = 0; i < ed; ++i) {
		int ind_in = rand() % ((pgraph->size - 1) - 0 + 1) + 0;
		int ind_out = rand() % ((pgraph->size - 1) + 1) + 0;
		Insert_Edge(pgraph, ind_out, pgraph->vertex[ind_in].Name, 1);
	}
	return 1;
}


/* Алгоритм Дейкстры для конкретной вершины. */
int D_Dijkstra(Graph *pgraph) {
	int rc;
	Line;
	printf("Введите имя начальной вершины: --> ");
	Type name_out = getStr();
	if (!name_out)
		return 0;
	int ind_out;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_out, pgraph->vertex[i].Name)) {
			ind_out = i;			// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1) {
			free(name_out);
			printf("%s", errmsgs[1]);
			return 1;				// Вершина не найдена
		}
	}
	printf("Введите имя конечной вершины: --> ");
	Type name_in = getStr();
	if (!name_out)
		return 0;
	int ind_in;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_in, pgraph->vertex[i].Name)) {
			ind_in = i;			// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1) {
			free(name_in);
			printf("%s", errmsgs[1]);
			return 1;				// Вершина не найдена
		}
	}
	int *P = (int *)malloc(pgraph->size * sizeof(int));
	/* Вектор кратчайшего пути. */
	for (int i = 0; i < pgraph->size; ++i)
		P[i] = ind_out;
	/* Вектор расстояний. */
	Unit *D = (Unit *)malloc(pgraph->size * sizeof(Unit));
	for (int i = 0; i < pgraph->size; ++i) {
		D[i].color = 0;
		D[i].distance = INF;
	}
	D[ind_out].distance = 0;
	Dijkstra(*pgraph, ind_out, &P, &D);
	if (D[ind_in].distance == INF) {
		printf("Пути из вершины %s в вершину %s не существует. Попробуйте еще раз.", name_out, name_in);
		return 1;
	}
	printf("Кратчайший путть из вершины %s в вершину %s равен %f\nПуть: ", name_out, name_in, D[ind_in].distance);
	int res = ind_in;
	int *result = (int *)malloc(2 * sizeof(int));
	result[0] = 1;
	result[1] = ind_out;
	while (res != ind_out) {
		result = (int *)realloc(result, (++result[0] + 1) * sizeof(int));
		result[result[0]] = res;
		res = P[res];
	}
	if (result[0] == 1)
		printf("%s", pgraph->vertex[result[1]].Name);
	else
		printf("%s --> ", pgraph->vertex[result[1]].Name);
	for (int i = result[0]; i > 1; --i)
		if (i == 2)
			printf("%s.", pgraph->vertex[result[i]].Name);
		else
			printf("%s --> ", pgraph->vertex[result[i]].Name);
	free(P);
	free(D);
	return 1;
}


/* Таймирование алгоритма Дейкстры. */
int D_Timing(Graph *pgraph) {
	Graph *graph = NULL;
	graph = (Graph *)malloc(sizeof(Graph));
	graph->fd = NULL;
	graph->size = 0;
	graph->vertex = NULL;
	int n = 10, cnt = 10000;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		for (int i = 0; i < cnt; ++i) {
			int x = rand() * rand();
			int y = rand() * rand();
			Type name = (Type)(malloc(11));
			for (int j = 0; j < 10; ++j)
				name[j] = rand() % (122 - 97 + 1) + 97;
			name[11] = '\0';
			Insert_Graph(graph, name, x, y, -1);
		}
		for (int i = 0; i < 3 * cnt; ++i) {
			int ind_out = rand() % ((cnt - 1) - 0 + 1) + 0;
			int ind_in = rand() % ((cnt - 1) - 0 + 1) + 0;
			Insert_Edge(graph, ind_out, graph->vertex[ind_in].Name, 1);
		}
		int m = 0;
		first = clock();
		for (int i = 0; i < cnt * (11 - n); ++i) {
			int ind_out = rand() % ((cnt - 1) - 0 + 1) + 0;
			int ind_in = rand() % ((cnt - 1) - 0 + 1) + 0;
			int *P = (int *)malloc(graph->size * sizeof(int));
			/* Вектор кратчайшего пути. */
			for (int i = 0; i < pgraph->size; ++i)
				P[i] = ind_out;
			/* Вектор расстояний. */
			Unit *D = (Unit *)malloc(graph->size * sizeof(Unit));
			for (int i = 0; i < graph->size; ++i) {
				D[i].color = 0;
				D[i].distance = INF;
			}
			D[ind_out].distance = 0;
			Dijkstra(*graph, ind_out, &P, &D);
			if (!(D[ind_in].distance == INF))
				++m;
			free(P);
			free(D);
		}
		last = clock();
		printf("%d pathes were found\n", m);
		printf("test #%d, number of vertices = %d, count of finds = %d, time = %d\n", 10 - n, (11 - n)*cnt, cnt * (11 - n), (last - first));
	}
	delGraph(graph);
	return 1;
}


/**************************************************************************************************************************************/
/* Функции для работы с графом */

/* Функция вставки новой вершины в граф. */
int Insert_Graph(Graph *pgraph, Type name, int x, int y, int flag) {
	if (flag == -1) {
		for (int i = 0; i < pgraph->size; ++i) {
			if ((!strcmp(name, pgraph->vertex[i].Name)) || ((x == pgraph->vertex[i].x) && (y == pgraph->vertex[i].y)))
				return 2;								// Найдена вершина с таким же именем или такими же координатами
		}
		++(pgraph->size);
		pgraph->vertex = (Vertex *)realloc(pgraph->vertex, sizeof(Vertex) * pgraph->size);
		pgraph->vertex[pgraph->size - 1].Name = name;
		pgraph->vertex[pgraph->size - 1].x = x;
		pgraph->vertex[pgraph->size - 1].y = y;
		pgraph->vertex[pgraph->size - 1].size_adj = 0;
		pgraph->vertex[pgraph->size - 1].root = NULL;
	}
	else {
		pgraph->vertex = (Vertex *)realloc(pgraph->vertex, sizeof(Vertex) * pgraph->size);
		pgraph->vertex[flag].Name = name;
		pgraph->vertex[flag].x = x;
		pgraph->vertex[flag].y = y;
		pgraph->vertex[flag].size_adj = 0;
		pgraph->vertex[flag].root = NULL;
	}
	return 0;
}


/* Функция вставки нового ребра в граф. */
int Insert_Edge(Graph *pgraph, int ind_out, Type name_in, int flag) {
	int ind_in;
	if (!pgraph->size)
		return 4;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_in, pgraph->vertex[i].Name)) {
			ind_in = i;				// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;				// Вершина не найдена
	}
	Edge *pF = pgraph->vertex[ind_out].root;
	while (pF) {
		if (!strcmp(name_in, pF->Name))
			return 3;				// Такое ребро уже есть
		if (!pF->next)
			break;
		pF = pF->next;
	}
	Edge *tmp = (Edge *)malloc(sizeof(Edge));
	tmp->Name = name_in;
	tmp->wd = count_wd(pgraph->vertex[ind_in].x, pgraph->vertex[ind_in].y, pgraph->vertex[ind_out].x, pgraph->vertex[ind_out].y);
	tmp->next = NULL;
	if (!pF)
		pgraph->vertex[ind_out].root = tmp;
	else
		pF->next = tmp;
	if (flag)
		pgraph->vertex[ind_out].size_adj++;
	return 0;
}


/* Удаление вершины вместе со всеми ребрами. */
int delete(Graph *pgraph, Type name) {
	int ind_del;
	if (!pgraph->size)
		return 4;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name, pgraph->vertex[i].Name)) {
			ind_del = i;				// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;				// Вершина не найдена
	}
	for (int i = 0; i < pgraph->size; ++i) {
		Edge *tmp = pgraph->vertex[i].root;
		while (tmp) {
			if (!strcmp(tmp->Name, name)) {
				tmp = del_edge(&pgraph->vertex[i].root, tmp);
				--(pgraph->vertex[i].size_adj);
			}
			else
				tmp = tmp->next;
		}
	}
	Edge *tmp = pgraph->vertex[ind_del].root;
	while (tmp) {
		del_edge(&pgraph->vertex[ind_del].root, tmp);
		tmp = pgraph->vertex[ind_del].root;
	}
	for (int i = ind_del; i < pgraph->size - 1; ++i)
		pgraph->vertex[i] = pgraph->vertex[i + 1];
	--(pgraph->size);
	pgraph->vertex = (Vertex *)realloc(pgraph->vertex, sizeof(Vertex) * pgraph->size);
	return 0;
}


/* Удаление узла списка. */
Edge *del_edge(Edge **root, Edge *target) {
	if (!(*root)->next) {
		free(*root);
		(*root) = NULL;
		return (*root);
	}
	if ((*root) == target) {
		(*root) = (*root)->next;
		free(target);
		return (*root);
	}
	Edge *tmp = (*root);
	while (tmp->next != target)
		tmp = tmp->next;
	tmp->next = target->next;
	free(target);
	return tmp;
}



/* Рекурсивный алгоритм Дейкстры. */
void Dijkstra(Graph graph, int ind_out, int **P, Unit **D) {
	int ind_in;
	Edge *p = graph.vertex[ind_out].root;
	while (p) {
		for (int i = 0; i < graph.size; ++i)
			if (!strcmp(p->Name, graph.vertex[i].Name)) {
				ind_in = i;
				break;
			}
		double weight = (*D)[ind_out].distance + p->wd;
		if ((weight <= (*D)[ind_in].distance) && !((*D)[ind_in].color)) {
			(*P)[ind_in] = ind_out;
			(*D)[ind_in].distance = weight;
		}
		p = p->next;
	}
	(*D)[ind_out].color = 1;
	double min_dist = INF;
	int min_ind = -1;
	for (int i = 0; i < graph.size; ++i)
		if (((*D)[i].distance <= min_dist) && !((*D)[i].color) && ((*D)[i].distance != INF)) {
			min_ind = i;
			min_dist = (*D)[i].distance;
		}
	if (min_ind != -1)
		Dijkstra(graph, min_ind, P, D);

}



/*Удаление графа из оперативной памяти. */
void delGraph(Graph *pgraph) {
	for (int i = 0; i < pgraph->size; ++i) {
		Edge *p = pgraph->vertex[i].root;
		while (p)
			p = del_edge(&(pgraph->vertex[i].root), p);
	}
	free(pgraph->vertex);
}


/**************************************************************************************************************************************/
/* Функции для работы с файлом */


/* Функция загрузки таблицы из файла */
int D_Load(Graph *pgraph, Type *fname) {
	Line;
	printf("Введите имя файла (без типа файла .bin): --> ");
	*fname = getStr();
	if (!fname)
		return 0;
	if (!load(pgraph, *fname))
		if (!create(pgraph, *fname)) {
			printf("Возникла непредвиденная ошибка при моздании файлов. Попробуйте еще раз.");
			return 0;
		}
	return 1;
}


/* Функции загрузки существующего файла (если таковой имеется). */
int load(Graph *pgraph, Type name) {
	/* Открываем существующие файлы на чтение и запись; в случае не нахождеия одного из нужных файлов, выдется сообщение об ошибке */
	Type fname = (Type)malloc(strlen(name) + 5);
	strcpy(fname, name);
	strcat(fname, ".bin");
	pgraph->fd = fopen(fname, "r+b");
	free(fname);
	if (pgraph->fd == NULL)
		return 0;
	if (!fread(&pgraph->size, sizeof(int), 1, pgraph->fd))   // Считываем кол-во вершин
		return 0;
	for (int i = 0; i < pgraph->size; ++i) {				// Для каждой вершины считываем ее имя, координаты и кол-во исходящих ребер
		Type Name = (Type)malloc(1);
		*Name = '\0';
		char p;
		while (p = fgetc(pgraph->fd)) {						// Пока не считали нуль-байт
			int k = strlen(Name);
			Name = (Type)realloc(Name, k + 2);
			Name[k] = p;
			Name[k + 1] = '\0';
		}
		Name = (Type)realloc(Name, strlen(Name) + 1);
		int x;
		fread(&x, sizeof(int), 1, pgraph->fd);
		int y;
		fread(&y, sizeof(int), 1, pgraph->fd);
		Insert_Graph(pgraph, Name, x, y, i);
		fread(&pgraph->vertex[i].size_adj, sizeof(int), 1, pgraph->fd);  // Считываем размер списка связности текущей вершины
	}
	// Считываем имена для списков связности
	for (int i = 0; i < pgraph->size; ++i)
		for (int j = 0; j < pgraph->vertex[i].size_adj; ++j) {
			Type Name = (Type)malloc(1);
			*Name = '\0';
			char p;
			while ((p = fgetc(pgraph->fd)) && !feof(pgraph->fd)) {					// Пока не считали нуль-байт
				int k = strlen(Name);
				Name = (Type)realloc(Name, k + 2);
				Name[k] = p;
				Name[k + 1] = '\0';
			}
			Name = (Type)realloc(Name, strlen(Name) + 1);
			Insert_Edge(pgraph, i, Name, 0);
		}
	printf("Файл успешно найден и загружен.\n");
	Line;
	return 1;
}


/* Создание и открытие файла для записи. */
int create(Graph *pgraph, Type name) {
	printf("Файл с указанным именем не найден. Будет создан новый файл.\n");
	Line;
	Type fname = (Type)malloc(strlen(name) + 5);
	strcpy(fname, name);
	strcat(fname, ".bin");
	pgraph->fd = fopen(fname, "w+b");
	free(fname);
	if (!pgraph->fd)
		return 0;
	return 1;
}


/* Функция сохранения графа в файл. */
int save(Graph graph, Type name) {
	Type fname = (Type)malloc(strlen(name) + 5);
	strcpy(fname, name);
	strcat(fname, ".bin");
	graph.fd = fopen(fname, "r+b");
	fwrite(&graph.size, sizeof(int), 1, graph.fd);
	/* Для каждой  вершины записываем имя, ее коор-ты и кол-во эл-тов в списке смежности. */
	for (int i = 0; i < graph.size; ++i) {
		fwrite(graph.vertex[i].Name, sizeof(char), strlen(graph.vertex[i].Name) + 1, graph.fd);
		fwrite(&graph.vertex[i].x, sizeof(int), 1, graph.fd);
		fwrite(&graph.vertex[i].y, sizeof(int), 1, graph.fd);
		fwrite(&graph.vertex[i].size_adj, sizeof(int), 1, graph.fd);
	}
	/* Записываем данные в списки смежности. */
	for (int i = 0; i < graph.size; ++i) {
		Edge *tmp = graph.vertex[i].root;
		for (int j = 0; j < graph.vertex[i].size_adj; ++j) {
			fwrite(tmp->Name, sizeof(char), strlen(tmp->Name) + 1, graph.fd);
			tmp = tmp->next;
		}
		free(tmp);
	}
	return 1;
}


/* Создание .dot файла. */
void createdot(Graph graph, Type fname) {
	Type name = (Type)malloc(strlen(fname) + 5);
	strcpy(name, fname);
	strcat(name, ".dot");
	FILE *fd_dot = fopen(name, "w+");
	fprintf(fd_dot, "digraph G {node [shape=\"circle\"];rankdir=\"LR\";");
	for (int i = 0; i < graph.size; ++i) {
		if (!graph.vertex[i].root) {
			fprintf(fd_dot, "\"%d - %d %s\";", graph.vertex[i].x, graph.vertex[i].y, graph.vertex[i].Name);
			continue;
		}
		Edge *tmp = graph.vertex[i].root;
		int ind_in;
		while (tmp) {
			for (int j = 0; j < graph.size; ++j) {
				if (!strcmp(tmp->Name, graph.vertex[j].Name)) {
					ind_in = j;				// Нашли нужную вершину
					break;
				}
			}
			fprintf(fd_dot, "\"%d - %d %s\" -> \"%d - %d %s\"[label=\"%.2f\"];", graph.vertex[i].x, graph.vertex[i].y, graph.vertex[i].Name, graph.vertex[ind_in].x, graph.vertex[ind_in].y, graph.vertex[ind_in].Name, tmp->wd);
			tmp = tmp->next;
		}
	}
	fprintf(fd_dot, "}");
	free(name);
	fclose(fd_dot);
	fd_dot = NULL;
}

/**************************************************************************************************************************************/


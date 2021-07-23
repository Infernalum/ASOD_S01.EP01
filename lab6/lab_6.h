#ifndef HEADER_6_H
#define HEADER_6_H

#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char* Type;

/* Структура ребра (эл-та списка смежности). */
typedef struct Edge {
  Type Name;  // Индекс вершины (расположение в массиве)
  double wd;          // Вес ребра
  struct Edge* next;  // Указатель на следующее ребро в списке смежности
} Edge;

/* Структура вершины (эл-т массива). */
typedef struct Vertex {
  Type Name;     // Уникальное имя вершины
  int x;         // Координата x
  int y;         // Координата y
  int size_adj;  // Текущий размер списка смежности (нужно для записи в файл)
  Edge* root;  // Указатель на начало списка смежности
} Vertex;

/* Структура непосредственно самого графа. */
typedef struct Graph {
  int size;        // Текущее кол-во вершин
  Vertex* vertex;  // Массив вершин
  FILE* fd;        // Дескриптор файла
} Graph;

/* Структура для алгоритма Дейкстры. */
typedef struct Unit {
  int color;  // Посещенность вершины 0 - если не была посещена; 1 - если обход
              // закончен
  double distance;  // Кратчайшее расстояние до данной вершины
} Unit;

/*******************************************************************************************************************************************************************/
/* Прототипы функций */

/* Чет по мелочи */
int getInt(int*);  // Ввод целого чмсла из входного потока
Type getStr();  // Ввод строки произвольной длины из входного потока
void clean_stdin();  // Очистка входного потока от мусора
int dialog(const Type msgs[], int);   // Диалог
double count_wd(int, int, int, int);  // Вычисление веса ребра

/**************************************************************************************************************************************/
/* Диалоговаые функции */

int D_Add_Graph(Graph*);  // Вставка новой вершины в граф
int D_Add_Edge(Graph*);   // Вставка нового ребра в граф
int D_Delete(Graph*);     // Удаление вершины из графа
int D_Show(Graph*);  // Вывод графа (в виде списка смежности)
int D_Random(Graph*);    // Рандомное создание графа
int D_Dijkstra(Graph*);  // Алгоритм Дейкстры
int D_Timing(Graph*);  // Таймирование алгоритма Дейкстры

/* Функции для работы с графом */

int Insert_Graph(Graph*,
                 Type,
                 int,
                 int,
                 int);  // Функция вставки новой вершины в дерево
int Insert_Edge(Graph*, int, Type, int);  // Функция вставки нового ребра в граф
int delete (Graph*, Type);      // Удаление вершиы
Edge* del_edge(Edge**, Edge*);  // Удвление ребра
void delGraph(Graph*);  // Удаление графа из оперативной памяти
void Dijkstra(Graph, int, int**, Unit**);  // Рекурсивный алгоритм

/* Функции для работы с файлом */

int D_Load(Graph*, Type*);  // Функция выбора принципа работы с файлом (создание
                            // или загрузка старого)
int load(Graph*, Type);  // Функция загрузки графа из файла
int create(Graph*, Type);  // Функция создания нового файла
int save(Graph, Type);  // Функция сохранения графа в файл
void createdot(Graph, Type);  // Создание .dot файла

/*******************************************************************************************************************************************************************/

#endif  // HEADER_6_H

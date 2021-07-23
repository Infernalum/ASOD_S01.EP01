#ifndef HEADER_5_H
#define HEADER_5_H

#define _CRT_SECURE_NO_WARNINGS
#define RED 1
#define BLACK 0

#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char* Type;

/* Структура узла */
typedef struct Node {
  int color;  // Цвет узла (1 - красный; 0 - черный)
  int key;    // Ключ (Да ладнаа)
  int offset;  // Смещение информации в файле fname_data.bin
  int len;  // Длина информации
  struct Node *left, *right;  // Указатели на левое и правое поддеревья
  struct Node* parent;  // Указатель на родительскую вершину
} Node;

extern Node EList;

/* Элемент кэш буфера */
typedef struct Item {
  int k;       // Ключ
  Node* link;  // Связь с RB-деревом
} Item;

/* Структура кэш буфера */
typedef struct Buffer {
  int SZ;      // Размер кэш буфера
  Item* hash;  // Сам буфер
} Buffer;

/* Структура дерева, сохраняющая указатель на родительский узел всего дерева и
 * дескрипторы для обоих файлов (чтобы не таскать их за собой по проге). */
typedef struct RB_Tree {
  FILE* fd_keys;  // Дескриптор файла с ключами
  FILE* fd_data;  // Дескриптор файла с информацией
  Node* root;     // Указатель на дерево
  Buffer buffer;  // Кэш буфер
} RB_Tree;

/*******************************************************************************************************************************************************************/
/* Прототипы функций */

/* Чет по мелочи */
int getInt(int*);  // Ввод целого чмсла из входного потока
Type getStr();  // Ввод строки произвольной длины из входного потока
void clean_stdin();  // Очистка входного потока от мусора
int dialog(const Type msgs[], int);  // Диалог
int I(int, int);                     // Хэш функция
Type ItCh(int);  // Конвертер инта в строку

/* Диалоговые функции (юзается второе косвенное общение, чтобы изменять
 * указатель на узел дерева) */
int D_Add(RB_Tree*);  // Диалоговая функция включения в дерево и в кэш буфер
int D_Find(RB_Tree*);  // Диалоговая функция поиска нужного узла
int D_Delete(
    RB_Tree*);  // Диалоговая функция удаления узла всего дерева по ключу
int D_Show(RB_Tree*);  // Диалоговая функция вывода всего дерева в порядке
                       // слежования ключей и кэш буфера
int D_Random(
    RB_Tree*);  // Диалоговая функуия заполнения дерева случайными ключами
int D_Timing(RB_Tree*);  // Таймирование дерева

/* Функции для работы с деревом */
int insert(RB_Tree*, int, int, int);     // Вставка в кэш буфер
Node* RB_Insert(Node**, int, int, int);  // Вставка узла в дерево
/* Повороты */
void right_rotate(Node**, Node*);  // Правый поворот
void left_rotate(Node**, Node*);   // Левый поворот
/************/
void RB_Insert_Fixup(Node**, Node*);  // Балансировка дерева при добавлении узла
int RB_Remove(Node**, int);  // Удаление узла из дерева (и из кеша тоже)
void RB_Remove_Fixup(
    Node**,
    Node*);  // Балансировка дерева после удаления узла как из бинарного
int find(RB_Tree, int, int);  // Нахождение ключа в дереве
Node* RB_Find(Node*, int);    // Поиск узла в дереве
void RB_Preorder(Node*);  // Вывод дерева в прямом порядке следования ключей
void deltree(Node**);  // Удаление всего дерева и кэш буфера

Node* insertfromfile(Node**, int, int, int, int);

/* Функции для работы с файлом данных */
int D_Load(RB_Tree*, Type*);  // Диалоговая функция загрузки дерева из файла
int load(RB_Tree*,
         Type);  // Функция загрузки существующих файлов (если таковые имеются)
int create(RB_Tree*, Type);  // Создание обоих файлов для записи инфы и ключей
int savekeys(Node*, FILE*);  // Рекурсивное сохранение всех ключей и
                             // сопровождающих их данных в файл с ключами
void createdot(
    RB_Tree,
    Type);  // Рекурсивное создание .dot файла для сохранения дерева (структура
            // дерева может поменяться из-за сделанных поворотов)
int inserttofole(Node*, FILE*, FILE*);  // Рекурсивная

/*******************************************************************************************************************************************************************/

#endif  // HEADER_5_H

// #ifndef - макрос препроцессора для исключения двойного включения
#ifndef LAB3_HEADER_H
#define LAB3_HEADER_H
// Прототипы функций
char* getstr();  // Считывание строки через построчный буферизированный ввод
int inputCheck(char* s);  // Проверка ввода строки на посторонние символы
char* skipSpace(char* s);  // Пропуск лидирующих пробелов и/или знаков табуляции
char* skipWord(
    char* s);  // Пропуск символов до первого пробела и/или знака табуляции
char* copyWord(char* from, char* to);  // Копирование слова
char* reorg(char* s);  // Формирование результирующей строки
#endif

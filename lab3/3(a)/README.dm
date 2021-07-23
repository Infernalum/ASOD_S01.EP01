 Лаба 4(а).
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
	Задание 6.
	Вариант 1.
	Написать программу для работы по запросам оператора с ориентированным взыешенным графом. Каждая вершина графа имеет свое уникальное имя и задается координатами плоскости (х, у).
	Граф задается списками смежностей. Вес ребра графа определяется расстоянием между соответсвующеми вершинами графа.
 Предусмотреть следующие операции:
 - Добавление новой вершины в граф;
 - Добавления в граф нового ребра между двумя заданными вершинами, с необходимым вычислением веса ребра;
 - Удаление заданной вершины из графа; при этом удаляются все ребра, связанные с удаляемой вершиной;
 - Поиск кратчайшего пути между двумя вершинами графа, используя алгоритм Дейкстры;
 - Вывод графа;
	<br/>Примечания:
1. Программа должна содержать несколько функций; функция main() должна выполнять: вывод меню, ввод и анализ ответа, вызов на исполнение требуемой функции.
2. Программа должна генерировать граф случайным образом.
3. Граф должен храниться в файле; в начале сеанса работы он хагружается из файла; если в процессе работы граф был изменен, новое состояние должно быть сохранено в файле.
4. В программе нужно предусмотреть проверку правильности ввода данных.
5. Оценить сложность реализованных алгоритмов.
6. Провести таймирование (или профилирование) алгоритма поиска кратчайшего пути.
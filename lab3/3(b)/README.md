  Лаба 3(б).
  Вариант 25.
		<br/> Из входного потока вводится произвольное число строк, состоящие из последовательности двоичных многоразрядных чисел, которые могут быть разделены одним или несколькими и/или знаками табуляции. Длина каждой строки произвольна. Конец ввода определяется концом файла. 
		<br/> При обработке строки сформировать новую строку, поместив в нее те же числа, записанные в шестнадцатиричном виде (упаковка справа налево). Числа в обработанной строке должны быть разделены только одним пробелом. Полученную строку вывести на экран.
		<br/> Примечания:
   1. Каждая строка представлена списком; первое поле элемента списка - символ строки; второе - указатель на следующий элемент списка или NULL в конце списка; список результата формировать,
модифицируя исходный список.
   2. Ввод должен быть организован с помощью функции scanf() со спецификациями для ввода строк. Функцию getchar() или спецификацию %c в scanf() не использовать!
   3. Логически законченные части алгоритма решения задачи должны быть оформлены отдельными функциями с параметрами
(например, пропуск лидирующих разделителей, создание списка, вывод списка и др.)
   4. Целесообразно обработку строк оформить соответствующей функцией, тогда в функции main должны быть ввод  строки, вызов функции обработки,
вывод результата - в цикле, пока не закончится ввод; конец цикла - по достижении конца файла.
   5. Все функции должны работать со списком; при вводе строки следует сразу создать список.

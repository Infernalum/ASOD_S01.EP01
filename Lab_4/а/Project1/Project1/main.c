#include "Header_4a.h"

extern const Type msgs[];
extern const int NMsgs;
extern const Type errmsgs[];


// Массив указателей на функции - для реализации выбора функции; порядок перечисления функций в списке инициализации должен соответсвовать порядку указания альтернатив в списке альернатив
int(*fptr[])(Table *) = { NULL, D_Add, D_Find, D_Delete, D_Show };


int main() {
	setlocale(LC_ALL, "Russian");
	Table table = { NULL , NULL };
	int rc;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&table))         // Вызывается диалоговая функция, соответствующая номеру альтернативы
			break;
	printf("Спасибо за внимание!\n");
	return 0;
}

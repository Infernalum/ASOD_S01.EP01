#include "Header_4a.h"

extern const Type msgs[];
extern const int NMsgs;
extern const Type errmsgs[];


// ������ ���������� �� ������� - ��� ���������� ������ �������; ������� ������������ ������� � ������ ������������� ������ �������������� ������� �������� ����������� � ������ ����������
int(*fptr[])(Table *) = { NULL, D_Add, D_Find, D_Delete, D_Show };


int main() {
	setlocale(LC_ALL, "Russian");
	Table table = { NULL , NULL };
	int rc;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&table))         // ���������� ���������� �������, ��������������� ������ ������������
			break;
	printf("������� �� ��������!\n");
	return 0;
}

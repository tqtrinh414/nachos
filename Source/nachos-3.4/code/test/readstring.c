#include "syscall.h"

int
main()
{	
	char buf[21];
	PrintString("Nhap mot chuoi (toi da la 20 ky tu): ");
	ReadString(buf, 20);
	PrintString("Chuoi ban vua nhap ne: ");
	PrintString(buf);
	PrintChar('\n');
	Exit(0);
}

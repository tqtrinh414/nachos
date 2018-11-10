#include "syscall.h"
int main()
{	
	char buf[20];
	PrintString("Nhap file name (toi da 20 ky tu): ");
	ReadString(buf, 20);
	if (CreateFile(buf) == 0) {
		PrintString("Da tao file: ");
		PrintString(buf);
		PrintString("\n");
	} else
		PrintString("Ko tao duoc, bo tay.com, ahihi!\n");
	
	Exit(0);
}

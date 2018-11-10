#include "syscall.h"
#define MAX 20
int main()
{	
	char buf[MAX+1];
	char filename[MAX+1];
	OpenFileId id;
	int stat;
	PrintString("Nhap ten file (vi du: da2.source): ");
	Read(filename, MAX, 0);
	
	id = Open(filename, 1);//read only
	if (id != -1) {
		PrintString("Noi dung cua file: ");
		PrintString(filename);
		PrintString(" id=");
		PrintInt(id);
		PrintString(" la: \n");
		do {
			stat = Read(buf, MAX, id);
			if (stat <= 0)
				break;
			PrintString(buf);
			buf[0] = '\0';
		} while (1);
		PrintChar('\n');
		CloseFile(id);
	} else
		PrintString("Ko mo duoc file! File ko ton tai!");
	Exit(0);
}

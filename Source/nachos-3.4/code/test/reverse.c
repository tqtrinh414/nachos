#include "syscall.h"
#define MAX 20
int main()
{	
	char buf[1];
	char filenameSource[MAX+1], filenameDest[MAX+1];
	OpenFileId ids, idd;//id source, id dest
	int stat, pos, l1, l2, i;
	int stop;
	PrintString("Nhap ten file nguon (vi du: da2.source): ");
	l1 = Read(filenameSource, MAX, 0);
	PrintString("Nhap ten file dich (vi du: da2.reverse): ");
	l2 = Read(filenameDest, MAX, 0);
	ids = Open(filenameSource, 1);//read only
	
	if (l1 == 0 || l2 == 0) {
		PrintString("Chua nhap dest hoac source\n");
	} else if (l1 == l2) {
		stop = 1;
		for (i = 0; i <= l2; i++)
			if (filenameSource[i] != filenameDest[i]) {
				stop = 0;
				break;
			}
		if (stop == 1) {
			PrintString("Co dua bon e ha :P, kieu nay thi bon em thua :3\n");
			Halt();
		}
	}
	
	if (ids != -1) {
		PrintString("Dang tien hanh copy file do ban hien.\n");
		stat = CreateFile(filenameDest);
		if (stat == -1) {
			PrintString("Ko tao file moi duoc");
			Halt();
		}
		idd = Open(filenameDest, 0);
		pos = Seek(-1, ids);
		buf[1] = '\0'; //buf == string with 1 letter
		while (pos > 0) {
			pos--;//bỏ đi ký tự \0 cuối muỗi file, tức là đầu file mới đó ahihi
			Seek(pos, ids);
			Read(buf, 1, ids);
			Write(buf, 1, idd);
		}
		PrintString("Da reverse xong.\n");
		CloseFile(idd);CloseFile(ids);
	} else
		PrintString("Ko mo duoc file! File ko ton tai!");
	Exit(0);
}

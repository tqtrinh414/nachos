#include "syscall.h"
#define MAX 100
int main()
{	
	char buf[MAX+1];
	while(1) {
		int length = Read(buf, MAX, 0);
		if (length > 0)
			Write(buf, length, 1);
		else break;
	}
	Exit(0);
}

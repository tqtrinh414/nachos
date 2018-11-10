#include "syscall.h"
int main()
{	
char c;
	int i=-127;
	while(i<128)
	{
		c=i++;
		PrintChar(c);
		PrintString("\n");
	}
	Exit(0);
}

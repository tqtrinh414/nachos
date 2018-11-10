#include "syscall.h"

int
main()
{	
	int i;
	do {
		i = ReadInt();
		PrintInt(i);
		PrintChar('\n');
	} while (i > 0);//read until error
	Exit(0);
}

#include "syscall.h"

int
main()
{	
	char ch;
	do {
		ch = ReadChar();
		PrintChar(ch);
		PrintChar('\n');
	} while (ch != '!'); //read until pressing "!" key
	Exit(0);
}

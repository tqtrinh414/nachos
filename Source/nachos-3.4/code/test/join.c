#include "syscall.h"

int
main()
{
    Join(Exec("./exit2"));
	Exit(0);
}
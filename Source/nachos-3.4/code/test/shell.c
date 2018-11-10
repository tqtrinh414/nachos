#include "syscall.h"

int
main()
{
    SpaceId newProc;
	OpenFileId id;//read only
    char buffer[BUFFER_LENGTH];
    int i, exitCode, length, background;
	

    while( 1 )
    {
		background = 0;
		PrintString(">>");
		buffer[0] = 0;
		length = 0;
		ReadString(buffer, BUFFER_LENGTH - 1);
		for (length = 0; buffer[length] != 0; length++);
		if (length == 0 || buffer[0] == '.' || buffer[0] == '/') {
			PrintString("Lam on nhap lai a!\n");
			continue;
		}
		if (length > 0 && buffer[0] == '&') {
			length = length - 1;
			for (i = 0; i <= length; i++) {
				buffer[i] = buffer[i + 1];
			}
			background = 1;
		}
		if (length == 4 && buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
			break;
		id = Open(buffer, 1);//read only
		if (id == -1) {
			PrintString("File ko ton tai Co oi, hjx!\n");
			PrintString(buffer);
			PrintChar('\n');
			continue;
		}
		CloseFile(id);
		newProc = Exec(buffer);
		if (newProc > -1 && background == 0) {
			exitCode = Join(newProc);
			//PrintString("Exit code: ");
			//PrintInt(exitCode);
			//PrintChar('\n');
		}
		
    }
	PrintString("Done!");
	Exit(0);
}


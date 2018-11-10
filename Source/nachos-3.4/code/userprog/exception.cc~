// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"


void increasePC() {
	int programCounter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, programCounter);
	programCounter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, programCounter);
	machine->WriteRegister(NextPCReg, programCounter + 4);
}

int convertStringToInteger(char*a)
{
	int n = strlen(a);
	int minusCount = 0; 
	for (int i = 0; i < n; i++)
		if (!(('0' <= a[i] && a[i] <= '9') || (a[i] == '-' && minusCount++ == 0))) return 0;
 
	int q = atoi(a);
	return q;
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
	int data, i, arg2, arg1, arg3, arg4, tmp;
	char *buf = new char[BUFFER_LENGTH];
	buf[0] = '\0';
	static char bufStatic[BUFFER_LENGTH] = "";
	static int counter = 0;
    if (which == SyscallException/* && (type == SC_Halt)*/) {
		switch (type) {
			case SC_Halt:
				DEBUG('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt();
				break;
			case SC_ReadInt:
				if (gSynchConsole->Read(buf, BUFFER_LENGTH - 1) != -1) {
					data = convertStringToInteger(buf);
					machine->WriteRegister(2, data);
				} else 
					machine->WriteRegister(2, 0);				
				
				break;
			case SC_PrintInt:
				data = machine->ReadRegister(4);
				sprintf(buf, "%d", data);
				gSynchConsole->Write(buf, strlen(buf));
				
				break;
			case SC_ReadChar:
				//Using static char to store buffer after system call
				
				//can't print -> read
				if (counter == strlen(bufStatic)) {
					gSynchConsole->Read(buf, BUFFER_LENGTH - 1);
					strcpy(bufStatic, buf);
					counter = 0;
				}			

				//can print -> print
				if (counter < strlen(bufStatic))
					machine->WriteRegister(2, bufStatic[counter++]);
				
				break;
			case SC_PrintChar:
				data = machine->ReadRegister(4);
				sprintf(buf, "%c", char(data));
				gSynchConsole->Write(buf, 1);
				
				break;
			case SC_ReadString:
				//read arg (bufID, len)
				arg1 = machine->ReadRegister(4);//buf 
				arg2 = machine->ReadRegister(5);//length
				i = 0;
				while (i < arg2) {		//max length
					tmp = min(BUFFER_LENGTH - 1, arg2 - i);		//length by reading
					data = gSynchConsole->Read(buf, tmp);
						
					machine->System2User(arg1, tmp, buf);
					i += tmp;			//now we read i					
					arg1 += tmp;			//increate virtual address will be stored
					
					if (data < tmp)		//pressed Enter
						break;
				}
				
				break;
			case SC_PrintString:
				//read arg
				arg1 = machine->ReadRegister(4);//buf
				char *buffer;	//because user2system will make a new space!
				while ((buffer = machine->User2System(arg1, BUFFER_LENGTH - 1)) != NULL) {
					tmp = strlen(buffer);// the length of string;
					if (tmp == 0)	//empty string
						break;
					gSynchConsole->Write(buffer, tmp);
					arg1 += tmp;
					delete []buffer;
				}
				
				break;
			case SC_CreateFile:
				arg1 = machine->ReadRegister(4);//buf
				buf = machine->User2System(arg1, BUFFER_LENGTH - 1); // chep ten file tu bo nho len kernel
				// create a new file 
				machine->WriteRegister(2, fileSystem->Create(buf, 0) ? 0 : -1); // create file thanh cong return 0, that bai return -1
				
				break;
			case SC_Open:
				arg1 = machine->ReadRegister(4);//buf
				arg2 = machine->ReadRegister(5);//type
				
				if (fileSystem->index >= 10) // Neu dang mo 10 file -> khong the mo nua -> return -1
					machine->WriteRegister(2, -1);
				else {
					buf = machine->User2System(arg1, BUFFER_LENGTH - 1); // lay chuoi tu bo nho truyen vao kernel
					if (strcmp(buf,"stdin") == 0)	// buf == stdin -> return vi tri file
						machine->WriteRegister(2, 0);//0 == in
					else if (strcmp(buf,"stdout") == 0) // buf == stdout -> return vi tri file
						machine->WriteRegister(2, 1);//1 == out
					else
						if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(buf, arg2)) != NULL)
							machine->WriteRegister(2, fileSystem->index++); // return vi tri file vua mo va tang index
						else
							machine->WriteRegister(2, -1);
				}
				
				break;
			case SC_CloseFile:
				arg1 = machine->ReadRegister(4);//index
				if (arg1 < 0 || arg1 >= 10 || fileSystem->openf[arg1] == NULL)
					machine->WriteRegister(2, -1);
				else {
					delete fileSystem->openf[arg1];
					fileSystem->openf[arg1] = NULL;
					machine->WriteRegister(2, 0);
				}
				
				break;
			case SC_Read: {
				arg1 = machine->ReadRegister(4);//buf
				arg2 = machine->ReadRegister(5);//numBuf
				arg3 = machine->ReadRegister(6);//index
				int OldPos;
				int NewPos;
				char *buf = new char[arg2];
				int i = 0;
				// kiểm tra tham số thứ tự
				if (arg3 < 0 || arg3 >= 10 || fileSystem->openf[arg3] == NULL) // kiem tra file co ton tai hay dang mo hay khong?
					machine->WriteRegister(2, -1);
				else {
					OldPos = fileSystem->openf[arg3]->GetCurrentPos(); //Lay vi tri cua file dang mo 
					buf = machine->User2System(arg1, arg2); // lay chuoi tu bo nho truyen vao kernel
					int type = fileSystem->openf[arg3]->type; // kiem tra xem co quyen doc, viet
					if (type == 2)
					{
						int sz = gSynchConsole->Read(buf, arg2);
						machine->System2User(arg1, sz, buf);
						machine->WriteRegister(2, sz);
					} else if (type == 0 || type == 1) {
						int realBytes = fileSystem->openf[arg3]->Read(buf, arg2);
						
						if ( realBytes > 0)
						{
							// kernal => user space
							NewPos = fileSystem->openf[arg3]->GetCurrentPos();
							machine->System2User(arg1, realBytes + 1, buf);//\0
							machine->WriteRegister(2, realBytes);//\0
						} else 
							machine->WriteRegister(2, -2);
					} else
						machine->WriteRegister(2, -1);
				}
				
				delete[] buf;
				
				break;
			}
				
			case SC_Write:
			{
				arg1 = machine->ReadRegister(4);//buf
				arg2 = machine->ReadRegister(5);//numBuf
				arg3 = machine->ReadRegister(6);//index
				int OldPos;
				int NewPos;
				char *buf = new char[arg2];
				// kiểm tra tham số thứ tự
				if (arg3 < 0 || arg3 >= 10 || fileSystem->openf[arg3] == NULL) // kiểm tra nó có tồn tại file tại ô đó không?
					machine->WriteRegister(2, -1);
				else {
					OldPos = fileSystem->openf[arg3]->GetCurrentPos();
				
					// type must equals '0'
					buf = machine->User2System(arg1, arg2);
					int type = fileSystem->openf[arg3]->type;
					if (type <= 2 && type >= 0)//0-đọc/ghi; 1-chỉ đọc; 2 console in; 3-console write
					{	
						if (type <= 1 && fileSystem->openf[arg3]->Write(buf, arg2) > 0) 
						{
							// Copy data from kernel to user space
							NewPos = fileSystem->openf[arg3]->GetCurrentPos();
							machine->WriteRegister(2, NewPos - OldPos);
						} else
							machine->WriteRegister(2, -1);
					} else if (type == 3) {
						// Write data to console
						if (type == 3)
						{
							int i = 0;
							while (buf[i] != 0)
							{
								gSynchConsole->Write(buf + i, 1);
								i++;
							}
							machine->WriteRegister(2, i - 1);
						}
					} else
						machine->WriteRegister(2, -1);
				}
				delete[] buf;
				
				break;
			}
			
			case SC_Seek: 
				arg1 = machine->ReadRegister(4);//pos
				arg2 = machine->ReadRegister(5);//index
				if (arg2 < 2 || arg2 >= 10 || fileSystem->openf[arg2] == NULL) // kiểm tra nó có tồn tại file tại ô đó không?
					machine->WriteRegister(2, -1);
					
				arg1 = (arg1 == -1) ? fileSystem->openf[arg2]->Length() : arg1;
				if (arg1 > fileSystem->openf[arg2]->Length() || arg1 < 0)
					machine->WriteRegister(2, -1);
				else {
					fileSystem->openf[arg2]->Seek(arg1);
					machine->WriteRegister(2, arg1);
				}
				
				break;
			case SC_Exec:
				arg1 = machine->ReadRegister(4);//filename
				buf = machine->User2System(arg1, BUFFER_LENGTH - 1);
				//printf("SC_EXEC\n");
				machine->WriteRegister(2, pTab->ExecUpdate(buf));
				//printf("SC_EXEC done\n");
				break;
			case SC_Join:
				arg1 = machine->ReadRegister(4);//pid;

				machine->WriteRegister(2, pTab->JoinUpdate(arg1));
				
				break;
			case SC_Exit:
				arg1 = machine->ReadRegister(4);//exitcode
				
				// nếu lỗi
				if (arg1 != 0) {
					printf("\nThread %s thoat bi loi : %d", currentThread->getName(), arg1);
					machine->WriteRegister(2, -1);
				} else
					machine->WriteRegister(2, pTab->ExitUpdate(arg1));
				
				break;
			case SC_CreateSemaphore:
				arg1 = machine->ReadRegister(4);//name
				arg2 = machine->ReadRegister(5);//semval
				buf = machine->User2System(arg1, BUFFER_LENGTH - 1);
				if(buf == NULL)
				{
					printf("\nSC_Semaphore :: Name = NULL");
					machine->WriteRegister(2,-1);
				} else {
					arg3 = semTab->Create(buf, arg2);//sem id 
					if(arg3 < 0)
					{
						printf("\nSC_Semaphore :: Khong the tao semaphore : %s", buf);
						machine->WriteRegister(2, -1);
					} else 
						machine->WriteRegister(2,0);
				}
				
				
				break;
			case SC_Up:
				arg1 = machine->ReadRegister(4);//filename
				buf = machine->User2System(arg1, BUFFER_LENGTH - 1);
				machine->WriteRegister(2, semTab->Signal(buf));
				
				break;
			case SC_Down:
				arg1 = machine->ReadRegister(4);//filename
				buf = machine->User2System(arg1, BUFFER_LENGTH - 1);
				machine->WriteRegister(2, semTab->Wait(buf));
				
				break;
			default:
				
				break;
		}
		
    } else if (which == NoException) {
		printf("Return to kernel");
		
	} else if (which == PageFaultException) {
		printf("PageFaultException: No valid translation found\n");
		//interrupt->Halt();
		}
	else if (which == ReadOnlyException) {
		printf("ReadOnlyException: Write attempted to page marked \"read-only\"\n");
		//interrupt->Halt();
		}
	else if (which == BusErrorException) {
		printf("BusErrorException: Translation resulted in an invalid physical address\n");
		//interrupt->Halt();
		}
	else if (which == AddressErrorException) {
		printf("AddressErrorException: Unaligned reference or one that was beyond the end of the address space\n");
		//interrupt->Halt();
		}
	else if (which == OverflowException) {
		printf("OverflowException: Integer overflow in add or sub.\n");
		//interrupt->Halt();
		}
	else if (which == IllegalInstrException) {
		printf("IllegalInstrException: Unimplemented or reserved instr.\n");
		//interrupt->Halt();
		}
	else if (which == NumExceptionTypes) {
		printf("NumExceptionTypes: error.\n");
		//interrupt->Halt();
		}
	else {
		printf("Unexpected user mode exception %d %d\n", which, type);
	//	ASSERT(FALSE);
		
    }
	increasePC();
	delete []buf;
}

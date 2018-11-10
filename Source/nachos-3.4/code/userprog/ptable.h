#ifndef PTABLE_H
#define PTABLE_H

#include "thread.h"
#include "synch.h"
#include "utility.h"
#include "syscall.h"
#include "bitmap.h"
#include "pcb.h"
#include "sem.h"
#define MAX_PROCESS 10
class PTable{
private:
	BitMap *bm;
	PCB* pcb[MAX_PROCESS];
	int psize;
	Sem* bmsem;
public:
	PTable(int size);
	~PTable();
	int ExecUpdate(char* name);
	int ExitUpdate(int ec);
	int JoinUpdate(int id);
	int GetFreeSlot();
	bool IsExist(int pid);
	void Remove(int pid);
	char* GetFileName(int id); 
};
#endif
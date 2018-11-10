#include "pcb.h"
#include "system.h"



PCB::PCB(): pid(0),
		parentID(-1),
		numwait(0),
		exitcode(0),
		thread(NULL),
		joinsem(new Semaphore("joinsem",0)),
		exitsem(new Semaphore("exitsem",0)),
		mutex(new Semaphore("mutex",1)){}
		
PCB::PCB(int id): pid(id),
		parentID(-1),
		numwait(0),
		exitcode(0),
		thread(NULL),
		joinsem(new Semaphore("joinsem",0)),
		exitsem(new Semaphore("exitsem",0)),
		mutex(new Semaphore("mutex",1)) {
	if(id != 0)
		parentID = currentThread->processID;//currentThread - thread we are running noW!
	else
		parentID = 0; // trùm ahihi
}

PCB::~PCB() {
	
	//printf("Delete PCB!\n");
	if (joinsem != NULL)
		delete joinsem;
	if (exitsem != NULL)
		delete exitsem;
	if (mutex != NULL)
		delete mutex;
	
	//printf("Delete PCB!\n");
	if (thread != NULL) {
		thread->FreeSpace();
		
		//printf("Delete PCB!\n");
		thread->Finish();
		
		//printf("Delete PCB!\n");
		//delete thread;
	}
}

void pFunc(int pID)
{
	char *filename = pTab->GetFileName(pID);
	//printf("FUNCTION\n");
	//mờ file thử 
	OpenFile *file = fileSystem->Open(filename);
	if (file == NULL) {
		printf("Ko ton tai file!");
		return;
	}
	//xin bộ nhớ
	AddrSpace *space= new AddrSpace(file);
	
	if(space == NULL) {//thua
		printf("not enough space to share!\n");
		return;
	}
	currentThread->space = space;

	space->InitRegisters();		// set the initial register values
	space->RestoreState();		// load page table register

	machine->Run();			// jump to the user progam
	//ASSERT(FALSE);			// machine->Run never returns;
				// the address space exits
				// by doing the syscall "exit"
	printf("\nAfter assert");  
}

int PCB::Exec(char *filename, int id) {
	mutex->P();
	
	//thử tạo tiến trình mới
	thread = new Thread(filename);
	if(thread == NULL) {//thua tạo không đc 
		mutex->V();
		return -1;
	}
	thread->processID = id;
	parentID = currentThread->processID;

	thread->Fork(pFunc, id);

	mutex->V();

	return pid;
}

//2 cái này đụng biến toàn cục, em ức chế lắm ạ... cứ nhào nhào vô làm liền bị sai mất 2 ngày....................asd fkjdf adsljfksadf
void PCB::IncNumWait(){
	mutex->P();
	++numwait;
	mutex->V();
}

void PCB::DecNumWait(){
	mutex->P();
	if (numwait > 0)
		--numwait;
	mutex->V();
}
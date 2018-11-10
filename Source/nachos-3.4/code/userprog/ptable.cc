#include "ptable.h"
#include "system.h"
PTable::PTable(int size) {
	if (size > MAX_PROCESS)
		size = MAX_PROCESS;
	
	psize = size;
	bm = new BitMap(size);
	//0 = trùm
	bm->Mark(0);

	for (int i = 0; i < psize ; i++)
		pcb[i] = NULL;
	 
	bmsem = new Sem("bitmapAccess",1);
}
PTable::~PTable() {
	int i;
	if (bm != NULL)
		delete bm;
	if (bmsem != NULL)
	delete bmsem;
	
	psize = 0;
}

int PTable::ExecUpdate(char* name) {
	
	if (name == NULL || strlen(name) == 0) {// em nghĩ chắc cô cũng thích chơi lầy giống bọn em nè hehe (ksp)
		printf("\nName loi!");
		return -1;
	}


	//Kiểm tra độ dài tên cho vừa ý, e thấy không cần thiết lắm nhưng có cái define này thôi quăng vào đây ạ
	if (strlen(name) >= BUFFER_LENGTH - 1) 
		return -1;

	//Kiểm tra file có tồn tại không?
	OpenFile* executable = fileSystem->Open(name);
	//you know what i means
	if (executable == NULL) {
		
		printf("\nKo ton tai file");
		return -1;
	}

	delete executable; // xóa chứ để làm chi cho chật ram ạ :D 


	// cái này nhờ bạn Đạt, tự nó gọi nó, loop forever cho đến khi hết RAM => sập ............................................................. na chos á
	if (strcmp(currentThread->getName(),name) == 0)
		return -1;
	bmsem->P();
	int pid = GetFreeSlot();    // tìm chỗ trống rồi chui vô thôi ahihi ahehe
	//thua, em tìm không ra slot
	if (pid == -1)
		return -1;
	
	// cấp phát 1 PCB mới nè cô
	
	pcb[pid] = new PCB(pid);
	pcb[pid]->parentID = currentThread->processID;
	
	bm->Mark(pid);
	int ret = pcb[pid]->Exec(name, pid);
	bmsem->V();
	return ret;
}

int PTable::GetFreeSlot(){
	//printf("Get FRee Slot!\n");
	int id;
	//bmsem->P();
	id = (bm->Find());
	//bmsem->V();
	return id;
}

char* PTable::GetFileName(int id)
{
	if(bm->Test(id))
		return pcb[id]->GetFileName();
	else 
		return NULL;
}









int PTable::ExitUpdate(int exitcode)
{
   
  
	int pid = currentThread->processID;
	
  // kiem tra ton tai
	if(!IsExist(pid)){
		printf("\nPTable::ExitUpdate unexpected process id %d",pid);
		return -1;
	}
  //
	if(pid == 0) {//trùm =>halt
	  
		interrupt->Halt();
		return 0;
	}

   
   
	pcb[pid]->SetExitCode(exitcode);

	// Neu co tien trinh cha doi no
	if(pcb[pid]->GetNumWait() > 0) { 
		pcb[pid]->JoinRelease();       // release cho tien trinh cha

		pcb[pid]->ExitWait();           // Xin phep thoat
		pcb[pid]->DecNumWait();

	}
	Remove(pid);         // Xoa khoi pcb

 

	return 0;
}

int PTable::JoinUpdate(int pid)
{
	
  IntStatus oldLevel = interrupt->SetLevel(IntOff);
  int currentid = currentThread->processID;
  
  if (currentid == pid){//tự nó join nó => loop
    printf("\n Can't join to yourself or main thread");
    interrupt->SetLevel(oldLevel);
    return -1;
  }
 

  if(pcb[pid] == NULL){//not enough main memory 
    interrupt->SetLevel(oldLevel);
    return -1;
  }

  if(currentid != pcb[pid]->parentID){//join nhà người ta hả?
    printf("\nPtable:Join: Process %d is not a child of calling process %d",pid,currentid);
    interrupt->SetLevel(oldLevel);
    return -1;
  }
  
  pcb[pid]->IncNumWait();

  pcb[pid]->JoinWait();

  int exitcode = pcb[pid]->GetExitCode();

  pcb[pid]->ExitRelease();


  interrupt->SetLevel(oldLevel);
  // Tra ve trang thai ban dau

  return (exitcode);
}

// kiểm tra tồn tại pid đó hay chưa
bool PTable::IsExist(int pid){
	return bm->Test(pid);
}

//khi tien trinh ket thuc , xoa 1 processID ra khoi mang quan ly no
void PTable::Remove(int pid){
	
	//bmsem->P();

	bm->Clear(pid);
	
	if ((int)pcb[pid] > 0)
		delete pcb[pid];
	
	//printf("Remove %d!\n", pid);
	pcb[pid] = NULL;
	//bmsem->V();
}
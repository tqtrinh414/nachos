#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"
#include "utility.h"
#include "syscall.h"
class PCB{
 private:
  Semaphore *joinsem;
  Semaphore *exitsem;
  Semaphore *mutex;
  int        exitcode; 
  int        numwait;

  Thread    *thread;
  int        pid;
 public: 
  int        parentID;
  PCB();
  PCB(int id);
  ~PCB();
  //đã lưu pid ở kia rồi thì không e nghi không cần phải thêm pid ở exec
  int Exec(char *filename, int id);
  int GetID() {return pid;}
  int GetNumWait() {return numwait;}
  void JoinWait(){joinsem->P();}
  void ExitWait(){exitsem->P();}
  void JoinRelease(){joinsem->V();}
  void ExitRelease(){exitsem->V();}
  void IncNumWait();
  void DecNumWait();
  void SetExitCode(int ec){exitcode = ec;}
  int  GetExitCode(){return exitcode;}
  void SetFileName(char* fn){if (thread) thread->setName(fn);}
  char* GetFileName() {if (thread) return thread->getName(); else return NULL;}
};
#endif
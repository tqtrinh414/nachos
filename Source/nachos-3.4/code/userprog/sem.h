#ifndef SEM_H
#define SEM_H
#include "synch.h"
//em thấy cô viết đoạn này không hay lắm, đề xuất ngắn hơn ạ, tiết kiệm hơn
class Sem:public Semaphore {
public:
	Sem(char* na, int i):Semaphore(na, i) {}
	~Sem(){}
	void wait(){
		P();
	}
	void signal(){
		V();
	}
	char* GetName(){
		return getName(); // :v
	}
};

#endif
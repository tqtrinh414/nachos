#include "stable.h"

STable::STable() {
	bm = new BitMap(MAX_SEMAPHORE);
	for (int i =0 ; i < MAX_SEMAPHORE;i++)
		semTab[i] = NULL;
		
}
STable::~STable() {
	delete bm;
	int i;
	for (i = 0 ; i < MAX_SEMAPHORE ; i++)
		if (semTab[i]!= NULL)
			delete semTab[i];
}
int STable::Create(char* name, int init) {//0 - ok; -1 error
	int id;
	id = bm->Find();
	if (id == -1) // kh�ng c�n ch? d? th�m d�u
	  return -1;
	for (int i = 0 ; i < MAX_SEMAPHORE ; i++)
	{
		if (!FindFreeSlot(i))
			if(strcmp(semTab[i]->GetName(), name) == 0)//tr�ng r?i nha c�
				return -1;
	}

	semTab[id] = new Sem(name, init);
	bm->Mark(id);

	return 0;
}
int STable::Wait(char* name) {
	int i, index;
	for (i = 0 ; i < MAX_SEMAPHORE ; i++) {
		if (!FindFreeSlot(i))
			if(strcmp(semTab[i]->GetName(), name) == 0 ) {
				index = i;
				break;
			}
	}
	
	if (i == MAX_SEMAPHORE )
		return -1;//kh�ng t?n t?i 
	
	semTab[index]->wait();//->P()
	return 0;
}
int STable::Signal(char* name) {//y s� c�i tr�n th�i nha
	int i, index;
	for (i = 0 ; i < MAX_SEMAPHORE ; i++) {
		if (!FindFreeSlot(i))
			if(strcmp(semTab[i]->GetName(), name) == 0 ) {
				index = i;
				break;
			}
	}
	
	if (i == MAX_SEMAPHORE )
		return -1;//kh�ng t?n t?i 
	
	semTab[index]->signal();//->V()
	return 0;
}
int STable::FindFreeSlot(int id) {
	return semTab[id] == NULL;
}
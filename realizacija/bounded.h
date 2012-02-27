//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    bounded.h
//   Date:    June 2011
//   Opis:		Kruzni ograniceni bafer sa sinhronizacijom; ako se cita
//						element, moze se prekinuti sa wakeUp() i bice vraceno 0;
//						ako se upisuje, bilo kakav wakeUp() budi nit, i ta vrijednost
//						se vraca proizvodjacu kao signal da nije uspjelo; u slucaju
//						uspjeha vraca se 0
//===========================================================================//

#ifndef _OS1_BOUNDED_BUFFER_
#define _OS1_BOUNDED_BUFFER_



#include <semaphor.h>



class BoundedBuffer {
public:

	BoundedBuffer (unsigned size);
	~BoundedBuffer ();
	int append (char);
	char take ();
	int  fullCount(){return itemAvailable.val();};    // potrebno consumeru

private:
	unsigned Size;
	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;
	char* buffer;
	int head, tail;
   
};


#endif  // _OS1_BOUNDED_BUFFER_

#ifndef MM
#define MM
#include "1.h"

int func(void);

class Interface {
	CintN* var;
	int k;
       	public:
	Interface(): var(NULL), k(0) {}

	~Interface();

	void add(const CintN& x);
	
	void set(int k, const CintN& x);	

	friend std::ostream& operator<< (std::ostream &os, const Interface &view);
}

#endif

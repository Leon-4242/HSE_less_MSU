#ifndef
#define

#include <iostream>

class CintN {
	private:
		char * val[N] {};
	public:
		CintN (int value = 0);
		
		~CintN ();
		
		CintN(const CintN &v);
		
		CintN operator+ (const CintN &v);
		
		CintN operator- (const CintN &v);
		
		bool operator > (const CintN &v) const;
		
		bool operator == (const CintN &v) const;
		
};
#endif

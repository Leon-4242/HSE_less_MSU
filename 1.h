#ifndef
#define

#include <iostream>
#include <string>
using namespace CintN {
class CintN {
	private:
		char * val[N] {};
		CintN (char *arr[N]);
	public:
		CintN (int value = 0);
		
		~CintN ();
		
		CintN(const CintN &v);
		
		CintN operator+ (const CintN &v);
		
		CintN operator- (const CintN &v);
		
		bool operator > (const CintN &v) const;
		
		bool operator == (const CintN &v) const;
		
		std::string print() const;		
};

}
#endif

#ifndef CINTN
#define CINTN
#define N 1

#include <iostream>
#include <string>

class CintN {
	private:
		char val[N] {};
		CintN (char arr[N]);
	public:
		CintN(void);

		CintN (int value);
		
		~CintN ();
		
		CintN(const CintN &v);
		
		CintN operator= (const CintN &v);

		bool operator > (const CintN &v) const;

		bool operator == (const CintN &v) const;

		CintN operator+ (const CintN &v) const;
		
		CintN operator- (const CintN &v) const;
		
		std::string print() const;

		CintN gen(void);	
};


int pow_int(int, int);
#endif

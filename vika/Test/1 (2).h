#ifndef CINTN
#define CINTN
#define N 10

#include <cstdlib>
#include <iostream>
#include <string>

class CintN {
	private:
		char val[N];
		CintN (char arr[N]);
		CintN (std::string val);
	public:
		CintN (int value);
		
		~CintN ();
		
		CintN(const CintN &v);
		
		bool operator > (const CintN &v) const;

		bool operator == (const CintN &v) const;

		CintN operator+ (const CintN &v) const;
		
		CintN operator- (const CintN &v) const;
		
		CintN& operator= (const CintN &v);
		
		std::string print() const;
		
		CintN& generate(int n);
		
		friend CintN make(bool flag_d, bool &flag);
};

#endif

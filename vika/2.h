#ifndef LIB2
#define LIB2

#include <cstring>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace cint {
	class CintN;

	char* print (const CintN&);
	CintN operator+ (const CintN&, const CintN&);
	CintN operator- (const CintN&, const CintN&);
	CintN operator* (const CintN&, const CintN&);

	CintN operator- (const CintN&);
	CintN operator+ (const CintN&);
	bool sgn(const CintN&);

	bool operator== (const CintN&, const CintN&);
	bool operator!= (const CintN&, const CintN&);
	bool operator> (const CintN&, const CintN&);
	bool operator< (const CintN&, const CintN&);
	bool operator>= (const CintN&, const CintN&);
	bool operator<= (const CintN&, const CintN&);

	std::ostream& operator<< (std::ostream&, const CintN&);

	class CintN {
		char* val;
		int N;
	
		public:
			
		CintN(void);
		CintN(char*);
		CintN(int);
		CintN(int, bool);
		CintN(const CintN&);

		char* getVal(void) const;
		int getN(void) const;

		~CintN(void);
		
		CintN& operator= (const CintN&);	
		CintN& operator+= (const CintN&);
		CintN& operator-= (const CintN&);	
		CintN& operator*= (const CintN&);
		CintN& operator++ (void);
		CintN operator++ (int);
		CintN& operator-- (void);
                CintN operator-- (int);

		friend std::istream& operator>> (std::istream&, CintN&);
	};
}

#endif 

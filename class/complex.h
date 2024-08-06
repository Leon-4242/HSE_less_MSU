#ifndef complexx
#define complexx

#include <math.h>

constexpr double eps = 1e-12;

class Complex {
	double x, y;
	
	public:
	Complex():
       		x(0.),
		y(0.)
	{ }

	Complex(double a, double b):
       		x(a), 
		y(b)	
	{ }

	Complex(const Complex& z):
		x(z.x),
		y(z.y)
	{ }

	~Complex() { };

	Complex& operator= (const Complex& z);

	Complex operator+ (const Complex& z);
	Complex& operator+= (const Complex& z);

	Complex operator- (const Complex& z);
	Complex& operator-= (const Complex& z);
	Complex operator- ();

	Complex operator* (const Complex& z);
	Complex& operator*= (const Complex& z);

	Complex operator* (double c);
	Complex& operator*= (double c);

	double mod() {
		return sqrt(x*x+y*y);
	}

	double arg() {
		return(acos(x/(this->mod())));
	}

	Complex conj();
};

#endif

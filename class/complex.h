#ifndef complexx
#define complexx

#include <math.h>
#include <iostream>

namespace comp {

	constexpr double eps = 1e-12;

	class Complex;

	Complex operator+ (const Complex& z1, const Complex& z2);
	Complex operator- (const Complex& z1, const Complex& z2);
	Complex operator* (const Complex& z1, const Complex& z2);
	Complex operator/ (const Complex& z1, const Complex& z2);
	Complex operator- (const Complex& z);
	Complex operator+ (const Complex& z);

	bool operator== (const Complex& z1, const Complex& z2);
	bool operator!= (const Complex& z1, const Complex& z2);

	std::ostream& operator<< (std::ostream& os, const Complex& z);

	Complex rev(const Complex& z);
	Complex conj (const Complex& z);
	double mod(const Complex& z);
	double arg(const Complex& z);
	Complex powCI(const Complex& base, const int& exp);
	class Complex {
		double x, y;
	
		public:
		Complex(): x(0.), y(0.) {}
		
		Complex(double a, double b = 0): x(a), y(b) {}
	
		Complex(const Complex& z): x(z.x), y(z.y) {}

		~Complex() {};
		
		double real(void) const {
			return x;
		}

		double imag(void) const {
			return y;
		}
			
		Complex& operator= (const Complex& z) {
			x = z.x; y = z.y;
			return *this;
		}

		Complex& operator+= (const Complex& z) {
			*this = *this + z;
			return *this;
		}

		Complex& operator-= (const Complex& z) {
			*this = *this - z;
			return *this;
		}
	
		Complex& operator*= (const Complex& z) {
			*this = (*this)*z;
			return *this;
		}

		Complex& operator/= (const Complex& z) {
			*this = *this/z;
			return *this;
		}

		friend std::istream& operator>> (std::istream& is, Complex& z);
	};
}
#endif

#ifndef complexx
#define complexx

#include <math.h>

namespace comp {

	constexpr double eps = 1e-12;

	class Complex;

	Complex operator+ (const Complex& z1, const Complex& z2);
	Complex operator- (const Complex& z1, const Complex& z2);
	Complex operator* (const Complex& z1, const Complex& z2);
	Complex operator/ (const Complex& z1, const Complex& z2);
	Complex operator- (const Complex& z);

	Complex rev (const Complex& z) {
		z1 = conj(z);
		if (mod(z1) < eps) {
			exit(-1);
		}
		return Complex(z1.real()/mod(z1), z1.imag()/mod(z1));
	}

	Complex conj (const Complex& z) {
		return Complex(z.real, -z.imag);
	}

	double mod(const Complex& z) {
        	return sqrt(z.real()*z.real()+z.imag()*z.imag());
	}

        double arg(const Complex& z) {
		if (mod(z) < eps) {
			return 0;
		}
                return(acos(z.real()/(mod(z))));
	}


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
	};
}
#endif

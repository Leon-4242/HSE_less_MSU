#ifndef COMPLEX
#define COMPLEX

#include <cmath>
#include <iostream>
#include <string>

namespace comp {

	constexpr double eps = 1e-12;
	
	class Complex {
		double x, y;
	
		public:
		Complex(): x(0.), y(0.) {}
		
		Complex(double a, double b = 0): x(a), y(b) {}
	
		Complex(const Complex& z): x(z.x), y(z.y) {}
		
		Complex(Complex&& mv) {
			x = mv.x;
			y = mv.y;
			mv.x = mv.y = 0;
		}

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

		Complex& operator= (Complex&& mv) {
			x = mv.x; y = mv.y;
			mv.x = mv.y = 0;
			return *this;
		}
		
		friend Complex operator+ (const Complex& z1, const Complex& z2);
		friend Complex operator- (const Complex& z1, const Complex& z2);
		friend Complex operator* (const Complex& z1, const Complex& z2);
		friend Complex operator/ (const Complex& z1, const Complex& z2);
		friend Complex operator- (const Complex& z);
		friend Complex operator+ (const Complex& z);

		friend bool operator== (const Complex& z1, const Complex& z2);
		friend bool operator!= (const Complex& z1, const Complex& z2);
		friend bool operator< (const Complex& z1, const Complex& z2);	
		friend bool operator<= (const Complex& z1, const Complex& z2);
		friend bool operator> (const Complex& z1, const Complex& z2);
		friend bool operator>= (const Complex& z1, const Complex& z2);

		friend std::string print(const Complex& z);
		friend std::ostream& operator<< (std::ostream& os, const Complex& z);
		friend std::istream& operator>> (std::istream& is, Complex& z);

		friend Complex rev(const Complex& z);
		friend Complex conj (const Complex& z);
		friend double mod(const Complex& z);
		friend double arg(const Complex& z);
		friend Complex powCI(const Complex& base, const int& exp);

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
	};
	
	
	Complex operator+ (const Complex& z1, const Complex& z2) {
		return Complex(z1.real()+z2.real(), z1.imag() + z2.imag()); 
	}

	Complex operator- (const Complex& z1, const Complex& z2) {
		return Complex(z1.real()-z2.real(), z1.imag() - z2.imag());
	}
	
	Complex operator* (const Complex& z1, const Complex& z2) {
		return Complex(z1.real()*z2.real()-z1.imag()*z2.imag(), z1.real()*z2.imag()+z1.imag()*z2.real());
	}

	Complex operator/ (const Complex& z1, const Complex& z2) {
		return z1*rev(z2);			
	}

	Complex operator- (const Complex& z) {
		return Complex(-z.real(), -z.imag());
	}

	Complex operator+ (const Complex& z) {
		return Complex(z.real(), z.imag());
	}

	bool operator== (const Complex& z1, const Complex& z2) {
		return fabs(z1.real()-z2.real()) < eps && fabs(z1.imag()-z2.imag()) < eps;
	}
	
	bool operator!= (const Complex& z1, const Complex& z2) {
		return !(z1 == z2);
	}
	
	bool operator< (const Complex& z1, const Complex& z2) {
		return (z1.real() < z2.real() && z1.imag() < z2.imag());
	}

        bool operator<= (const Complex& z1, const Complex& z2) {
		return z1 == z2 || z1 < z2;
	}

        bool operator> (const Complex& z1, const Complex& z2) {
		return !(z1 <= z2);
	}

        bool operator>= (const Complex& z1, const Complex& z2) {
		return !(z1 < z2);
	}

	std::string print(const Complex& z) {
		std::string str;
		str += std::to_string(z.real());
		if (fabs(z.imag()) > eps) {
			str += "+i*("+std::to_string(z.imag()) + ")";
		}
		return str;
	}

	std::ostream& operator<< (std::ostream& os, const Complex& z) {
		return os <<"(" << print(z) << ")";
	}

	std::istream& operator>> (std::istream& is, Complex& z) {
		is >> z.x >> z.y;
		return is;
	}

	 Complex rev(const Complex& z) {
                 if (mod(z) < eps || mod(z)*mod(z) < eps) {
                         exit(-1);
                 }
                 return Complex(z.real()/(mod(z)*mod(z)), -z.imag()/(mod(z)*mod(z)));
         }

         Complex conj (const Complex& z) {
                 return Complex(z.real(), -z.imag());
         }

         double mod(const Complex& z) {
                 return sqrt(z.real()*z.real()+z.imag()*z.imag());
         }

         double arg(const Complex& z) {
                 if (fabs(z.real()) < eps) {
                         return M_PI/2;
                 }
                 return atan2(z.imag(), z.real()) ;
         }

	Complex powCI(const Complex& base, const int& exp) {
		Complex a(base), y(1); int b(exp);

		while (b > 0) {
			if (b%2 == 0) {
				b /= 2;
				a *= a;
			} else {
				b--;
				y *= a;
			}
		}
		return y;
	}
}
#endif

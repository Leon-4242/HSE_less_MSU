#include "complex.h"
namespace comp {
	Complex::Complex(const std::string str) {
		size_t tmp = str.find("+");
                if (tmp == std::string::npos) {
                        x = stod(str);
			y = 0;
                } else {
                        x = stod(str.substr(0, tmp));
                        size_t buff = str.find("i", tmp);
                        if (buff == std::string::npos) {
                                exit(-1);
                        }
                        auto curr = ((buff-tmp == 1) ? buff : tmp ) + 1;
                        y = stod(str.substr(curr, str.size()-curr ));
                }
	}

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
		str.append(std::to_string(z.real()));
		if (!(z.imag() < eps)) {
			str.append("+i*("+std::to_string(z.imag()) + ")");
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
                 Complex z1 = conj(z);
                 if (mod(z1) < eps) {
                         exit(-1);
                 }
                 return Complex(z1.real()/mod(z1), z1.imag()/mod(z1));
         }

         Complex conj (const Complex& z) {
                 return Complex(z.real(), -z.imag());
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



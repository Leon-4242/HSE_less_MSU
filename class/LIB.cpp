#include "LIB.h"

namespace EXCEPT {
	const char* Except::what() const noexcept {
		return message.c_str();
	}

	Except::Except(const std::string text): message(text) { }

	Except::~Except() {	}
    
}

namespace CMP {
	int cmp (const std::string str1, const std::string str2) {
        if (cmplen(str1, str2) != 0) return cmplen(str1, str2);
        if (str1 > str2) return 1;
        if (str1 < str2) return -1;

        return 0;
    }
    
    int cmplen (const std::string str1, const std::string str2) {
        if (str1.length() < str2.length()) return -1;
        if (str1.length() > str2.length()) return 1;
        return 0;
    }
    
}


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

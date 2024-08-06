#include "complex.h"

Complex& Complex::operator= (const Complex& z){
	x = z.x; y = z.y;
	return *this;	
}

Complex Complex::operator+ (const Complex& z) {
	return Complex(x+z.x, y+z.y);
}
Complex& Complex::operator+= (const Complex& z) {
	x += z.x;
	y += z.y;
	return *this;
}

Complex Complex::operator- (const Complex& z) {
	return Complex(x-z.x, y-z.y);
}
Complex& Complex::operator-= (const Complex& z) {
        x -= z.x;
        y -= z.y;
        return *this;
}
Complex Complex::operator- () {
	return Complex(-x, -y);
}

Complex Complex::operator* (const Complex& z) {
	return Complex(x*z.x-y*z.y, x*z.y+y*z.x);	
}
Complex& Complex::operator*= (const Complex& z) {
	double tmp = x;
        x = x*z.x-y*z.y;
	y = tmp*z.y+y*z.x;
	return *this;	
}

Complex Complex::operator* (double c) {
	return Complex(x*c, y*c);
}
Complex& Complex::operator*= (double c) {
	x *= c;
	y *= c;
	return *this;
}
Complex Complex::conj(){
	return Complex(x, -y);	
}


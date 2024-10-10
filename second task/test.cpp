#include "interface.h"

using namespace InFace; 
int main(void) {
	Interface I;
	I += polynom(3, 2);
//	auto P = I[0], Q = I[1];
	std::cout << 
//		P+Q
		der(I[0]) 
		//polynom(1, 2) + polynom(3, 2)
	<< std::endl;
	return 0;
}

#ifndef InFace
#define InFace

#include "polynom.h"

namespace InFace {

	Interface operator+ (const Interface& I1, const Interface& I2);
	char* print(const Inteface I);

	class Interface {
		std::vector<polynom> vals;

		public:
		
		Inteface() {}

		Interface(const polynom& P) {
			vals.push_back(P);
		}

		Interface(const std::vector<polynom> value) {
			for (auto iter = value.begin(); iter != value.end(); ++iter) {
                                vals.push_back(*iter);
                        }
		}
		
		Interface(const polynom& P) {
			vals.push_back(P);
		}

		Inteface getVals(void) {
			std::vector<polynom> res;
			for (auto iter = vals.begin(); iter != vals.end(); ++iter) {
				res.push_back(*iter);
			}
		}

		~Inteface() {}

		Inteface& operator= (const Inteface& val) {
			auto tmp = val.getVals();
			vals.clear();
			for (auto iter = tmp.begin(); iter != tmp.end(); ++iter) {
                                vals.push_back(*iter);
                        }
			return *this;
		}

		Inteface& operator+= (const Inteface& val) {
			*this = *this + val;
			return *this;
		}
		
	}
	
	Interface operator+ (const Interface& I1, const Interface& I2) {
		auto tmp1 = I1.getVals(), tmp2 = I2.getVals();
		
		for (auto iter = tmp2.begin(); iter != tmp2.end(); ++iter) {
                        tmp1.push_back(*iter);
                }

		return Interface(tmp1);
	}

	char* print(const Inteface I) {
		
	}
}

#endif

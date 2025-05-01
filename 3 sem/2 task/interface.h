#ifndef InterFace
#define InterFace

#include "polynom.h"

namespace InFace {	
	using namespace poly;

	class Interface;

	std::string print(const Interface I);

	std::ostream& operator<< (std::ostream& os, const Interface& I);

	class Interface {
		std::vector<polynom> vals;

		public:
		
		friend Interface operator+ (const Interface& I1, const Interface& I2);
	
		Interface(void) {}

		Interface(const polynom& P) {
			vals.push_back(P);
		}
		Interface(Interface&& I) {
			vals.swap(I.vals);
			I.vals.clear();
		}
		Interface(const std::vector<polynom> value) {
			for (auto iter = value.begin(); iter != value.end(); ++iter) {
                                vals.push_back(*iter);
                        }
		}
		
		Interface(const Interface& I) {
			auto tmp = I.getVals();
			for (auto iter = tmp.begin(); iter != tmp.end(); ++iter) {
				vals.push_back(*iter);
			}
		}

		std::vector<polynom> getVals(void) const{
			std::vector<polynom> res;
			for (auto iter = vals.begin(); iter != vals.end(); ++iter) {
				res.push_back(*iter);
			}

			return res;
		}

		~Interface() {}

		Interface& operator= (const Interface& val) {
			vals.clear();
			for (auto iter = val.vals.begin(); iter != val.vals.end(); ++iter) {
                                vals.push_back(*iter);
                        }
			return *this;
		}

		Interface& operator= (Interface&& val) {
			vals.swap(val.vals);
			val.vals.clear();
			return *this;
		}

		Interface& operator+= (const Interface& val) {
			*this = *this + val;
			return *this;
		}

		bool empty(void) const {
			return vals.empty();
		}

		size_t size(void) const{
			return vals.size();
		}
		
		polynom operator[] (unsigned k);
	};
	
	Interface operator+ (const Interface& I1, const Interface& I2) {
		auto tmp = I1.vals;
		for (auto iter = I2.vals.begin(); iter != I2.vals.end(); ++iter) {
                        tmp.push_back(*iter);
                }

		return Interface(tmp);
	}

	std::string print(const Interface I) {
		auto tmp = I.getVals(); std::string str;
		for (auto iter = tmp.begin(); iter != tmp.end(); ++iter) {
			str += "z" + std::to_string(iter-tmp.begin()+1) + ": " + print(*iter) +"\n";
		}

		return str;
	}

	std::ostream& operator<< (std::ostream& os, const Interface& I) {
		return os << print(I);
	}

	polynom Interface::operator[] (unsigned k) {
		return vals[k];
	}
}

#endif

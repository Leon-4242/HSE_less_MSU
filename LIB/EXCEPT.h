#ifndef EXCP
#define EXCP

#include <exception>
#include <string>

namespace EXCEPT {
    class Except: public std::exception {
		std::string message;
		public:
		Except::Except(const std::string text): message(text) { }
		Except::~Except() {	}
		const char* what() const noexcept override;
	};
	
	const char* Except::what() const noexcept {
		return message.c_str();
	}
}

#endif


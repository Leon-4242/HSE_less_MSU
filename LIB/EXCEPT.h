#ifndef EXCP
#define EXCP

#include <exception>
#include <string>

namespace EXCEPT {
    class Except: public std::exception {
		std::string message;
		public:
		Except(const std::string text): message(text) { }
		~Except() {	}
		const char* what() const noexcept override {
			return message.c_str();
		}
	};
}

#endif


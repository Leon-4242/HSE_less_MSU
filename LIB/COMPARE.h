#ifndef COMPARE
#define COMPARE

#include <string>

namespace CMP {
	int cmplen (const std::string str1, const std::string str2);
	int cmp (const std::string str1, const std::string str2);


    int cmplen (const std::string str1, const std::string str2) {
        if (str1.length() < str2.length()) return -1;
        if (str1.length() > str2.length()) return 1;
        return 0;
    }

	int cmp (const std::string str1, const std::string str2) {
        if (cmplen(str1, str2) != 0) return cmplen(str1, str2);
        if (str1 > str2) return 1;
        if (str1 < str2) return -1;

        return 0;
    }
    
}

#endif

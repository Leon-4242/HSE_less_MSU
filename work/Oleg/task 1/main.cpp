#include "string_list.hpp"

int cmp(const char*, const char*);

int main(void) {
	CStringList lst{};

	lst.AddToEnd("3");
	lst.AddToEnd("2");
	lst.AddToEnd("4");
	lst.AddToEnd("1");
	
	for(auto elem : lst) std::cout << elem << " ";
	std::cout << std::endl;

	lst.Sort(cmp);

	for(auto elem : lst) std::cout << elem << " ";
	std::cout << std::endl;

	return 0;
}

int cmp(const char* a, const char* b) {
	int i = 0;
	while (*(a+i) && *(b+i)) {
		if (*(a+i) - *(b+i) != 0) return *(a+i) - *(b+i);
		++i;
	}
	if (*(a+i)) return 1;
	if (*(b+i)) return -1;
	return 0;
}

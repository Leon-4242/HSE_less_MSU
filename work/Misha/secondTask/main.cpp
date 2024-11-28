#include "APROXIMATOR.h"

using namespace APROXIMATOR;

int main(void) {
    Aproximator func;
    func[1] = 1;
    std::cout << func << std::endl;
    func(2, 3);
    std::cout << func << std::endl;
    func.remove(1);
    std::cout << func << std::endl;
    std::cout << func(4) << std::endl;
    return 0;
}

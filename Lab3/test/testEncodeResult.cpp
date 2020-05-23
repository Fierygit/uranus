#include <iostream>
#include <string>
#include "../common/Util.h"
#include "../common/public.h"

int main() {
    std::string res = Util::EncodeResult("Comput cla");
    std::cout << res << std::endl;
}
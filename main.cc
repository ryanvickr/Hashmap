#include <functional>
#include <iostream>

#include "src/hashmap.hpp"

int main() {
    std::cout << "Hello" << std::endl;

    std::function<int(int)> hash_func = [](int key) {
        return 1;
    };

    auto map = std::make_unique<HashMap<int, int>>(10, hash_func);
    return 0;
}

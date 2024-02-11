#include <functional>
#include <iostream>

#include "src/hashmap.hpp"

int main() {
    std::cout << "Hello" << std::endl;

    std::function<int(const int&)> hash_func = [](const int& key) {
        return 1;
    };

    auto map = std::make_unique<HashMap<int, int>>(10, hash_func);

    map->Insert(1, 3);

    auto* result = map->Get(1);
    if (result == nullptr) {
        std::cerr << "fail." << std::endl;
        return 1;
    } else {
        std::cout << "Got value: " << *result << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}

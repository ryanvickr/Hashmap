#include <functional>
#include <iostream>

#include "src/hashmap.hpp"

int main() {
    std::cout << "Hello" << std::endl;

    std::function<int(const int&)> hash_func = [](const int& key) {
        return 1;
    };

    auto map = std::make_unique<HashMap<int, int>>(10, hash_func);

    // Try and insert a value.
    map->Insert(1, 3);

    // Try and get the same value.
    auto* result = map->Get(1);
    if (result == nullptr) {
        std::cerr << "fail." << std::endl;
        return 1;
    } else {
        std::cout << "Got value: " << *result << std::endl;
    }

    // Try and remove a value.
    map->Remove(1);
    if (map->Contains(1)) {
        std::cerr << "Failed to remove object." << std::endl;
        return 1;
    } else {
        std::cout << "Removed object." << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}

#include <functional>
#include <iostream>

#include "src/hashmap.hpp"

const int kTestKey = 11;

int main() {
    auto map = std::make_unique<HashMap<int, int>>(10);

    // Try and insert a value.
    map->Insert(kTestKey, 3);

    // Try and get the same value.
    auto* result = map->Get(kTestKey);
    if (result == nullptr) {
        std::cerr << "fail." << std::endl;
        return 1;
    } else {
        std::cout << "Got value: " << *result << std::endl;
    }

    // Try and remove a value.
    map->Remove(kTestKey);
    if (map->Contains(kTestKey)) {
        std::cerr << "Failed to remove object." << std::endl;
        return 1;
    } else {
        std::cout << "Removed object." << std::endl;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}

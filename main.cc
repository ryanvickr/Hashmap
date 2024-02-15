// Author: Ryan Vickramasinghe
#include <functional>
#include <iostream>
#include <utility>
#include <assert.h>

#include "src/hashmap.hpp"

const u_int32_t kNumItems = 10;

int main() {
    std::cout << "Testing <int, int> map with copy constructor."
        << std::endl;
    HashMap<int, int> map0(10);
    for (int i = 0; i < kNumItems; i++) {
        map0.Insert(i, i+1);
    }
    HashMap<int, int> map1(std::move(map0));
    for (int i = 0; i < kNumItems; i++) {
        auto* val = map1.Get(i);
        if (*val != (i+1)) {
            std::cerr << "FAILURE" << std::endl;
            return 1;
        }
    }

    assert(map0.Size() == 0);

    // std::cout << "Testing <string, int> map." << std::endl;
    // auto map2 = std::make_unique<HashMap<std::string, int>>(10);
    // const std::vector<std::string> test_vals = 
    //     {"Zuzu", "Ira", "Ryan", "Edie"};
    // for (int i = 0; i < test_vals.size(); i++) {
    //     map2->Insert(test_vals[i], i);
    // }
    // for (int i = 0; i < test_vals.size(); i++) {
    //     auto* val = map2->Get(test_vals[i]);
    //     if (*val != i) {
    //         std::cerr << "FAILURE" << std::endl;
    //         return 1;
    //     }
    // }

    return 0;
}

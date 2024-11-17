#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    int nums[100];
    auto now = std::chrono::system_clock::now();
    RBTree<int> treeTest;

    for( auto i = 0, j = 1; i < 100; i++, j*=-1 ){
        nums[i] = i * j;
        treeTest.insert(&nums[i]);
    }
    auto end = std::chrono::system_clock::now();
    int foundAim = -1;
    std::cout << "Found: " << treeTest.find( &foundAim ) << std::endl;
    std::cout << "Finished with: " << end - now << std::endl;
}
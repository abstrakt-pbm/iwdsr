#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    auto now = std::chrono::system_clock::now();
    RBTree<int> treeTest;
    int testData = 1;
    int two = 2;
    treeTest.insert(&testData);
    treeTest.insert(&two);
    auto end = std::chrono::system_clock::now();
    std::cout << "Finished with: " << end - now << std::endl;
}
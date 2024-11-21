#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    int nums[100];
    RBTree<int> treeTest;

    for( auto i = 0, j = 1; i < 100; i++, j*=-1 ){
        nums[i] = i;
        treeTest.insert(&nums[i]);
    }
    std::cout << "Black depth: " << treeTest.getBlackDepth() << std::endl;
    for ( auto i = 0 ; i < 100; i++) {
        std::cout << "Find: " << treeTest.isExists(i) << std::endl;
    }
}
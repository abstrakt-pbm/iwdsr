#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    int nums[100000];
    RBTree<int> treeTest;

    for( auto i = 1, j = 1; i <= 10; i++, j*=-1 ){
        nums[i] = i*j;
        treeTest.insert(&nums[i]);
    }
    
    for ( auto i = 1, j = 1; i <= 10; i++, j*=-1 ) {
        std::cout << "Delete: " << i*j << std::endl;
        treeTest.del(i*j);
    }

    
    std::cout << "Find: " << treeTest.isKeyExists(100) << std::endl;
    std::cout << "Black Depth: " << treeTest.getBlackDepth() << std::endl;
}
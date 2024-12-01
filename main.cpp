#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    int nums[100000];
    RBTree<int> treeTest;

    for( auto i = 0, j = 1; i < 100000; i++, j*=-1 ){
        nums[i] = i;
        treeTest.insert(&nums[i]);
    }
    
    for ( auto i = 0; i < 100000 ; i++ ) {
        std::cout << "Delete: " << i << std::endl;
        treeTest.del(i);
    }

    
    std::cout << "Find: " << treeTest.isKeyExists(100) << std::endl;
    std::cout << "Black Depth: " << treeTest.getBlackDepth() << std::endl;
}
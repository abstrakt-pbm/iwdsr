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
        nums[i] = i;
        treeTest.insert(&nums[i]);
    }
    std::cout << "Root: " << *treeTest.root->getPayload() << std::endl;
    std::cout << "LeftChild: " << *treeTest.root->getLeftChild()->getPayload() << std::endl;
    std::cout << "RightChild: " << treeTest.root->getRightChild() << std::endl;
    auto end = std::chrono::system_clock::now();
    //int foundAim = 0;
    //std::cout << "Found: " << treeTest.find( &foundAim );
    std::cout << "Finished with: " << end - now << std::endl;
}
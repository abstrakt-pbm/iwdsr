#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "iwdsr/misc/data-structures/rbtree/rbtree.hpp"
#include <chrono>

int main() {
    int nums[101];
    auto now = std::chrono::system_clock::now();
    RBTree<int> treeTest;

    for( auto i = 0, j = 1; i <= 7; i++, j*=-1 ){
        nums[i] = i;
        treeTest.insert(&nums[i]);
    }
    auto end = std::chrono::system_clock::now();
    
    std::cout << "Root payload: " << *treeTest.root->getPayload() << std::endl;
    std::cout << "Left Payload: " << *treeTest.root->getLeftChild()->getPayload() << std::endl;
    std::cout << "Right Payload: " << *treeTest.root->getRightChild()->getPayload() << std::endl;
    std::cout << "Left colour: " << treeTest.root->getLeftChild()->getColour() << std::endl;
    std::cout << "Right colour: " << treeTest.root->getRightChild()->getColour() << std::endl;

    std::cout << "Depth is: " << treeTest.getDepth() << std::endl;
    std::cout << "Black depth is: " << treeTest.getBlackDepth() << std::endl;
    //std::cout << "Finished with: " << end - now << std::endl;
}
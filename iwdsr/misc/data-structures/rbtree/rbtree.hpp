#pragma once

enum RBColour : bool {
    RED,
    BLACK
};

template<typename NodeType>
class RBNode {
    public:
    RBNode* father;
    RBNode* leftChild;
    RBNode* rightChild;
    RBColour colour; 
    NodeType* payload;

    public: 
    RBNode(NodeType* payload = nullptr, RBColour colour = RBColour::BLACK, RBNode<NodeType>* father = nullptr, RBNode<NodeType>* leftChild = nullptr, RBNode<NodeType>* rightChild = nullptr);
    RBNode<NodeType>* getLeftChild();
    RBNode<NodeType>* getRightChild();
    RBNode<NodeType>* getFather();   
    NodeType* getPayload();
    RBColour getColour();

    void setLeftChild(RBNode<NodeType>* child);
    void setRightChild(RBNode<NodeType>* child);
    void setFather(RBNode<NodeType>* newFather);
    void setPayload(NodeType* newPayload);
    void changeColour(RBColour colour);
};

template<typename PayloadType>
class RBTree {
    public:
    RBNode<PayloadType>* root;

    void deleteNode(RBNode<PayloadType>* delNode);

    void rebalanceAfterInsert(RBNode<PayloadType>* relativeNode);
    void rebalanceAfterDelete(RBNode<PayloadType>* problemNode);

    void rotateRight(RBNode<PayloadType>* relativeNode);
    void rotateLeft(RBNode<PayloadType>* relativeNode);

    void rebalanceAfterInsertRotateCases(RBNode<PayloadType>* relativeNode);
    void rebalanceAfterInsertRepainting(RBNode<PayloadType>* realativeNode);

    void rebalanceAfterDeleteBB(RBNode<PayloadType>* relativeNode);
    void rebalanceAfterDeleteRB(RBNode<PayloadType>* relativeNode);

    public:
    RBTree();
    void insert(PayloadType*);
    void del(PayloadType key);
    PayloadType* find(PayloadType key);
    bool isKeyExists(PayloadType key);
    RBNode<PayloadType>* findNode( PayloadType key );
    uint64_t getDepth();
    uint64_t getBlackDepth();
    
};

template<typename PayloadType>
RBTree<PayloadType>::RBTree() {
    root = nullptr;
}

template<typename PayloadType> 
void RBTree<PayloadType>::insert(PayloadType* value) {
    if ( root == nullptr) {
        root = new RBNode<PayloadType>(value, RBColour::BLACK);
        root->setLeftChild(new RBNode<PayloadType>(nullptr, RBColour::BLACK, root));
        root->setRightChild(new RBNode<PayloadType>(nullptr, RBColour::BLACK, root));
        return;
    }

    RBNode<PayloadType>* currentNode = root;
    while(currentNode->getPayload() != nullptr){
        if ( *(currentNode->getPayload()) == *value ) {
           return; 
        } else if (*(currentNode->getPayload()) < *value) {
            currentNode = currentNode->getRightChild();
        } else {
            currentNode = currentNode->getLeftChild();
        }
    }

    currentNode->setPayload(value);
    currentNode->changeColour(RBColour::RED);
    currentNode->setLeftChild(new RBNode<PayloadType>(nullptr, RBColour::BLACK, currentNode));
    currentNode->setRightChild(new RBNode<PayloadType>(nullptr, RBColour::BLACK, currentNode));

    rebalanceAfterInsert(currentNode);
}

template<typename PayloadType>
void RBTree<PayloadType>::del(PayloadType key) {
    RBNode<PayloadType>* targetNode = findNode(key);
    if ( targetNode->getPayload() == nullptr) {
        return;
    }
    deleteNode(targetNode);
}

template<typename PayloadType>
void RBTree<PayloadType>::deleteNode(RBNode<PayloadType>* delNode) {
    if ( delNode->getLeftChild() == nullptr && delNode->getRightChild() == nullptr ) {
        delNode->setLeftChild(nullptr);
        delNode->setRightChild(nullptr);
        delNode->setPayload(nullptr); 
        delNode->changeColour(RBColour::BLACK);

    } else if ( delNode->getLeftChild() != nullptr && delNode->getRightChild() != nullptr ) {
        RBNode<PayloadType>* replacementNode = delNode->getLeftChild();
        while ( replacementNode->getLeftChild() != nullptr && replacementNode->getLeftChild()->getPayload() != nullptr ) {
            replacementNode = replacementNode->getLeftChild();
        }
        delNode->setPayload(replacementNode->getPayload());
        deleteNode(replacementNode);

    } else {
       RBNode<PayloadType>* existedChild = delNode->getLeftChild() == nullptr ? delNode->getRightChild() : delNode->getLeftChild();
       delNode->setLeftChild(existedChild->getLeftChild()); 
       delNode->setRightChild(existedChild->getRightChild()); 
       delNode->setPayload(existedChild->getPayload());
       delete existedChild;  
    }

    if ( delNode->getColour() == RBColour::BLACK && delNode != root) {
        rebalanceAfterDelete(delNode);
    }

}

template<typename PayloadType>
bool RBTree<PayloadType>::isKeyExists(PayloadType key) {
    RBNode<PayloadType>* target = findNode(key);
    return target->getPayload() != nullptr; 
}

template<typename PayloadType>
PayloadType* RBTree<PayloadType>::find(PayloadType key) {
    RBNode<PayloadType>* targetNode = findNode(key); 
    return targetNode->getPayload();
}

template<typename PayloadType>
RBNode<PayloadType>* RBTree<PayloadType>::findNode(PayloadType key) {
   RBNode<PayloadType>* currentNode = root;
    
    while ( currentNode->getPayload() != nullptr) { 
        if ( *currentNode->getPayload() == key ) {
            return currentNode;
        } else if ( *currentNode->getPayload() < key ) {
            currentNode = currentNode->getRightChild();
        } else {
            currentNode = currentNode->getLeftChild();
        }
    } 
    return currentNode;
}

template<typename PayloadType>
uint64_t RBTree<PayloadType>::getDepth() {
    uint64_t depth = 0;
    RBNode<PayloadType>* currentNode = root;
    while (currentNode->getRightChild() != nullptr) {
        depth++;
        currentNode = currentNode->getRightChild();
    }
    return depth;
}

template<typename PayloadType>
uint64_t RBTree<PayloadType>::getBlackDepth() {
    uint64_t blackDepth = 0;
    RBNode<PayloadType>* currentNode = root;
    while(currentNode != nullptr) {
        if (currentNode->getColour() == RBColour::BLACK) {
            blackDepth++;
        }
        currentNode = currentNode->getRightChild();
    }
    return blackDepth;
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterDelete(RBNode<PayloadType>* problemNode) {
    RBNode<PayloadType>* currentNode = problemNode;

    while ( currentNode != nullptr ) {
        RBNode<PayloadType>* cnFather = currentNode->getFather();
        RBNode<PayloadType>* cnBrother = currentNode->getPayload() > cnFather->getPayload() ? cnFather->getLeftChild() : cnFather->getRightChild(); 
        if ( cnBrother->getColour() == RBColour::BLACK) {
            rebalanceAfterDeleteBB(problemNode);
            if (cnFather->getColour() == RBColour::RED) {
                cnFather->changeColour(RBColour::BLACK);
                return;
            }
            currentNode = cnFather;

        } else {
            rebalanceAfterDeleteRB(problemNode); 
            return;
        }
    }
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterDeleteBB(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* rnLeftChild = relativeNode->getLeftChild();
    RBNode<PayloadType>* rnRightChild = relativeNode->getRightChild();
    RBNode<PayloadType>* rnFather = relativeNode->getFather();
    RBNode<PayloadType>* rnBrother = relativeNode->getPayload() > rnFather->getPayload() ? rnFather->getLeftChild() : rnFather->getRightChild(); 

    if ( rnLeftChild->getColour() == RBColour::RED || rnRightChild->getColour() == RBColour::RED) {
        RBNode<PayloadType>* redChild = rnLeftChild->getColour() == RBColour::RED ? rnLeftChild : rnRightChild; 
        rnBrother->changeColour(rnFather->getColour());        
        rnFather->changeColour(RBColour::BLACK);
        redChild->changeColour(RBColour::BLACK);
        if ( rnLeftChild->getColour() == RBColour::RED ) {
            rotateLeft(rnFather);
        } else {
            rotateRight(rnFather);
        }

    } else {
        rnBrother->changeColour(RBColour::RED);
    } 
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterDeleteRB(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* cnFather = relativeNode->getFather();
    RBNode<PayloadType>* cnBrother = relativeNode->getPayload() > cnFather->getPayload() ? cnFather->getLeftChild() : cnFather->getRightChild(); 

    cnFather->changeColour(RBColour::RED);
    cnBrother->changeColour(RBColour::BLACK);
    if ( cnBrother->getPayload() > relativeNode->getPayload() ){
        rotateLeft(cnFather); 
    } else {
        rotateRight(cnFather);
    }
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterInsert(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* currentNode = relativeNode;
    while ( currentNode != nullptr ) {
        RBNode<PayloadType>* rnFather = currentNode->getFather();
        RBNode<PayloadType>* rnGrandFather;
        RBNode<PayloadType>* uncle;

        if( rnFather == nullptr) {
            currentNode->changeColour(RBColour::BLACK);
            return;
        }

        if ( rnFather->getFather() != nullptr ) {
            rnGrandFather = rnFather->getFather();
            uncle = *rnFather->getPayload() < *currentNode->getPayload() ? rnGrandFather->getLeftChild() : rnGrandFather->getRightChild();
        } 

        if( rnFather->getColour() == RBColour::RED) {
            if( uncle == nullptr || uncle->getColour() == RBColour::BLACK ) {
                rebalanceAfterInsertRotateCases(currentNode);
            } else {
                rebalanceAfterInsertRepainting(currentNode);
            }
            currentNode = rnGrandFather;
        } else {
            return;
        }
    }
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterInsertRotateCases(RBNode<PayloadType>* relativeNode){
    RBNode<PayloadType>* rnFather = relativeNode->getFather();
    RBNode<PayloadType>* rnGrandFather = rnFather->getFather();

    if ( *relativeNode->getPayload() < *rnFather->getPayload() && *rnFather->getPayload() > *rnGrandFather->getPayload() ) {
        rotateLeft(rnFather);
        rotateRight(rnGrandFather);
    } else if (*relativeNode->getPayload() > *rnFather->getPayload() && *rnFather->getPayload() < *rnGrandFather->getPayload()) {
        rotateLeft(rnFather);
        rotateLeft(rnGrandFather);
    } else if ( *relativeNode->getPayload() < *rnFather->getPayload() &&  *rnFather->getPayload() < *rnGrandFather->getPayload()) {
        rotateRight(rnGrandFather);
    } else if (*relativeNode->getPayload() > *rnFather->getPayload() &&  *rnFather->getPayload() > *rnGrandFather->getPayload()) {
        rotateLeft(rnGrandFather); 
    }
    rnGrandFather->changeColour(RBColour::RED);
    rnFather->changeColour(RBColour::BLACK);
}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceAfterInsertRepainting(RBNode<PayloadType>* relativeNode){
    RBNode<PayloadType>* rnFather = relativeNode->getFather();
    RBNode<PayloadType>* rnGrandfather;
    RBNode<PayloadType>* rnUncle;

    if (rnFather == nullptr) {
        relativeNode->changeColour(RBColour::BLACK);
        return;
    } else {
        rnGrandfather = rnFather->getFather();
    }

    if ( rnGrandfather == nullptr ) {
        rnUncle = nullptr;
    } else {
        rnUncle = *rnFather->getPayload() < *relativeNode->getPayload() ? rnGrandfather->getLeftChild() : rnGrandfather->getRightChild();
        rnUncle->changeColour(RBColour::BLACK);
    }

    rnFather->changeColour(RBColour::BLACK);

    if( rnGrandfather != nullptr && rnGrandfather->getFather() != nullptr ) {
        rnGrandfather->changeColour(RBColour::RED);
    }

}

template<typename PayloadType>
void RBTree<PayloadType>::rotateLeft(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* rightNode = relativeNode->getRightChild();
    RBNode<PayloadType>* rnFather = relativeNode->getFather();

    relativeNode->setRightChild(rightNode->getLeftChild());
    rightNode->setLeftChild(relativeNode);
    rightNode->getLeftChild()->setFather(rightNode);
    rightNode->setFather(rnFather);
    relativeNode->getRightChild()->setFather(relativeNode);
    
    if ( rnFather != nullptr) {
        if ( *rightNode->getPayload() > *rnFather->getPayload() ) {
            rnFather->setRightChild(rightNode);
        } else {
            rnFather->setLeftChild(rightNode);
        }
    } else {
        root = rightNode;
        rightNode->changeColour(RBColour::BLACK);
    }
    
    rightNode->setFather(rnFather);
    
}

template<typename PayloadType>
void RBTree<PayloadType>::rotateRight(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* leftNode = relativeNode->getLeftChild(); 
    RBNode<PayloadType>* rnFather = relativeNode->getFather();
    relativeNode->setLeftChild(leftNode->getRightChild());

    leftNode->setRightChild(relativeNode);
    leftNode->getRightChild()->setFather(leftNode);
    leftNode->setFather(rnFather); 
    relativeNode->getLeftChild()->setFather(relativeNode);
    
    if ( rnFather != nullptr) {
        if ( *leftNode->getPayload() > *rnFather->getPayload() ) {
            rnFather->setRightChild(leftNode);
        } else {
            rnFather->setLeftChild(leftNode);
        } 
    } else {
        root = leftNode;
        leftNode->changeColour(RBColour::BLACK);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename NodeType>
RBNode<NodeType>::RBNode(NodeType* payload, RBColour colour, RBNode<NodeType>* father, RBNode<NodeType>* leftChild, RBNode<NodeType>* rightChild) {
    this->father = father;
    this->payload = payload;
    this->colour = colour;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

template<typename NodeType>
RBNode<NodeType>* RBNode<NodeType>::getLeftChild() {
    return leftChild;
}

template<typename NodeType>
RBNode<NodeType>* RBNode<NodeType>::getRightChild() {
    return rightChild;
}

template<typename NodeType>
NodeType* RBNode<NodeType>::getPayload() {
    return payload;
}

template<typename NodeType>
RBNode<NodeType>* RBNode<NodeType>::getFather() {
    return father;
}

template<typename NodeType>
RBColour RBNode<NodeType>::getColour() {
    return colour;
}

template<typename NodeType>
void RBNode<NodeType>::setLeftChild(RBNode<NodeType>* child) {
    leftChild = child;
}

template<typename NodeType>
void RBNode<NodeType>::setRightChild(RBNode<NodeType>* child) {
    rightChild = child;
}

template<typename NodeType>
void RBNode<NodeType>::setFather(RBNode<NodeType>* newFather) {
    father = newFather;
}

template<typename NodeType>
void RBNode<NodeType>::setPayload(NodeType* newPayload) {
    payload = newPayload;
}

template<typename NodeType>
void RBNode<NodeType>::changeColour(RBColour colour) {
    this->colour = colour;
}

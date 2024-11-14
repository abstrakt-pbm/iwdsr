#pragma once

enum RBColour : bool {
    RED,
    BLACK
};


template<typename NodeType>
class RBNode {
    private:
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
    private:
    RBNode<PayloadType>* root;
    void rotateRight(RBNode<PayloadType>* relativeNode);
    void rotateLeft(RBNode<PayloadType>* relativeNode);
    void rebalanceWithRotates(RBNode<PayloadType>* relativeNode);
    void rebalanceWithoutRotates(RBNode<PayloadType>* realativeNode);

    public:
    RBTree();
    void insert(PayloadType*);
    void destroy(PayloadType*);
    void find(PayloadType);
    
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
        root->setRightChild(new RBNode<PayloadType>(nullptr, RBColour::BLACK ,root));
        return;
    }

    RBNode<PayloadType>* currentNode = root;
    while(currentNode->getPayload() != nullptr){
        if (*(currentNode->getPayload()) < *value) {
            currentNode = currentNode->getRightChild();
        } else {
            currentNode = currentNode->getLeftChild();
        }
    }
    currentNode->setPayload(value);
    currentNode->changeColour(RBColour::RED);
    RBNode<PayloadType>* cnFather = currentNode->getFather(); 
    if( cnFather->getColour() == RBColour::RED) {
        RBNode<PayloadType>* uncle = *(cnFather->getFather()->getPayload()) < *cnFather->getPayload() ? cnFather->getFather()->getLeftChild() : cnFather->getFather()->getRightChild();
        if( uncle == nullptr || uncle->getColour() == RBColour::BLACK) {
            rebalanceWithRotates(currentNode);
        } else {
            rebalanceWithoutRotates(currentNode);
        }
    }
}

template<typename PayloadType>
void RBTree<PayloadType>::destroy(PayloadType* key) {

}

template<typename PayloadType>
void RBTree<PayloadType>::rebalanceWithRotates(RBNode<PayloadType>* relativeNode){
    RBNode<PayloadType>* currentNode = relativeNode;
    while ( currentNode->getFather() != nullptr ) {
        RBNode<PayloadType>* currentNodeFather = currentNode->getFather();
        RBNode<PayloadType>* currentNodeGrandfather = currentNodeFather->getFather();
        RBNode<PayloadType>* currentNodeUncle = *currentNodeFather->getPayload() < *currentNode->getPayload() ? currentNodeFather->getLeftChild() : currentNodeFather->getRightChild();

        currentNodeFather->changeColour(RBColour::BLACK);
        currentNodeUncle->changeColour(RBColour::BLACK);

        if( currentNodeGrandfather != nullptr && currentNodeGrandfather->getFather() != nullptr ) {
            currentNodeGrandfather->changeColour(RBColour::RED);
            currentNode = currentNodeGrandfather;
        }
    }
}



template<typename PayloadType>
void RBTree<PayloadType>::rebalanceWithoutRotates(RBNode<PayloadType>* relativeNode){

}

template<typename PayloadType>
void RBTree<PayloadType>::rotateLeft(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* rightNode = relativeNode->getRightChild();
    RBNode<PayloadType>* rnFather = relativeNode->getFather;

    relativeNode->setRightChild(rightNode->getLeftChild());

    rightNode->setLeftChild(relativeNode);
    rightNode->getLeftChild()->setFather(rightNode);
    rightNode->setFather(rnFather);

    relativeNode->getRightChild()->setFather(rnFather) = relativeNode;

    if ( rnFather != nullptr) {
        rnFather->setRightChild(rightNode);
    } else {
        rightNode->setColour(RBColour::BLACK);
    }
    rightNode->setFather(rnFather);

}

template<typename PayloadType>
void RBTree<PayloadType>::rotateRight(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* leftNode = relativeNode->getLeftChild(); 
    RBNode<PayloadType>* rnFather = relativeNode->getFather();

    relativeNode->setLeftChild(leftNode->rightChild());

    leftNode->setRightChild(relativeNode);
    leftNode->getRightChild()->setFather(leftNode);
    leftNode->setFather(rnFather); 

    relativeNode->getLeftChild()->setFather(relativeNode);

    if ( rnFather != nullptr) {
        leftNode->setFather(rnFather);
    } else {
        leftNode->setColour(RBColour::BLACK);
    }
}

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
    rightChild = newFather;
}

template<typename NodeType>
void RBNode<NodeType>::setPayload(NodeType* newPayload) {
    payload = newPayload;
}

template<typename NodeType>
void RBNode<NodeType>::changeColour(RBColour colour) {
    this->colour = colour;
}

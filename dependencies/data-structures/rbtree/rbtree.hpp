#pragma once

enum RBColour : bool {
    RED,
    BLACK
};

template<typename NodeType>
class RBNode {
    friend RBTree;
    private:
    RBNode* father;
    RBNode* leftChild;
    RBNode* rightChild;
    RBColour colour; 
    NodeType payload;

    public: 
    RBNode(RBNode* father = nullptr, NodeType payload = nullptr, RBColour colour = RBColour::BLACK, RBNode* leftChild = nullptr, RBNode* rightChild = nullptr);
    void setChild(RBNode* child);
    void changeColour(RBColour colour);
    NodeType getPayload();

};

template<typename PayloadType>
class RBTree {
    private:
    RBNode<PayloadType>* root;
    void rotateRight(RBNode<PayloadType>* relativeNode);
    void rotateLeft(RBNode<PayloadType>* relativeNode);
    bool needRebalance();
    public:
    RBTree();
    void insert(PayloadType);
    void destroy(PayloadType);
    void find(PayloadType);
    
};

template<typename PayloadType>
RBTree<PayloadType>::RBTree() {
    root = new RBNode<PayloadType>();
}

template<typename PayloadType> 
void insert(PayloadType value) {
    RBNode<PayloadType>* currentRBNode = root;
    while ( currentRBNode->leftChild != nullptr) {
        currentRBNode = currentRBNode->payload < value ? currentRBNode->rightChild : currentRBNode->leftChild;  
    }

    currentRBNode->leftChild = new RBNode<PayloadType>(value, RBColour::RED);

    switch ( currentRBNode->colour ) {
        case RED: {
            RBNode* uncle = currentRBNode->father->rightChild;
            if ( uncle->colour == RBColour::RED ) {
                currentRBNode->changeColour(RBColour::BLACK);
                uncle->changeColour(RBColour::BLACK);
                currentRBNode->father->changeColour(RBColour::RED);
            } else {
                
            }
            break;
        };
        case BLACK: {
            return;
        }
    }

}

template<typename PayloadType>
void RBTree<PayloadType>::rotateLeft(RBNode<PayloadType>* relativeNode) {
    RBNode<PayloadType>* rnFather = relativeNode->father;
    RBNode<PayloadType>* rnRight = relativeNode->rightChild;
    if ( rnFather != nullptr ) {
        rnFather->rightChild = rnRight;
        rnFather->rightChild->father = rnFather;
        relativeNode->rightChild = rnRight->leftChild;
        relativeNode->rightChild->father = relativeNode;
        rnRight->leftChild = relativeNode;
        rnRight->leftChild = rnRight;

    } else {
        
    }
}

template<typename PayloadType>
void RBTree<PayloadType>::rotateRight(RBNode<PayloadType>* relativeNode) {
    
}

template<typename NodeType>
RBNode<NodeType>::RBNode(RBNode* father, NodeType payload, RBColour colour, RBNode* leftChild, RBNode* rightChild) {
    this->father = father;
    this->payload = payload;
    this->colour = colour;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

template<typename NodeType>
void RBNode<NodeType>::setChild(RBNode* child) {
    this->child = child;
    colour = RBColour::RED;
}

template<typename NodeType>
void RBNode<NodeType>::changeColour(RBColour colour) {
    this->colour = colour;
}

template<typename NodeType>
NodeType RBNode<NodeType>::getPayload() {
    return payload;
}
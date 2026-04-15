#include <queue>
#include <unordered_set>
#include "node.hpp"


void Node::deleteTree(Node * t) {
    std::unordered_set<Node *> delete_set;
    deleteTreeRecursive_(t,delete_set);
}


void Node::deleteTreeRecursive_(Node * t, std::unordered_set<Node *>& visited) {
    if (!t || visited.count(t)) {
        return; 
    }
    visited.insert(t);
    deleteTreeRecursive_(t->down(), visited);
    deleteTreeRecursive_(t->up(), visited);
    delete t;
}

void Node::updateCallPrice(Node * n_price, double strike) {
    setValue(std::max(n_price->value() - strike,0.));
}

void Node::updatePutPrice(Node * n_price, double strike) {
    setValue(std::max(strike - n_price->value(),0.));
}

Node * Node::createTree(int tree_depth) {
    if (tree_depth <= 0) {
        return nullptr;
    }
    std::queue<Node *> previous_level_queue;

    Node * root = new Node(0,0);
    previous_level_queue.push(root);

    for(int i = 1; i < tree_depth; i++) {
        std::queue<Node *> current_level_queue;

        for (int j = 0; j <= i; j++) {
            Node * node = new Node(i * (i + 1) / 2 + j, i);
            node->setValue(0);
            current_level_queue.push(node);
        }

        std::queue<Node *> temp_children = current_level_queue;

        while (!previous_level_queue.empty()) {
            Node * parent = previous_level_queue.front();
            previous_level_queue.pop();

            Node * downChild = temp_children.front();
            parent->setDown(downChild);
            downChild->setPrevUp(parent);

            temp_children.pop();

            Node * upChild = temp_children.front();
            parent->setUp(upChild);
            upChild->setPrevDown(parent);
        }
        previous_level_queue = current_level_queue;
    }
    return root;
}
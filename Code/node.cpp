#include <queue>
#include <set>
#include "node.hpp"


void Node::deleteTree(Node * t) {
    std::set<Node * > delete_set;
    deleteTreeRecursive_(t,delete_set);
}


void Node::deleteTreeRecursive_(Node * t, std::set<Node*>& visited) {
    if (!t || visited.count(t)) {
        return; 
    }
    visited.insert(t);
    deleteTreeRecursive_(t->down(), visited);
    deleteTreeRecursive_(t->up(), visited);
    delete t;
}


Node * Node::createTree(int tree_depth, double init_value, double d_value, double u_value) {
    if (tree_depth <= 0) {
        return nullptr;
    }
    std::queue<Node *> previous_level_queue;

    Node * root = new Node(0,0);
    root->setValue(init_value);
    previous_level_queue.push(root);

    for(int i = 1; i < tree_depth; i++) {
        std::queue<Node *> current_level_queue;

        for (int j = 0; j <= i; j++) {
            Node * node = new Node(i * (i + 1) / 2 + j, i);
            current_level_queue.push(node);
        }

        std::queue<Node *> temp_children = current_level_queue;

        while (!previous_level_queue.empty()) {
            Node * parent = previous_level_queue.front();
            previous_level_queue.pop();

            parent->setDown(temp_children.front());
            temp_children.pop();
            parent->down()->setValue(parent->value() * d_value);

            parent->setUp(temp_children.front());
            parent->up()->setValue(parent->value() * u_value);
        }
        previous_level_queue = current_level_queue;
    }
    return root;
}
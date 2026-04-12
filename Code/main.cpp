#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include "node.hpp"
#include "crr.hpp"

int main(int argc, char * argv[]) {
    if (argc != 5) {
        std::cerr << "USAGE : " << argv[0] << " DEPTH INIT VOL TIME\n";
        return -1;
    }
    int depth = std::stoi(argv[1]);
    double init_value = std::stod(argv[2]);
    double vol_value = std::stod(argv[3]);
    float time_value = std::stof(argv[4]);

    double d = CRR::calculateDownFactor(vol_value, time_value, depth);
    double u = CRR::calculateUpFactor(vol_value, time_value, depth);

    Node * tree_price = Node::createTree(depth);
    Node * tree_premium = Node::createTree(depth);

    CRR::evaluateLeafNodes(tree_price, tree_premium, Node::Call, init_value);

    std::ofstream file_price("arbre_price.dot");
    if (file_price) {
        CRR::generateDotFile(tree_price, file_price);
        file_price.close();
    }
    std::ofstream file_premium("arbre_premium.dot");
    if (file_premium) {
        CRR::generateDotFile(tree_premium, file_premium);
        file_premium.close();
    }

    Node::deleteTree(tree_price);
    Node::deleteTree(tree_premium);
    return 0;
}
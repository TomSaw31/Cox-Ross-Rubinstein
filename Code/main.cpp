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
    Node * root = Node::createTree(depth, init_value, d, u);

    std::ofstream file("arbre.dot");
    if (file) {
        CRR::generateDotFile(root, file);
        file.close();
    }
    
    Node::deleteTree(root);
    return 0;
}
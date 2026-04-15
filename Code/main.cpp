#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include "node.hpp"
#include "crr.hpp"

int main(int argc, char * argv[]) {
    if (argc < 7 || argc > 8) {
        std::cerr << "USAGE : " << argv[0] << " DEPTH INIT STRIKE VOL TIME RISK_FREE_RATE\n";
        return -1;
    }
    int depth = std::stoi(argv[1])+1;
    double init_value = std::stod(argv[2]);
    double strike = std::stod(argv[3]);
    double vol_value = std::stod(argv[4]);
    double time_value = std::stof(argv[5]);
    double risk_free_rate = std::stof(argv[6]);

    double d = CRR::calculateDownFactor(vol_value, time_value, depth);
    double u = CRR::calculateUpFactor(vol_value, time_value, depth);

    // TEST MODE
    if (argc == 8 && std::stoi(argv[7]) == 1) {
        Node * tree_price = Node::createTree(depth);
        Node * tree_premium = Node::createTree(depth);
        CRR::fillPrices(tree_price,init_value,u,d);
        CRR::evaluateLeafNodes(tree_price, tree_premium, Node::Call, strike);
        CRR::backwardInduction(tree_premium, u, d, risk_free_rate, time_value, depth);
        printf("%f",tree_premium->value());
        return 1;
    }

    printf("Creating Trees\n");
    Node * tree_price = Node::createTree(depth);
    Node * tree_premium = Node::createTree(depth);

    printf("Filling Prices\n");
    CRR::fillPrices(tree_price,init_value,u,d);

    printf("Evaluating Leaves\n");
    CRR::evaluateLeafNodes(tree_price, tree_premium, Node::Call, strike);

    printf("Backward Induction\n");
    CRR::backwardInduction(tree_premium, u, d, risk_free_rate, time_value, depth);

    printf("Generating PDF\n");
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
    CRR::generatePDFs();
    
    printf("------------------------\n");
    printf("Option Price : %f\n",tree_premium->value());
    printf("------------------------\n");

    Node::deleteTree(tree_price);
    Node::deleteTree(tree_premium);

    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <limits>
#include <iomanip>

#include "node.hpp"
#include "crr.hpp"

enum OptionType {Call = 0, Put = 1};

struct Params {
    int depth;
    double S0;
    double K;
    double sigma;
    double T;
    double r;
    OptionType type;
};

struct Model {
    Node * tree_price;
    Node * tree_premium;
    double d;
    double u;
};

void log(const std::string& msg, bool test_mode) {
    if (!test_mode) std::cout << msg << std::endl;
}

// Model : Cox-Ross-Rubinstein binomial tree
// Inputs : depth, S0, K, sigma, T, r, type
int main(int argc, char * argv[]) {
    if (argc < 8 || argc > 9) {
        std::cerr << "USAGE : " << argv[0] << " depth S0 K sigma T r type [TEST_MOD]\n";
        return -1;
    }

    // ----------------- PARAMETERS ----------------- //
    Params params {
        std::stoi(argv[1]),
        std::stod(argv[2]),
        std::stod(argv[3]),
        std::stod(argv[4]),
        std::stod(argv[5]),
        std::stod(argv[6]),
        (std::stoi(argv[7]) == 0) ? Call : Put
    };
    
    bool test_mode = (argc == 9 && std::stoi(argv[8]) == 1);
    
    Model model {
        nullptr,
        nullptr,
        CRR::calculateDownFactor(params.sigma, params.T, params.depth),
        CRR::calculateUpFactor(params.sigma, params.T, params.depth)
    };

    // ----------------- MODEL CRR ----------------- //
    log("Creating Trees",test_mode);
    model.tree_price = Node::createTree(params.depth + 1);
    model.tree_premium = Node::createTree(params.depth + 1);

    log("Filling Prices",test_mode);
    CRR::fillPrices(model.tree_price,params.S0,model.u,model.d);

    log("Evaluating Leaves",test_mode);
    CRR::evaluateLeafNodes(model.tree_price, model.tree_premium, params.type, params.K);

    log("Backward Induction",test_mode);
    CRR::backwardInduction(model.tree_premium, model.u, model.d, params.r, params.T, params.depth);

    // ----------------- PDF ----------------- //
    if (!test_mode) {
        std::cout << "Generating PDF" << std::endl;
        std::ofstream file_price("arbre_price.dot");
        if (file_price) {
            CRR::generateDotFile(model.tree_price, file_price);
            file_price.close();
        }
        std::ofstream file_premium("arbre_premium.dot");
        if (file_premium) {
            CRR::generateDotFile(model.tree_premium, file_premium);
            file_premium.close();
        }
        CRR::generatePDFs();
    }

    // ----------------- OUTPUT ----------------- //
    log("---------------------------------",test_mode);
    if (!test_mode) std::cout << "Option Price : " << std::setprecision(std::numeric_limits<double>::max_digits10) 
        << model.tree_premium->value() << std::endl;
    log("---------------------------------",test_mode);
    if (test_mode) std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) 
        << model.tree_premium->value() << std::endl;

    // ----------------- MEMORY CLEANING ----------------- //
    Node::deleteTree(model.tree_price);
    Node::deleteTree(model.tree_premium);

    return 0;
}
#include <fstream>
#include <iomanip>
#include <set>
#include <cmath>
#include "crr.hpp"



void exportDot(const Node * t, std::ostream& out, std::set<const Node*>& visited) {
    if (t == nullptr || visited.count(t)) return;
    visited.insert(t);

    out << std::fixed << std::setprecision(2);

    out << "  \"" << t << "\" [label=\"" << t->value() << "\"];\n";

    if (t->up()) {
        out << "  \"" << t << "\" -> \"" << t->up() << "\" [fontcolor=\"#00aa00\", color=\"#00aa00\"];\n";
        exportDot(t->up(), out, visited);
    }

    if (t->down()) {
        out << "  \"" << t << "\" -> \"" << t->down() << "\" [fontcolor=\"#aa0000\", color=\"#aa0000\"];\n";
        exportDot(t->down(), out, visited);
    }
}


void CRR::generateDotFile(const Node * root, std::ostream& file) {
    file << "digraph G {\n";
    file << "splines = line;\n";
    file << "rankdir = LR;\n";
    file << "nodesep = 0.25;\n";
    file << "node [shape=circle, width = 0.8, fontsize = 10, fixedsize = true];\n";
    std::set<const Node * > visited;
    exportDot(root, file, visited);
    file << "}\n";
}

void evaluateLeafNodesRecursive_(Node * t_price, Node * t_premium, Node::OptionType type, double strike, std::set<Node*>& visited) {
    if (!t_price || !t_premium || visited.count(t_price)) {
        return; 
    }
    visited.insert(t_price);
    if(t_price->up() == nullptr) {
        switch (type) {
            case Node::OptionType::Call:
                t_premium->updateCallPrice(t_price, t_premium, strike); 
                printf("Call : %d\n",t_price->id());
                break;
            case Node::OptionType::Put:
                t_premium->updatePutPrice(t_price, t_premium, strike);
                printf("Put : %d\n",t_price->id());
                break;
        }
    } else {
        evaluateLeafNodesRecursive_(t_price->up(), t_premium->up(), type, strike, visited);
        evaluateLeafNodesRecursive_(t_price->down(), t_premium->down(), type, strike, visited);
    }
}

void CRR::evaluateLeafNodes(Node * t_price, Node * t_premium, Node::OptionType type, double strike) {
    std::set<Node *> visited_set;
    evaluateLeafNodesRecursive_(t_price, t_premium, type, strike, visited_set);
}

double CRR::calculateDownFactor(double volatility, float time_period, int depth) {
    return exp(-volatility * sqrt(time_period / (depth - 1)));
}

double CRR::calculateUpFactor(double volatility, float time_period, int depth) {
    return exp(volatility * sqrt(time_period / (depth - 1)));
}
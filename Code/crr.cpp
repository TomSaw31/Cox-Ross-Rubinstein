#include <fstream>
#include <iomanip>
#include <set>
#include <cmath>
#include "crr.hpp"



void CRR::fillPrices(Node * node, double currentPrice, double u, double d) {
    if (node == nullptr) {
        return;
    }
    node->setValue(currentPrice);
    fillPrices(node->up(), currentPrice * u, u, d);
    fillPrices(node->down(), currentPrice * d, u, d);
}

void CRR::evaluateLeafNodes(Node * t_price, Node * t_premium, Node::OptionType type, double strike) {
    Node * iter = t_price;
    Node * iter2 = t_premium;

    while(iter->down() != nullptr) {
        iter = iter->down();
        iter2 = iter2->down();
    }

    while(iter != nullptr) {
        switch (type) {
            case Node::OptionType::Call:
                iter2->updateCallPrice(iter, strike);
                break;
            case Node::OptionType::Put:
                iter2->updatePutPrice(iter, strike);
                break;
        }
        if (iter->prevUp() != nullptr) {
            iter = iter->prevUp()->up();
            iter2 = iter2->prevUp()->up();
        } else {
            break;
        }
    }
}

double getNodeMeanValue(Node * node, double R, double p, double q) {
    if (!node || !node->down() || !node->up()) {
        std::cerr << "Invalid Node to get mean value\n";
        return -1.;
    }
    return (p * node->up()->value() + q * node->down()->value()) / R;
}

void CRR::backwardInduction(Node * node, double u, double d, double r, double time_value, int depth) {
    double dt = time_value / (depth - 1);
    double R = exp(r * dt);
    double p = (R - d)/(u - d);
    double q = (u - R)/(u - d);
    Node * iter = node;
    while(iter->down() != nullptr) {
        iter = iter->down();
    }
    iter = iter->prevUp();
    while(iter->prevUp() != nullptr) {
        Node * parent = iter->prevUp();
        while(iter->up()->prevUp() != nullptr) {
            iter->setValue(getNodeMeanValue(iter, R, p, q));
            iter = iter->up()->prevUp();
        }
        iter->setValue(getNodeMeanValue(iter, R, p, q));
        iter = parent;
    }
    iter->setValue(getNodeMeanValue(iter, R, p, q));
}

double CRR::calculateDownFactor(double volatility, double time_period, int depth) {
    return exp(-volatility * sqrt(time_period / (depth - 1)));
}

double CRR::calculateUpFactor(double volatility, double time_period, int depth) {
    return exp(volatility * sqrt(time_period / (depth - 1)));
}

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

void CRR::generatePDFs() {
    int status1 = std::system("dot -Tpdf arbre_price.dot -o arbre_price.pdf");
    int status2 = std::system("dot -Tpdf arbre_premium.dot -o arbre_premium.pdf");
    if (status1 != 0 || status2 != 0) {
        std::cerr << "Erreur lors de l'exécution de Graphviz. Vérifie que 'dot' est installé et dans ton PATH." << std::endl;
    }
}
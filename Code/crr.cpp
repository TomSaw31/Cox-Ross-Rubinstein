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


double CRR::calculateDownFactor(double volatility, float time_period, int depth) {
    return exp(-volatility * sqrt(time_period / (depth - 1)));
}


double CRR::calculateUpFactor(double volatility, float time_period, int depth) {
    return exp(volatility * sqrt(time_period / (depth - 1)));
}
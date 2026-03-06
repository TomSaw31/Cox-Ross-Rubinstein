#include <iostream>
#include <string>
#include <fstream>

class Node;

typedef void(*OperateFunctor)(const Node *, void*);

class Node {
public:
    int level;
    int tree_depth;
    Node * left;
    Node * right;

    Node (int level, int tree_depth) {
        this->level = level;
        this->tree_depth = tree_depth;
        left = nullptr;
        right = nullptr;
    }

    ~Node() {
        delete left;
        delete right;
    }

    static Node * create_tree(int level, int tree_depth) {
        if (level > tree_depth) {
            return nullptr;
        }
        Node * node = new Node(level, tree_depth);
        node->left = Node::create_tree(level + 1, tree_depth);
        node->right = Node::create_tree(level + 1, tree_depth);
        
        return node;
    }

    bool is_tree_empty(const Node * t) {
        return t == nullptr;
    }

    void tree_depth_infix(const Node * t, OperateFunctor f, void * environment) {
        if (t == nullptr) {
            return;
        }
        tree_depth_infix(t->left,f,environment);
        f(t,environment);
        tree_depth_infix(t->right,f,environment);
    }

    static void export_dot(const Node* t, std::ostream& out) {
        if (t == nullptr) return;
        if (t->left) {
            out << "  \"" << t << "\" -> \"" << t->left << "\" [label=\"Up\"];\n";
            export_dot(t->left, out);
        }
        if (t->right) {
            out << "  \"" << t << "\" -> \"" << t->right << "\" [label=\"Down\"];\n";
            export_dot(t->right, out);
        }
        out << "  \"" << t << "\" [label=\"Niveau " << t->level << "\"];\n";
    }
};

void print_node(const Node * n, void * env) {
    for(int i = 0; i < n->level; i++) {
        std::cout << "     ";
    }
    std::cout << n->level << "\n";
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cerr << "USAGE : " << argv[0] << " DEPTH\n";
        return -1;
    }
    int depth = std::stoi(argv[1]);
    Node * root = Node::create_tree(0, depth);
    std::cout << "Structure de l'arbre : \n";
    root->tree_depth_infix(root, print_node, nullptr);
    std::ofstream fichier("arbre.dot");
    if (fichier) {
        fichier << "digraph G {\n";
        fichier << "  node [shape=circle];\n";
        Node::export_dot(root, fichier);
        fichier << "}\n";
        fichier.close();
        std::cout << "Fichier 'arbre.dot' généré.\n";
    }
    std::cout << "\n\nNettoyage de la mémoire :" << std::endl;
    delete root; 
    return 0;
}




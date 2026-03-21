#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <iostream>
#include <set>

/**
 * @class Node
 * @brief Représente un noeud dans l'arbre associé du modèle binomial de Cox-Ross-Rubinstein
 *  * Cette classe gère la structure de l'arbre recombinant et stocke les valeurs d'actifs en fonction
 * des hausses et des baisses au cours du temps.
 */
class Node {
private:
    int id_;
    int level_;
    double value_;
    Node * down_;
    Node * up_;

    static void deleteTreeRecursive_(Node * t, std::set<Node*>& visited);

public:
    Node (int id, int level) : id_(id), level_(level), down_(nullptr), up_(nullptr) {}
    

    // --- Getters --- //

    int level() const { return level_; }
    int id() const { return id_; }
    double value() const { return value_; }
    Node * down() const { return down_; }
    Node * up() const { return up_; }


    // --- Setters --- //

    void setLevel(int val) { level_ = val; }
    void setValue(double val) { value_ = val; }
    void setDown(Node * node) { down_ = node; }
    void setUp(Node * node) { up_ = node; }

    /**
     * @brief Calcul et crée récursivement l'arbre binomial
     * @param tree_depth Profondeur totale de l'arbre créé
     * @param init_value Prix de l'actif à l'initialisation (t=0)
     * @param d_value Coefficient de baisse
     * @param u_value Coefficient de hausse
     * @return Pointeur vers la racine de l'arbre créé
     */
    static Node * createTree(int depth, double s0, double d, double u);

    /**
     * @brief Supprime l'arbre dans sa totalité
     * @param t racine de l'arbre à supprimer
     */
    void static deleteTree(Node * t);
};

#endif
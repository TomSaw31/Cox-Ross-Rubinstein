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
    enum OptionType {Call, Put};    

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
     * @brief Crée récursivement l'arbre binomial avec des noeuds initialisés à 0
     * @param tree_depth Profondeur totale de l'arbre créé
     * @return Pointeur vers la racine de l'arbre créé
     */
    static Node * createTree(int depth);

    /**
     * @brief Supprime l'arbre dans sa totalité
     * @param t racine de l'arbre à supprimer
     */
    static void deleteTree(Node * t);

    /**
     * @brief Définit avec la formule du Call la valeur du noeud n_premium en fonction du noeud n_price et de la valeur du strike
     * @param n_price noeud dont la valeur doit être utilisée pour le calcul de la prime
     * @param n_premium noeud dont la valeur va être la valeur de la prime apres execution de la fonction
     * @param strike valeur du strike
     */
    void updateCallPrice(Node * n_price, Node * n_premium, double strike);

    /**
     * @brief Définit avec la formule du Put la valeur du noeud n_premium en fonction du noeud n_price et de la valeur du strike
     * @param n_price noeud dont la valeur doit être utilisée pour le calcul de la prime
     * @param n_premium noeud dont la valeur va être la valeur de la prime apres execution de la fonction
     * @param strike valeur du strike
     */
    void updatePutPrice(Node * n_price, Node * n_premium, double strike);
};

#endif
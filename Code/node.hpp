#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <iostream>
#include <unordered_set>

/**
 * @class Node
 * @brief Représente un noeud dans l'arbre binomial du modèle de Cox-Ross-Rubinstein
 *
 * Cette classe implémente la structure d'un arbre recombining utilisé pour la
 * valorisation d'options financières. Chaque noeud contient :
 * - une valeur de l'actif sous-jacent (ou de l'option selon l'arbre)
 * - des pointeurs vers les noeuds enfants (up/down)
 * - des pointeurs vers les parents (prev_up/prev_down) pour faciliter les parcours
 */
class Node {
private:
    int id_;
    int level_;
    double value_;

    Node * down_;
    Node * up_;
    Node * prev_down_;
    Node * prev_up_;

    static void deleteTreeRecursive_(Node * t, std::unordered_set<Node*>& visited);

public:
    /**
     * @brief Constructeur d'un noeud
     * @param id identifiant unique du noeud
     * @param level niveau dans l'arbre (temps discret)
     */
    Node(int id, int level)
        : id_(id),
          level_(level),
          value_(0.0),
          down_(nullptr),
          up_(nullptr),
          prev_down_(nullptr),
          prev_up_(nullptr) {}

    int level() const { return level_; }
    int id() const { return id_; }

    /**
     * @brief Retourne la valeur stockée dans le noeud
     */
    double value() const { return value_; }

    Node * down() const { return down_; }
    Node * up() const { return up_; }
    Node * prevDown() const { return prev_down_; }
    Node * prevUp() const { return prev_up_; }

    void setLevel(int val) { level_ = val; }
    void setValue(double val) { value_ = val; }
    void setDown(Node * node) { down_ = node; }
    void setUp(Node * node) { up_ = node; }
    void setPrevDown(Node * node) { prev_down_ = node; }
    void setPrevUp(Node * node) { prev_up_ = node; }

    /**
     * @brief Crée un arbre binomial recombinant initialisé à 0
     * @param depth profondeur de l'arbre
     * @return racine de l'arbre
     */
    static Node * createTree(int depth);

    /**
     * @brief Supprime entièrement l'arbre et libère la mémoire
     * @param t racine de l'arbre
     */
    static void deleteTree(Node * t);

    /**
     * @brief Calcule le payoff d'un Call européen au noeud courant
     * @param n_price noeud contenant le prix du sous-jacent
     * @param strike prix d'exercice
     *
     * Formule :
     * max(S - K, 0)
     */
    void updateCallPrice(Node * n_price, double strike);

    /**
     * @brief Calcule le payoff d'un Put européen au noeud courant
     * @param n_price noeud contenant le prix du sous-jacent
     * @param strike prix d'exercice
     *
     * Formule :
     * max(K - S, 0)
     */
    void updatePutPrice(Node * n_price, double strike);
};

#endif
#ifndef __CRR_HPP__
#define __CRR_HPP__

#include <iostream>
#include "node.hpp"

/**
 * @brief Classe permettant d'implémenter la logique du modèle binomial de Cox-Ross-Rubinstein
 */
class CRR {
public:
    /**
     * @brief Calcule les payoffs de l'option aux feuilles de l'arbre
     * @param t_price arbre des prix du sous-jacent
     * @param t_premium arbre des valeurs de l'option
     * @param type type d'option (Call ou Put)
     * @param strike prix d'exercice
     *
     * Initialise les feuilles de l'arbre des primes à partir des prix du sous-jacent.
     */
    static void evaluateLeafNodes(Node * t_price, Node * t_premium, int type, double strike);

    /**
     * @brief Propage les prix dans l'arbre binomial
     * @param node racine de l'arbre
     * @param current_price prix initial de l'actif
     * @param u facteur de hausse
     * @param d facteur de baisse
     *
     * Remplit récursivement les valeurs de l'arbre du sous-jacent.
     */
    static void fillPrices(Node * node, double current_price, double u, double d);

    /**
     * @brief Applique la backward induction pour calculer le prix de l'option
     * @param node racine de l'arbre des primes
     * @param u facteur de hausse
     * @param d facteur de baisse
     * @param r taux sans risque
     * @param time_value maturité de l'option
     * @param depth profondeur de l'arbre
     *
     * Calcule récursivement la valeur de l'option en remontant l'arbre
     * selon l'espérance risque-neutre actualisée.
     */
    static void backwardInduction(Node * node, double u, double d, double r, double time_value, int depth);

    /**
     * @brief Renvoie la valeur du facteur de baisse en fonction des paramètres indiqués
     * @param volatility volatilité de l'actif
     * @param time_period maturité de l'option à valoriser
     * @param depth profondeur de l'arbre
     */
    static double calculateDownFactor(double volatility, double time_period, int depth);

    /**
     * @brief Renvoie la valeur du facteur de hausse en fonction des paramètres indiqués
     * @param volatility volatilité de l'actif
     * @param time_period maturité de l'option à valoriser
     * @param depth profondeur de l'arbre
     */
    static double calculateUpFactor(double volatility, double time_period, int depth);

    /**
     * @brief Calcule la valeur espérée actualisée d'un noeud
     * @param node noeud courant de l'arbre
     * @param R facteur d'actualisation e^{r*dt}
     * @param p probabilité risque-neutre de hausse
     * @param q probabilité risque-neutre de baisse
     * @return valeur actualisée du noeud
     *
     * Utilise la formule :
     * (p * V_up + q * V_down) / R
     */
    static double getNodeMeanValue(Node * node, double R, double p, double q);

    /**
     * @brief Génère un fichier au format dot représentant l'arbre binomial calculé
     * @param root racine de l'arbre à transformer
     * @param file fichier dans lequel écrire le code dot représentant l'arbre
     */
    static void generateDotFile(const Node * root, std::ostream& file);

    /**
     * @brief Génère les fichiers PDF représentant les arbres via Graphviz
     *
     * Convertit les fichiers DOT générés en PDF à l'aide de l'outil dot.
     */
    static void generatePDFs();
};

#endif
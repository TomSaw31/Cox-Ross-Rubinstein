#ifndef __CRR_HPP__
#define __CRR_HPP__

#include <iostream>
#include <set>
#include "node.hpp"

/**
 * @brief Classe permettant d'implémenter la logique du modèle binomial de Cox-Ross-Rubinstein
 */
class CRR {
public:
    /**
     * @brief Génère un fichier au format dot représentant l'arbre binomial calculé
     * @param root racine de l'arbre à transformer
     * @param file fichier dans lequel écrire le code dot représentant l'arbre
     */
    static void generateDotFile(const Node* root, std::ostream& file);

    /**
     * @brief Renvoie la valeur du facteur de baisse en fonction des paramètres indiqués
     * @param volatility volatilité de l'actif
     * @param time_period maturité de l'option à valoriser
     * @param depth profondeur de l'arbre
     */
    static double calculateDownFactor(double volatility, float time_period, int depth);

    /**
     * @brief Renvoie la valeur du facteur de hausse en fonction des paramètres indiqués
     * @param volatility volatilité de l'actif
     * @param time_period maturité de l'option à valoriser
     * @param depth profondeur de l'arbre
     */
    static double calculateUpFactor(double volatility, float time_period, int depth);
};

#endif
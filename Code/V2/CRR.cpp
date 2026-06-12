#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

enum class OptionType {Call, Put};
enum class ExerciseType {European, American};

/**
 * Calcule le prix d'une option via le modèle CRR (Arbre Binomial)
 * @param S0    : Prix actuel du sous-jacent
 * @param K     : Prix d'exercice (Strike)
 * @param r     : Taux d'intérêt sans risque
 * @param sigma : Volatilité du sous-jacent
 * @param T     : Temps jusqu'à l'échéance en années
 * @param N     : Profondeur de l'arbre
 * @param oType : Type d'option
 * @param eType : Type d'exercice
 * @return      : Le prix estimé de l'option
 */
double crrOptionPrice(double S0, double K, double r, double sigma, double T, int N, OptionType oType, ExerciseType eType) {
    double dt = T / N;
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp(r * dt) - d) / (u - d);
    double discount = std::exp(-r * dt);

    if (p <= 0.0 || p >= 1.0) {
        std::cerr << "Problème d'arbitrage\n";
    }

    std::vector<double> optionValues(N + 1);

    for (int i = 0; i <= N; i++) {
        double ST = S0 * std::pow(u, N - i) * std::pow(d, i);
        if (oType == OptionType::Call) {
            optionValues[i] = std::max(0., ST - K);
        } else {
            optionValues[i] = std::max(0., K - ST);
        }
    }

    for (int j = N - 1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            double continuation = discount * (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
            if (eType == ExerciseType::American) {
                double S_current = S0 * std::pow(u, j - i) * std::pow(d, i);
                double value = (oType == OptionType::Call) ? std::max(0.0, S_current - K) : std::max(0.0, K - S_current);
                optionValues[i] = std::max(continuation, value);
            } else {
                optionValues[i] = continuation;
            }
        }
    }
    return optionValues[0];
}

int main() {
    double S0 = 105.0;
    double K = 100.0;
    double r = 0.05;
    double sigma = 0.20;
    double T = 1.0;
    int N = 500;

    // Calculs
    double eCall = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Call, ExerciseType::European);
    double aCall = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Call, ExerciseType::American);
    double ePut  = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Put,  ExerciseType::European);
    double aPut  = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Put,  ExerciseType::American);

    // Affichage des résultats
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "--- Résultats du Modèle CRR (N = " << N << ") ---\n";
    std::cout << "European Call : " << eCall << "\n";
    std::cout << "American Call : " << aCall << "\n";
    std::cout << "European Put  : " << ePut << "\n";
    std::cout << "American Put  : " << aPut  << "\n";

    return 0;
}
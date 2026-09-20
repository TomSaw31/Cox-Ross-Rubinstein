#include "CRR.hpp"

enum class OptionType {Call, Put};
enum class ExerciseType {European, American};

/**
 * Calculates the option price using the CRR model (Binomial Tree)
 * @param S0    : Current price of the underlying asset
 * @param K     : Strike price
 * @param r     : Risk-free interest rate
 * @param sigma : Volatility of the underlying asset
 * @param T     : Time to maturity in years
 * @param N     : Tree depth
 * @param oType : Option type
 * @param eType : Exercise type
 * @return      : The estimated option price
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

// int main() {
//     double S0 = 50.0;
//     double K = 50.0;
//     double r = 0.04;
//     double sigma = 0.20;
//     double T = 1.0;
//     int N = 10;

//     // Calculations
//     double eCall = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Call, ExerciseType::European);
//     double aCall = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Call, ExerciseType::American);
//     double ePut  = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Put,  ExerciseType::European);
//     double aPut  = crrOptionPrice(S0, K, r, sigma, T, N, OptionType::Put,  ExerciseType::American);

//     // Results
//     std::cout << std::fixed << std::setprecision(4);
//     std::cout << "--- Résultats du Modèle CRR (N = " << N << ") ---\n";
//     std::cout << "European Call : " << eCall << "\n";
//     std::cout << "American Call : " << aCall << "\n";
//     std::cout << "European Put  : " << ePut << "\n";
//     std::cout << "American Put  : " << aPut  << "\n";

//     return 0;
// }
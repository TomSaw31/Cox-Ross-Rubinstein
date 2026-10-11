#include "CRR.hpp"

double CRR::crrOptionPrice() {
    updateModel();
    for(int i = 0; i < N + 1; ++i) {
        optionValues.push_back(0);
    }

    double p = (std::exp(r * dt) - d) / (u - d);
    double discount = std::exp(-r * dt);

    if (p <= 0.0 || p >= 1.0) {
        std::cerr << "ERROR : Arbitrage\n";
        return -1.;
    }
    for (int i = 0; i < N + 1; ++i) {
        double ST = S0 * std::pow(u, N - i) * std::pow(d, i);
        if (oType == OptionType::CALL) {
            optionValues[i] = std::max(0., ST - K);
        } else {
            optionValues[i] = std::max(0., K - ST);
        }
    }
    double continuation;
    for (int j = N - 1; j >= 0; --j) {
        for (int i = 0; i <= j; ++i) {
            continuation = discount * (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
            if (eType == ExerciseType::EUROPEAN) {
                optionValues[i] = continuation;
                
            } else {
                double S_current = S0 * std::pow(u, j - i) * std::pow(d, i);
                double value = (oType == OptionType::CALL) ? std::max(0.0, S_current - K) : std::max(0.0, K - S_current);
                optionValues[i] = std::max(continuation, value);
            }
        }
    }

    riskNeutral = p;
    result = optionValues[0];
    computed = true;
    return optionValues[0];
}

void CRR::updateModel() {
    dt = T / N;
    u = std::exp(sigma * std::sqrt(dt));
    d = 1.0 / u;
}


void CRR::setSigma(double vol) {
    sigma = vol;
    updateModel();
}

double CRR::getS0() const { return S0; }
double CRR::getK() const { return K; }
double CRR::getR() const { return r; }
double CRR::getSigma() const { return sigma; }
double CRR::getU() const { return u; }
double CRR::getD() const { return d; }
double CRR::getDt() const { return dt; }
double CRR::getT() const { return T; }
int CRR::getN() const { return N; }
bool CRR::getComputed() const { return computed; }
double CRR::getResult() const { return result; }
ExerciseType CRR::getEType() const { return eType; };
OptionType CRR::getOType() const { return oType; };
double CRR::getRiskNeutral() const { return riskNeutral; };

void CRR::setS0(double s0) { S0 = s0; }
void CRR::setK(double k) { K = k; }
void CRR::setR(double rate) { r = rate; }
void CRR::setT(double t) { T = t; }

void CRR::setN(int n) { N = n; }
void CRR::setComputed(bool b) { computed = b; }
void CRR::setResult(double r) { result = r; }
void CRR::setEType(ExerciseType e) { eType = e; };
void CRR::setOType(OptionType o) { oType = o; };
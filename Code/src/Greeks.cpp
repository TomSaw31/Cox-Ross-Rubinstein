#include "CRR.hpp"
#include "Greeks.hpp"

void Greeks::estimateGreeks(CRR crr) {
    const double h = 0.05 * crr.getS0();
    const double hSigma = 1e-3;
    const double hR = 1e-3;
    const double hT = std::min(1e-3, crr.getT() / 2.0);

    CRR crrU;
    CRR crrD;
    double V;
    double Vup;
    double Vdown;

    // DELTA - GAMMA
    crrU = crr;
    crrD = crr;
    crrU.setS0(crrU.getS0() + h);
    crrD.setS0(crrD.getS0() - h);

    V = crr.crrOptionPrice();
    Vup = crrU.crrOptionPrice();
    Vdown = crrD.crrOptionPrice();

    delta = (Vup - Vdown) / (2.0 * h);
    gamma = (Vup - 2 * V + Vdown) / (h * h);

    // VEGA
    crrU = crr;
    crrD = crr;
    crrU.setSigma(crrU.getSigma() + hSigma);
    crrD.setSigma(crrD.getSigma() - hSigma);

    Vup = crrU.crrOptionPrice();
    Vdown = crrD.crrOptionPrice();

    vega = (Vup - Vdown) / (2 * hSigma * 100.);

    // RHO
    crrU = crr;
    crrD = crr;
    crrU.setR(crrU.getR() + hR);
    crrD.setR(crrD.getR() - hR);

    Vup = crrU.crrOptionPrice();
    Vdown = crrD.crrOptionPrice();

    rho = (Vup - Vdown) / (2 * hR * 100.);

    // THETA
    crrU = crr;
    crrD = crr;
    crrU.setT(crrU.getT() + hT);
    crrD.setT(crrD.getT() - hT);

    Vup = crrU.crrOptionPrice();
    Vdown = crrD.crrOptionPrice();

    theta = - (Vup - Vdown) / (2 * hT * 365.);
}

double Greeks::getDelta() const {
    return delta;
}
double Greeks::getGamma() const {
    return gamma;
}
double Greeks::getTheta() const {
    return theta;
}
double Greeks::getVega() const {
    return vega;
}
double Greeks::getRho() const {
    return rho;
}

void Greeks::setDelta(double d) {
    delta = d;
}
void Greeks::setGamma(double g) {
    gamma = g;
}
void Greeks::setTheta(double t) {
    theta = t;
}
void Greeks::setVega(double v) {
    vega = v;
}
void Greeks::setRho(double r) {
    rho = r;
}
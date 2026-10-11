#pragma once

#include "CRR.hpp"
#include "Greeks.hpp"

class WindowCRR {
    private:
        CRR crr;
        void drawUI();
        double res;
        double dt;
        double u;
        double d;
        double riskNeutral;
        Greeks greeks;

    public:
        WindowCRR(CRR crr);

        void render(double spot, double strike, double priceResult);
};
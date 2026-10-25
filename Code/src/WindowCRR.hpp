#pragma once

#include "CRR.hpp"
#include "Greeks.hpp"
#include <string>

class WindowCRR {
    private:
        CRR crr;
        void drawUI();
        double res;
        double dt;
        double u;
        double d;
        double riskNeutral;
        std::string filename = "../data/save";

    public:
        WindowCRR();
        WindowCRR(CRR crr);

        void render(double spot, double strike, double priceResult);
};
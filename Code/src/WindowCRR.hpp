#pragma once

#include "CRR.hpp"

class WindowCRR {
    private:
        CRR crr;
        void drawUI();

    public:
        WindowCRR(CRR crr);

        void render(double spot, double strike, double priceResult);
};
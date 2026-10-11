#pragma once

class Greeks {
    private:
        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;

    public:
        void estimateGreeks(CRR crr);

        double getDelta() const;
        double getGamma() const;
        double getTheta() const;
        double getVega() const;
        double getRho() const;

        void setDelta(double d);
        void setGamma(double g);
        void setTheta(double t);
        void setVega(double v);
        void setRho(double r);
};
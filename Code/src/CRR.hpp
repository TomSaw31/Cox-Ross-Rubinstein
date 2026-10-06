#pragma once
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

enum OptionType {CALL, PUT};
enum ExerciseType {EUROPEAN, AMERICAN};

class CRR {
    private: 
        double S0{105.};
        double K{100.};
        double r{0.05};
        double sigma{0.2};
        double u;
        double d;
        double dt;
        double p;
        int T{5};
        int N{20};
        OptionType oType{CALL};
        ExerciseType eType{EUROPEAN};
        std::vector<double> optionValues;

        double result;
        bool computed = false;
    public:

        double crrOptionPrice();
        CRR();
        double getS0() const;
        double getK() const;
        double getR() const;
        double getSigma() const;
        double getU() const;
        double getD() const;
        double getDt() const;
        int getT() const;
        int getN() const;
        double getResult() const;
        bool getComputed() const;
        ExerciseType getEType() const;
        OptionType getOType() const;

        void setS0(double s0);
        void setK(double k);
        void setR(double rate);
        void setSigma(double sigma);
        void setD(double down);
        void setU(double up);
        void setDt(double delta_t);
        void setT(int t);
        void setN(int n);
        void setResult(double result);
        void setComputed(bool computed);
        void setEType(ExerciseType e);
        void setOType(OptionType o);
};
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <memory>
#include <sstream>

struct Test {
    int depth;
    double init;
    double strike;
    double vol;
    double time;
    double rate;
    double expected;
};

double run_crr(const Test& t) {
    std::stringstream cmd;

    cmd << "./crr " << t.depth << " " << t.init << " " << t.strike << " " << t.vol << " " << t.time << " " << t.rate << " " << 1;

    FILE * pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Erreur execution\n";
        return -1.;
    }

    char buffer[128];

    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        result += buffer;

    pclose(pipe);
    return std::stod(result);
}

int main()
{
    const double THRESHOLD = 0.5;

    std::vector<Test> tests = {
    {2,100,100,0.2,1,0.05,9.5405},
    {3,100,100,0.2,1,0.05,11.0439},
    {5,100,100,0.2,1,0.05,10.8059},
    {10,100,100,0.2,1,0.05,10.2534},
    {20,100,100,0.2,1,0.05,10.3513},
    {30,100,100,0.2,1,0.05,10.3842},

    {20,100,110,0.2,1,0.05,5.96623},
    {20,100,90,0.2,1,0.05,16.7006},
    {20,120,100,0.2,1,0.05,26.1184},
    {20,80,100,0.2,1,0.05,1.87966},

    {20,100,100,0.1,1,0.05,6.75169},
    {20,100,100,0.3,1,0.05,14.0849},
    {20,100,100,0.5,1,0.05,21.5525},

    {20,100,100,0.2,0.25,0.05,4.56541},
    {20,100,100,0.2,0.5,0.05,6.81855},
    {20,100,100,0.2,2,0.05,15.9865},

    {20,100,100,0.2,1,0.01,8.33478},
    {20,100,100,0.2,1,0.03,9.3149},
    {20,100,100,0.2,1,0.1,13.166},
    {20,100,100,0.2,1,-0.01,7.41353},

    {30,100,50,0.2,1,0.05,52.4387},
    {30,100,150,0.2,1,0.05,0.34696},

    {30,100,100,0.8,1,0.05,32.5694},

    {30,100,100,0.2,5,0.05,28.9938}
    };

    int passed = 0;

    for (size_t i = 0; i < tests.size(); i++)
    {
        
        std::cout << "Test " << i+1 << " : ";
        double price = run_crr(tests[i]);
        double diff = std::abs(price - tests[i].expected);

        if (diff < THRESHOLD) {
            std::cout << "PASS ";
            passed++;
        } else {
            std::cout << "FAIL ";
        }
        std::cout << "(expected=" << tests[i].expected << ", got=" << price << ", diff=" << diff << ")\n";
    }

    std::cout << "\nResult : " << passed << "/" << tests.size() << " tests passed\n";
}
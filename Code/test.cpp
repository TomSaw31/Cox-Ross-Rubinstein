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
    int type;
    double expected;
};

struct Dataset {
    std::string name;
    std::vector<Test> tests;
};


double threshold(const Test& t) {
    return 1e-4 * std::max(1.0, t.expected);
}

double run_crr(const Test& t) {
    std::stringstream cmd;

    cmd << "./crr " << t.depth << " " << t.init << " " << t.strike << " " << t.vol << " " << t.time << " " << t.rate << " " << t.type << " " << 1;

    FILE * pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Erreur execution\n";
        return -1.;
    }

    char buffer[128];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        result = buffer;
    pclose(pipe);

    if (result.empty()) {
        std::cerr << "Empty CRR output\n";
        return -1;
    }
    return std::stod(result);
}

int main()
{
    std::vector<Test> tests_call = {
        {2, 100, 100, 0.2, 1, 0.05, 0, 9.5405},
        {6, 100, 100, 0.2, 1, 0.05, 0, 10.1256},
        {15, 100, 100, 0.2, 1, 0.05, 0, 10.5682},
        {24, 100, 100, 0.2, 1, 0.05, 0, 10.3677},
        {10, 110, 100, 0.2, 0.5, 0.03, 0, 13.2444},
        {20, 120, 100, 0.25, 1, 0.05, 0, 27.4759},
        {5, 105, 100, 0.1, 0.25, 0.02, 0, 5.9129},
        {12, 90, 100, 0.2, 1, 0.05, 0, 5.0047},
        {18, 80, 100, 0.3, 0.5, 0.02, 0, 1.5467},
        {22, 95, 100, 0.15, 0.5, 0.03, 0, 2.5628},
    };

    std::vector<Test> tests_put = {
        {2, 100, 100, 0.2, 1, 0.05, 1, 4.6634},
        {6, 100, 100, 0.2, 1, 0.05, 1, 5.2485},
        {15, 100, 100, 0.2, 1, 0.05, 1, 5.6911},
        {24, 100, 100, 0.2, 1, 0.05, 1, 5.4907},
        {10, 110, 100, 0.2, 0.5, 0.03, 1, 1.7556},
        {20, 120, 100, 0.25, 1, 0.05, 1, 2.5988},
        {5, 105, 100, 0.1, 0.25, 0.02, 1, 0.4142},
        {12, 90, 100, 0.2, 1, 0.05, 1, 10.1276},
        {18, 80, 100, 0.3, 0.5, 0.02, 1, 20.5517},
        {22, 95, 100, 0.15, 0.5, 0.03, 1, 6.0740},
    };

    std::vector<Dataset> datasets = {
        {"CALL", tests_call},
        {"PUT", tests_put}
    };

    for (const auto& data : datasets) {
        std::cout << std::endl << data.name << " TESTS" << std::endl;;

        int passed = 0;
        double total_error = 0.0;
        for (size_t i = 0; i < data.tests.size(); i++) {
            std::cout << "Test " << i+1 << " : ";

            double price = run_crr(data.tests[i]);
            double diff = std::abs(price - data.tests[i].expected);

            total_error += diff * diff;
            
            if (diff < threshold(data.tests[i])) {
                std::cout << "PASS ";
                passed++;
            } else {
                std::cout << "FAIL ";
            }

            std::cout << "(expected=" << data.tests[i].expected << ", got=" << price << ", diff=" << diff << ")" << std::endl;
        }

        std::cout << std::endl << "Result : " << passed << "/" << data.tests.size() << " " << data.name << " tests passed" << std::endl;
        std::cout << "RMSE = " << std::sqrt(total_error / data.tests.size()) << std::endl;
    }
    
}
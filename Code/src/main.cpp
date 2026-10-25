#include "WindowCRR.hpp"
#include "JsonParser.hpp"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        CRR(argv[1], argv[1]);
    } else if (argc == 2) {
        CRR(argv[1], argv[2]);
    } else {
        WindowCRR window = WindowCRR();
        window.render(0.,0.,0.);
    }
    return 0;
}
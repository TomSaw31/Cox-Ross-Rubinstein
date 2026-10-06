#include "WindowCRR.hpp"

int main() {
    CRR crr = CRR{};
    WindowCRR window = WindowCRR(crr);
    window.render(0.,0.,0.);
    return 0;
}
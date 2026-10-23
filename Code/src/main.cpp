#include "WindowCRR.hpp"
#include "JsonParser.hpp"
#include <iostream>

int main() {
    CRR crr = CRR{};
    WindowCRR window = WindowCRR(crr);
    window.render(0.,0.,0.);

    JsonParser jsonParser = JsonParser();
    std::string json_text;
    jsonParser.readFile("../data/testcopy.json", json_text);
    text_it start = json_text.begin();
    JsonValue parsed = jsonParser.parseJsonHelper(json_text, start);
    // JsonValue two = (*parsed.json)["two"];

    // std::cout << two.json << std::endl;
    return 0;
}
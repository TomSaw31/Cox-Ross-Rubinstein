#include "WindowCRR.hpp"
#include "JsonParser.hpp"
#include <iostream>

int main() {
    CRR crr = CRR{};
    WindowCRR window = WindowCRR(crr);
    window.render(0.,0.,0.);

    JsonParser jsonParser = JsonParser();
    std::string json_text;
    jsonParser.readFile("../data/test.json", json_text);
    text_it start = json_text.begin();
    JsonValue parsed = jsonParser.parseJsonHelper(json_text, start);
    JsonValue three = (*parsed.json)["three"];

    std::cout << three.i << std::endl;
    return 0;
}
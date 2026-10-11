#include <map>
#include <string>
#include "JsonParser.hpp"
#include <fstream>
#include <assert.h>

static void skipWs(const std::string& text, text_it& it) {
    while (it != text.end() && std::isspace(static_cast<unsigned char>(*it))) { ++it; }
}

void JsonParser::readFile(const std::string& filepath, std::string& output) {
    std::ifstream file(filepath);
    if (!file) throw std::runtime_error("Cannot open " + filepath);
    std::string line;

    while (std::getline(file, line)) {
        output.append(line);
    }
}

JsonValue JsonParser::parsePrimitive(const std::string& text, text_it start, text_it end) {
    if (start == end)
    throw std::runtime_error("Empty primitive at offset " +
                             std::to_string(start - text.begin()));
    std::string substr = text.substr(start - text.begin(), end - start);
    size_t float_point_index = substr.find(".");

    JsonValue val;
    if (float_point_index == std::string::npos) {
        val.i = std::stoi(substr);
    } else {
        val.d = std::stod(substr);
    }
    return val;
}

JsonValue JsonParser::parseJsonHelper(const std::string& text, text_it& it) {
    assert(*it == '{');
    ++it;

    std::map<std::string, JsonValue>* json_map = new std::map<std::string, JsonValue>;

    do {
        const auto [key, value] = retrieveKeyValuePair(text, it);
        (*json_map)[key] = value;

        skipWs(text, it);
        if (it == text.end()) throw std::runtime_error("Unexpected end of input");
    } while (*it != '}');
    ++it;
    JsonValue val;
    val.json = json_map;
    return val;
}

std::pair<std::string, JsonValue> JsonParser::retrieveKeyValuePair(const std::string& text, text_it& it) {
    assert(it != text.end());

    skipWs(text, it);

    text_it curr_it;
    std::string key;
    JsonValue value;

    if (*it != '\"')
        throw std::runtime_error("Expected '\"' at offset " + std::to_string(it - text.begin()));

    curr_it = ++it;
    while (*it != '\"') { ++it; }

    key = text.substr(curr_it - text.begin(), it - curr_it);
    ++it;
    skipWs(text, it);
    if (*it != ':')
        throw std::runtime_error("Expected ':' at offset " + std::to_string(it - text.begin()));
    ++it;

    skipWs(text, it);

    if (*it == '{') { value = parseJsonHelper(text, it); }
    else {
        curr_it = it;

        while (isdigit(*it) || *it == '.') { ++it; }

        value = parsePrimitive(text, curr_it, it);
    }
    skipWs(text, it);
    if (*it == ',') { ++it; }

    return std::make_pair(key, value);
}

JsonValue JsonParser::parseJson(const std::string& filepath) {
    std::string text;
    readFile(filepath, text);
    text_it start = text.begin();
    return parseJsonHelper(text, start);
}
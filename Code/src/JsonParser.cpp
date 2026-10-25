#include <cctype>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "JsonParser.hpp"

const JsonObject& asObject(const JsonValue& v) {
    const auto* p = std::get_if<std::shared_ptr<JsonObject>>(&v.data);
    if (!p || !*p)
        throw std::runtime_error("Expected a JSON object");
    return **p;
}

double asNumber(const JsonValue& v) {
    if (const int* i = std::get_if<int>(&v.data)) 
        return *i;
    if (const double* d = std::get_if<double>(&v.data)) 
        return *d;
    throw std::runtime_error("Expected a number");
}

const JsonValue& field(const JsonObject& obj, const std::string& key) {
    auto it = obj.find(key);
    if (it == obj.end())
        throw std::runtime_error("Missing key: " + key);
    return it->second;
}

// ----- Parsing -----

static void skipWs(const std::string& text, text_it& it) {
    while (it != text.end() && std::isspace(static_cast<unsigned char>(*it))) { ++it; }
}

static void ensureNotEnd(const std::string& text, text_it it) {
    if (it == text.end())
        throw std::runtime_error("Unexpected end of input");
}

static bool isNumberChar(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) || c == '.' || c == '-';
}

void JsonParser::readFile(const std::string& filepath, std::string& output) {
    std::ifstream file(filepath);
    if (!file)
        throw std::runtime_error("Cannot open " + filepath);

    std::string line;
    while (std::getline(file, line))
        output.append(line);
}

JsonValue JsonParser::parsePrimitive(const std::string& text, text_it start, text_it end) {
    if (start == end)
        throw std::runtime_error("Empty primitive at offset " + std::to_string(start - text.begin()));

    std::string substr(start, end);

    if (substr.find_first_of(".eE") == std::string::npos)
        return JsonValue(std::stoi(substr));
    return JsonValue(std::stod(substr));
}

JsonValue JsonParser::parseJsonHelper(const std::string& text, text_it& it) {
    skipWs(text, it);
    ensureNotEnd(text, it);
    if (*it != '{')
        throw std::runtime_error("Expected '{' at offset " + std::to_string(it - text.begin()));
    ++it;

    auto json_map = std::make_shared<JsonObject>();

    skipWs(text, it);
    ensureNotEnd(text, it);
    if (*it == '}') {
        ++it;
        return JsonValue(json_map);
    }

    do {
        const auto [key, value] = retrieveKeyValuePair(text, it);
        (*json_map)[key] = value;

        skipWs(text, it);
        ensureNotEnd(text, it);
    } while (*it != '}');
    ++it;
    return JsonValue(json_map);
}

std::pair<std::string, JsonValue> JsonParser::retrieveKeyValuePair(const std::string& text, text_it& it) {
    skipWs(text, it);
    ensureNotEnd(text, it);

    if (*it != '"')
        throw std::runtime_error("Expected '\"' at offset " + std::to_string(it - text.begin()));

    text_it key_start = ++it;
    while (it != text.end() && *it != '"') { ++it; }
    ensureNotEnd(text, it);

    std::string key(key_start, it);
    ++it;
    skipWs(text, it);
    ensureNotEnd(text, it);
    if (*it != ':')
        throw std::runtime_error("Expected ':' at offset " + std::to_string(it - text.begin()));
    ++it;

    skipWs(text, it);
    ensureNotEnd(text, it);

    JsonValue value;
    if (*it == '{') {
        value = parseJsonHelper(text, it);
    } else {
        text_it num_start = it;
        while (it != text.end() && isNumberChar(*it)) { ++it; }
        value = parsePrimitive(text, num_start, it);
    }
    skipWs(text, it);
    if (it != text.end() && *it == ',') { ++it; }

    return {key, value};
}

JsonValue JsonParser::parseJson(const std::string& filepath) {
    std::string text;
    readFile(filepath, text);
    text_it start = text.begin();
    return parseJsonHelper(text, start);
}

// ----- Storing -----

static void writeValue(std::ostream& out, const JsonValue& value, int indent);

template <typename Container>
static void writeObject(std::ostream& out, const Container& entries, int indent) {
    if (entries.empty()) {
        out << "{}";
        return;
    }
    out << "{\n";
    bool first = true;
    for (const auto& [key, value] : entries) {
        if (!first) out << ",\n";
        first = false;
        out << std::string(indent + 4, ' ') << '"' << key << "\": ";
        writeValue(out, value, indent + 4);
    }
    out << "\n" << std::string(indent, ' ') << '}';
}

static void writeValue(std::ostream& out, const JsonValue& value, int indent) {
    if (const int* i = std::get_if<int>(&value.data)) {
        out << *i;
    } else if (const double* d = std::get_if<double>(&value.data)) {
        out << std::fixed << std::setprecision(6) << *d;
    } else {
        const auto& obj = std::get<std::shared_ptr<JsonObject>>(value.data);
        if (obj) writeObject(out, *obj, indent);
        else out << "{}";
    }
}

void JsonParser::storeData(const std::string& filepath, const std::vector<std::pair<std::string, JsonValue>>& values) {
    std::ofstream file(filepath);
    if (!file)
        throw std::runtime_error("Cannot open " + filepath);

    writeObject(file, values, 0);
    file << '\n';
}
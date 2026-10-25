#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

using text_it = std::string::const_iterator;

struct JsonValue;
using JsonObject = std::map<std::string, JsonValue>;

struct JsonValue {
  std::variant<int, double, std::shared_ptr<JsonObject>> data;

  JsonValue() = default;
  JsonValue(int i) : data(i) {}
  JsonValue(double d) : data(d) {}
  JsonValue(std::shared_ptr<JsonObject> j) : data(std::move(j)) {}
};

const JsonObject& asObject(const JsonValue& v);
double asNumber(const JsonValue& v);
const JsonValue& field(const JsonObject& obj, const std::string& key);

class JsonParser {
  public:
    JsonValue parseJson(const std::string& filepath);
    void storeData(const std::string& filepath, const std::vector<std::pair<std::string, JsonValue>>& values);

  private:
    void readFile(const std::string& filepath, std::string& output);
    JsonValue parsePrimitive(const std::string& text, text_it start, text_it end);
    JsonValue parseJsonHelper(const std::string& text, text_it& it);
    std::pair<std::string, JsonValue> retrieveKeyValuePair(const std::string& text, text_it& it);
};
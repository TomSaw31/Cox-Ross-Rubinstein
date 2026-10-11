#include <map>
#include <string>

using text_it = std::string::iterator;

union JsonValue {
  int i;
  double d;
  std::map<std::string, JsonValue>* json;
};

class JsonParser {
  public:
    void readFile(const std::string& filepath, std::string& output);
    JsonValue parsePrimitive(const std::string& text, text_it start, text_it end);
    JsonValue parseJsonHelper(const std::string& text, text_it& it);
    std::pair<std::string, JsonValue> retrieveKeyValuePair(const std::string& text, text_it& it);
    JsonValue parseJson(const std::string& filepath);
};
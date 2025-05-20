#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
using namespace std;
enum class JsonType { Object, Array, Number, String, Boolean, Null };
class JsonValue {

public:
  using Array = std::vector<JsonValue>;
  using Object = map<string, JsonValue>;

  JsonValue();
  JsonValue(const double &d);
  JsonValue(const string &s);

  JsonValue(const Array &a);
  JsonValue(const bool &b);
  JsonValue(const Object &o);

  ~JsonValue();

  JsonValue(const JsonValue &other);

  JsonValue &operator=(const JsonValue &other);
  JsonType getType() const;

  // getter

  const Object &asObject() const;

  const Array &asArray() const;
  const double &asNumber() const;
  const bool &asBool() const;
  const nullptr_t &asNull() const;
  const string &asString() const;

private:
  JsonType type_;

  union {
    nullptr_t null_;
    double number_;
    string string_;
    Array array_;
    Object object_;
    bool bool_;
  };
};

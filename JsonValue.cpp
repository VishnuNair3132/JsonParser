
#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
enum class JsonType { Object, Array, Number, String, Boolean, Null };
class JsonValue {
  using Array = vector<JsonValue>;
  using Object = map<string, JsonValue>;

  // contructors
  JsonValue() : type_(JsonType::Null) { null_ = nullptr; }

  JsonValue(const double &d) : type_(JsonType::Number) { number_ = d; }

  JsonValue(const string &s) : type_(JsonType::String) {
    new (&string_) string(s);
  }

  JsonValue(const Array &a) : type_(JsonType::Array) { new (&array_) Array(a); }

  JsonValue(const bool &b) : type_(JsonType::Boolean) { bool_ = b; }

  JsonValue(const Object &o) : type_(JsonType::Object) {
    new (&object_) Object(o);
  }

  // deconsturctor

  ~JsonValue() {
    switch (type_) {
    case JsonType::Object:
      object_.~map();
      break;
    case JsonType::Array:
      array_.~vector();
      break;
    case JsonType::String:
      string_.~basic_string();
      break;
    default:
      break;
    }
  };

  // Deep Copy contructors

  JsonValue(const JsonValue &other) : type_(other.type_) {
    switch (type_) {
    case JsonType::Object:
      new (&object_) Object(other.object_);
      break;
    case JsonType::Array:
      new (&array_) Array(other.array_);
      break;
    case JsonType::Number:
      number_ = other.number_;
    case JsonType::String:
      new (&string_) string(other.string_);
      break;
    case JsonType::Boolean:
      bool_ = other.bool_;
      break;
    case JsonType::Null:
      null_ = other.null_;
      break;
    }
  };

  JsonValue &operator=(const JsonValue &other) {
    if (this != &other) {

      this->~JsonValue();

      switch (type_) {
      case JsonType::Object:
        this->object_ = other.object_;
        break;
      case JsonType::Array:
        this->array_ = other.array_;
        break;
      case JsonType::Number:
        this->number_ = other.number_;
        break;
      case JsonType::String:
        this->string_ = other.string_;
        break;
      case JsonType::Boolean:
        this->bool_ = other.bool_;
        break;
      case JsonType::Null:
        this->null_ = other.null_;
        break;
      }
    }
    return *this;
  };

  JsonType getType() const { return type_; }

  // getter

  const Object &asObject() {
    if (type_ != JsonType::Object) {
      throw runtime_error("Not a Object");
    }
    return object_;
  }

  const Array &asArray() {
    if (type_ != JsonType::Array) {
      throw runtime_error("Not a Array");
    }
    return array_;
  }

  const double &asNumber() {
    if (type_ != JsonType::Number) {
      throw runtime_error("Not a Number");
    }
    return number_;
  }

  const bool &asBool() {
    if (type_ != JsonType::Boolean) {
      throw runtime_error("Not a Boolean");
    }
    return bool_;
  }
  const nullptr_t &asNull() {
    if (type_ != JsonType::Null) {
      throw runtime_error("Not a Null");
    }
    return null_;
  }

  const string &asString() {
    if (type_ != JsonType::String) {
      throw runtime_error("Not a string");
    }
    return string_;
  }

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

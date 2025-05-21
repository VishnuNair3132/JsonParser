
#include "JsonValue.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
using namespace std;

// contructors
JsonValue::JsonValue() : type_(JsonType::Null) { null_ = nullptr; }

JsonValue::JsonValue(const double &d) : type_(JsonType::Number) { number_ = d; }

JsonValue::JsonValue(const string &s) : type_(JsonType::String) {
  new (&string_) string(s);
}

JsonValue::JsonValue(const Array &a) : type_(JsonType::Array) {
  new (&array_) Array(a);
}

JsonValue::JsonValue(const bool &b) : type_(JsonType::Boolean) { bool_ = b; }

JsonValue::JsonValue(const Object &o) : type_(JsonType::Object) {
  new (&object_) Object(o);
}
// deconsturctor

JsonValue::~JsonValue() {
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

JsonValue::JsonValue(const JsonValue &other) : type_(other.type_) {
  switch (type_) {
  case JsonType::Object:
    new (&object_) Object(other.object_);
    break;
  case JsonType::Array:
    new (&array_) Array(other.array_);
    break;
  case JsonType::Number:
    number_ = other.number_;
    break;
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

JsonValue &JsonValue::operator=(const JsonValue &other) {
  if (this != &other) {

    this->~JsonValue();

    type_ = other.type_;

    switch (type_) {
    case JsonType::Object:
      new (&object_) Object(other.object_);
      break;
    case JsonType::Array:
      new (&array_) Array(other.array_);
      break;
    case JsonType::Number:
      number_ = other.number_;
      break;
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
  }
  return *this;
};

JsonType JsonValue::getType() const { return type_; }

// helper method to remove escaping characters
static std::string escapeString(const std::string &str) {
  std::string result;
  result.reserve(str.length());

  for (char c : str) {
    switch (c) {
    case '"':
      result += "\\\"";
      break;
    case '\\':
      result += "\\\\";
      break;
    case '\b':
      result += "\\b";
      break;
    case '\f':
      result += "\\f";
      break;
    case '\n':
      result += "\\n";
      break;
    case '\r':
      result += "\\r";
      break;
    case '\t':
      result += "\\t";
      break;
    default:
      if (c >= 0 && c <= 0x1f) {
        char buf[7];
        snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
        result += buf;
      } else {
        result += c;
      }
    }
  }
  return result;
}

// operator overloading for <<Object

ostream &operator<<(ostream &out, const JsonValue &value) {

  switch (value.getType()) {
  case JsonType::Null:
    out << "null";
    break;
  case JsonType::Boolean:
    out << (value.asBool() ? "true" : "false");
    break;
  case JsonType::Number:
    out << value.asNumber();
    break;
  case JsonType::String:
    out << '"' << escapeString(value.asString()) << '"';
    break;
  case JsonType::Array: {
    out << '[';
    const auto &arr = value.asArray();
    for (size_t i{0}; i < arr.size(); ++i) {
      if (i != 0)
        out << ",";
      out << arr[i];
    }
    out << ']';
    break;
  }
  case JsonType::Object:
    out << value.asObject();
    break;
  }
  return out;
}
ostream &operator<<(ostream &out, const JsonValue::Object &object) {
  out << "{\n";
  const string indent = "    ";
  size_t count = 0;
  const size_t size = object.size();

  for (const auto &[key, value] : object) {
    out << indent << "|__\"" << escapeString(key) << "\": ";

    // For nested objects/arrays
    if (value.getType() == JsonType::Object ||
        value.getType() == JsonType::Array) {
      stringstream ss;
      ss << value; // Single print operation
      string nested = ss.str();

      // Add indentation
      size_t pos = 0;
      while ((pos = nested.find('\n', pos)) != string::npos) {
        nested.insert(pos + 1, indent);
        pos += indent.length() + 1;
      }
      out << nested;
    } else {
      out << value; // Single print for primitives
    }

    if (++count != size)
      out << ",";
    out << "\n";
  }
  out << "}";
  return out;
}
// getter

const JsonValue::Object &JsonValue::asObject() const {

  if (type_ != JsonType::Object) {
    throw runtime_error("Not a Object");
  }
  return object_;
}

const JsonValue::Array &JsonValue::asArray() const {
  if (type_ != JsonType::Array) {
    throw runtime_error("Not a Array");
  }
  return array_;
}

const double &JsonValue::asNumber() const {
  if (type_ != JsonType::Number) {
    throw runtime_error("Not a Number");
  }
  return number_;
}

const bool &JsonValue::asBool() const {
  if (type_ != JsonType::Boolean) {
    throw runtime_error("Not a Boolean");
  }
  return bool_;
}
const nullptr_t &JsonValue::asNull() const {
  if (type_ != JsonType::Null) {
    throw runtime_error("Not a Null");
  }
  return null_;
}

const string &JsonValue::asString() const {
  if (type_ != JsonType::String) {
    throw runtime_error("Not a string");
  }
  return string_;
}

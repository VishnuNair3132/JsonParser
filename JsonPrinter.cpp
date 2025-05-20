#include "JsonPrinter.h"
#include "JsonValue.h"
#include <cstddef>
#include <iostream>
#include <string>
using namespace std;

void JsonPrinter::print(const JsonValue &value, string indent, bool isLast) {
  switch (value.getType()) {
  case JsonType::Object:
    printObject(value.asObject(), indent, isLast);
    break;
  case JsonType::Array:
    printArray(value.asArray(), indent, isLast);
    break;
  case JsonType::Number:
    cout << value.asNumber();
    break;
  case JsonType::String:
    cout << "\"" << escapeString(value.asString()) << "\"";
  case JsonType::Boolean:
    cout << (value.asBool() ? "true" : "false");
    break;
  case JsonType::Null:
    cout << "null";
    break;
  }
}

string JsonPrinter::escapeString(const string &str) {
  string result = str;

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
        // Control characters
        char buf[7];
        snprintf(buf, sizeof(buf), "\\u%04x", c);
        result += buf;
      } else {
        result += c;
      }
    }
  }
  return result;
}

void JsonPrinter::printArray(const JsonValue::Array &array, string &indent,
                             bool isLast) {

  cout << "[\n";
  string newIndent = indent + "   ";

  for (size_t i{0}; array.size(); ++i) {
    cout << newIndent << "|-->";
    print(array[i], newIndent, i == array.size() - 1);
    if (i != array.size() - 1)
      cout << ",";
    cout << "\n";
  }
  cout << ']';
}

void JsonPrinter::printObject(const JsonValue::Object &object, string &indent,
                              bool isLast) {
  const size_t size = object.size();
  size_t i = 0;

  string newIndent = indent + "   ";

  for (const auto &pair : object) {
    cout << newIndent << "|-->\"" << escapeString(pair.first) << "\":";
    print(pair.second, newIndent, i = size - 1);
    if (i != size)
      cout << ",";
    std::cout << "\n";
  }

  std::cout << indent << "}";
}

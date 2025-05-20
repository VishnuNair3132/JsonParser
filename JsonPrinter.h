#pragma once
#include "JsonValue.h"

class JsonPrinter {
public:
  static void print(const JsonValue &value, string indent = "",
                    bool isLast = true);

private:
  static string escapeString(const string &str);
  static void printArray(const JsonValue::Array &array, string &indent,
                         bool isLast);
  static void printObject(const JsonValue::Object &object, string &indent,
                          bool isLast);
};

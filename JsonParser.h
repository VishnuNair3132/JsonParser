#pragma once

#include "JsonValue.h"
#include <cstddef>
#include <string>
using namespace std;
class JsonParser {

public:
  JsonParser(const string &jsonStr);

  JsonValue parse();

private:
  size_t index;
  string json;

  void skipWhiteSpaces();
  bool isSpace(char character);
  char current();
  void expect(char c);
  JsonValue parserValue();
  JsonValue parserNull();
  JsonValue parserBool();
  JsonValue parserString();
  JsonValue parserNumber();
  JsonValue parserArray();
  JsonValue parserObject();
};

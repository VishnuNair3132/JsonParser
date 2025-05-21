
#include "JsonParser.h"
#include "JsonValue.h"
#include <iostream>
using namespace std;

JsonParser::JsonParser(const string &jsonStr) : json(jsonStr), index(0) {};

JsonValue JsonParser::parse() {
  skipWhiteSpaces();
  return parserValue();
}

void JsonParser::skipWhiteSpaces() {
  while (index < json.size() && isSpace(json[index])) {
    index++;
  }
}

bool JsonParser::isSpace(char character) {
  return character == ' ' || character == '\n' || character == '\r' ||
         character == '\t';
}
char JsonParser::current() { return index < json.size() ? json[index] : '\0'; }

void JsonParser::expect(char c) {
  if (current() != c) {
    throw runtime_error(string("Expected '") + c + "' but got '" + current() +
                        "'");
  }
  index++;
}

JsonValue JsonParser::parserValue() {
  switch (current()) {
  case 'n':
    return parserNull();
  case 't':
  case 'f':
    return parserBool();
  case '{':
    return parserObject();
  case '[':
    return parserArray();
  case '"':
    return parserString();
  default:
    if (isdigit(current()) || current() == '-') {
      return parserNumber();
    }
  }
  throw runtime_error(string("Unexpected character '") + current() +
                      "' in JSON");
}
JsonValue JsonParser::parserNull() {
  expect('n');
  expect('u');
  expect('l');
  expect('l');
  return JsonValue();
}

JsonValue JsonParser::parserBool() {

  if (current() == 't') {
    expect('t');
    expect('r');
    expect('u');
    expect('e');
    return JsonValue(true);
  } else {
    expect('f');
    expect('a');
    expect('l');
    expect('s');
    expect('e');
    return JsonValue(false);
  }
}

JsonValue JsonParser::parserNumber() {
  size_t start = index;
  if (current() == '-') {
    index++;
  }
  while (isdigit(current())) {
    index++;
  }

  if (current() == '.') {
    index++;
    while (isdigit(current()))
      index++;
  }
  if (current() == 'e' or current() == 'E') {
    index++;
    if (current() == '+' or current() == '-') {
      index++;
    }
    while (isdigit(current())) {
      index++;
    }
  }
  string numstr = json.substr(start, index - start);
  try {
    return JsonValue(stod(numstr));
  } catch (exception e) {
    cout << "exception found: " << e.what() << endl;
    throw std::runtime_error("Invalid number format");
  }
}

JsonValue JsonParser::parserString() {
  expect('"');
  string str;
  str.reserve(32); // Pre-allocate to prevent reallocations

  while (current() != '"') {
    if (current() == '\\') {
      // Handle escape sequence
      index++;
      switch (current()) {
      case '"':
        str += '"';
        break;
      case '\\':
        str += '\\';
        break;
      case '/':
        str += '/';
        break;
      case 'b':
        str += '\b';
        break;
      case 'f':
        str += '\f';
        break;
      case 'n':
        str += '\n';
        break;
      case 'r':
        str += '\r';
        break;
      case 't':
        str += '\t';
        break;
      default:
        throw runtime_error("Invalid escape sequence");
      }
      index++;
    } else {
      str += current();
      index++;
    }
  }
  expect('"');
  return JsonValue(str); // Create new JsonValue with the string
}
JsonValue JsonParser::parserArray() {
  expect('[');
  skipWhiteSpaces();

  JsonValue::Array array;

  if (current() != ']') {
    while (true) {
      array.push_back(parserValue());
      skipWhiteSpaces();
      if (current() == ']')
        break;
      expect(',');
      skipWhiteSpaces();
    }
  }
  expect(']');
  return JsonValue(array);
};

JsonValue JsonParser::parserObject() {
  expect('{');
  skipWhiteSpaces();
  JsonValue::Object object;

  if (current() != '}') {
    while (true) {

      skipWhiteSpaces();
      JsonValue jsonVal = parserString();
      string key = jsonVal.asString();

      skipWhiteSpaces();
      expect(':');
      skipWhiteSpaces();
      JsonValue obj = parserValue();
      object[key] = obj;
      skipWhiteSpaces();
      if (current() == '}')
        break;
      expect(',');
      skipWhiteSpaces();
    }
  }
  expect('}');
  return JsonValue(object);
}

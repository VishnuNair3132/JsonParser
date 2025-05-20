

#include "JsonParser.h"
#include "JsonPrinter.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string readFile(const string &filename) {
  ifstream inFile(filename);

  stringstream buffer;

  buffer << inFile.rdbuf();
  return buffer.str();
};

int main() {
  std::string jsonStr = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false,
        "courses": ["Math", "Science"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown"},
        "specialChars": "Line1\nLine2\tTab\"Quote\\Backslash"})";
  JsonParser parser(jsonStr);
  try {
    JsonValue value = parser.parse();
    cout << "Parsing successful!" << endl;

    JsonPrinter::print(value);
    cout << "\n";
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}

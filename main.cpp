

#include "JsonParser.h"
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
  cout << "Enter Json Filename: " << endl;
  string fileName;
  cin >> fileName;
  string jsonStr = readFile(fileName);
  JsonParser parser(jsonStr);
  try {
    JsonValue value = parser.parse();
    cout << "Parsing successful!" << endl;

    cout << value;
    cout << "\n";
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}

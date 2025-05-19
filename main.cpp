

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

  string filename;

  cout << "Enter File Path:\n";
  cin >> filename;

  string line = readFile(filename);

  cout << line << endl;
  return 0;
}

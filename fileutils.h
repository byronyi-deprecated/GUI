#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

bool openFile(ifstream& ifs, string fileName);
bool openFile(ofstream& ofs, string fileName);

bool getLine(ifstream& ifs, stringstream& ss);
string stripped(const string& s);

#endif // FILEUTILS_H

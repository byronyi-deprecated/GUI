#include "fileutils.h"

bool openFile(ifstream& ifs, string fileName)
{
  ifs.open(fileName.c_str());
  if (!ifs.good())
    return false;
  else
    return true;
}
bool openFile(ofstream& ofs, string fileName)
{
  ofs.open(fileName.c_str());
  if (!ofs.good())
    return false;
  else
    return true;
}

bool getLine(ifstream& ifs, stringstream& ss)
{
  ss.str(string());
  ss.clear();
  string s;
  if (!ifs.good())
    return false;
  getline(ifs, s);
  ss.str(stripped(s));
  return true;
}
string stripped(const string& s)
{
  string::size_type firstPos = s.find_first_not_of(" \t\r\n");
  string::size_type lastPos = s.find_last_not_of(" \t\r\n");

  if (firstPos == string::npos || lastPos == string::npos)
      return string();

  return s.substr(firstPos, lastPos - firstPos + 1);
}

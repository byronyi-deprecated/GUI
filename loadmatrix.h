#ifndef LOADMATRIX_H
#define LOADMATRIX_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
using namespace std;

const int maxMatrixRow = 10000;
const int maxMatrixColumn = 200;

bool File_Has_Decimal(string fileName) {

  ifstream fin(fileName.c_str());

  while (fin.good()) {

      string s;
      getline(fin,s);

      // check if any non-numerical char
      if (s.find_first_not_of("1234567890.-\t\r ") != string::npos) {
          fin.close();
          throw string("Error: file has invalid characters (non-numerical).");
        }

      // if we find a decimal
      if (s.find_first_of('.') != string::npos) {
          fin.close();
          return true;
        }

    }

  fin.close();
  return false;

}

void generate01matrixbycutoffval(string fileName)
{
  ifstream fin(fileName.c_str());
  ofstream fout("temp01matrix");

  string s;
  getline(fin, s);
  stringstream ss(s);

  vector<int> firstLine((istream_iterator<int>(ss)), istream_iterator<int>());
  copy(firstLine.begin(), firstLine.end(), ostream_iterator<int>(fout, "\t"));
  fout << '\n';

  int col = firstLine.size();
  double* line = new double[col];
  double* backup = new double[col];
  double* begin = line;
  double* end = line + col;

  while (fin.good()) {

      for (int i = 0; i != col; ++i) {
          fin >> line[i];
          backup[i] = line[i];
        }

      sort(begin, end);

      double* variancefrombegin = varianceFromBegin(begin, col);
      double* variancefromend = varianceFromEnd(begin, col);

      double minWeight = variancefromend[0] * col * col;
      int position = 0;

      for (int i = 1; i < col; i++) {

          double Weight1 = variancefrombegin[i - 1] * i * i;
          double Weight2 = variancefromend[i] * (col - i) * (col - i);

          double Weight = Weight1 + Weight2;

          if (Weight < minWeight) {
              minWeight = Weight;
              position = i;
            }
        }

      double cutoffvalue = (line[position - 1] + line[position]) / 2;
      cout << cutoffvalue << endl;

      for (int i = 0; i != col; ++i)
        backup[i] = (backup[i] < cutoffvalue)? 0 : 1;

      copy(backup, backup + col, ostream_iterator<double>(fout, " "));
      fout << '\n';

      delete [] variancefrombegin;
      delete [] variancefromend;
    }

  delete [] line;
  delete [] backup;
  fin.close();
  fout.close();
}

void getMatrixInfo(string inputFileName,short matrix[][maxMatrixColumn],short class_type[],int& row,int& column,double& p0,double& p1)
{
  const char *chtemp=inputFileName.c_str();
  ifstream fin(chtemp);
  p0=0;column=0;
  string s;
  getline(fin,s);
  for (int i=0;i<s.length();i++)
    if (s[i]=='0')
      {
        class_type[column++]=0;
        p0=p0+1;
      }
    else if (s[i]=='1')
      class_type[column++]=1;
  p0=p0/column;p1=1-p0;row=0;
  while (fin>>matrix[row][0])
    {
      for (int i=1;i<column;i++)
        fin>>matrix[row][i];
      row++;
    }
  fin.close();
}

void Load_TainVali_Matrix(string fileName,short matrix[][maxMatrixColumn],short classType[],int& row,int& column,double& p0,double& p1)
{
  if (File_Has_Decimal(fileName))
    {
      generate01matrixbycutoffval(fileName);//LoadMatrix.h
      getMatrixInfo("temp01matrix",matrix,classType,row,column,p0,p1);//LoadMatrix.h
      system("del temp01matrix");
    }
  else
    {
      getMatrixInfo(fileName,matrix,classType,row,column,p0,p1);
    }
}




#endif // LOADMATRIX_H

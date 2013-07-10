#ifndef MATRIXm_H
#define MATRIXm_H

#include "fileutils.h"
#include <vector>
using namespace std;

class Line
{
public:
  Line(double* line, int col);
  ~Line();
  double cutoffValue() const {return m_cutoff;}
private:
  void generateCutoff();

  void calVarianceFromBegin(double* variance);
  void calVarianceFromEnd(double* variance);

  double* m_line;
  int m_col;
  double m_cutoff;
  double* m_varianceFromBegin;
  double* m_varianceFromEnd;
};

class Matrix
{
public:
  Matrix();
  ~Matrix();

  bool load(ifstream& ifs, bool raw = true);
  string outputFirstLine();

private:
  bool generate01Matrix(ifstream& ifs);
  bool getMatrixInfo(ifstream& ifs);
  bool getFirstLine(ifstream& ifs);

  vector<int> m_classType;
  vector<int*> m_matrix;
  int m_col;
  int m_row;
  double m_p0;
  double m_p1;
};

#endif // MATRIXm_H

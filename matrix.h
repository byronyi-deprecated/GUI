#ifndef MATRIXm_H
#define MATRIXm_H

#include "fileutils.h"
#include <vector>
#include <set>
#include <map>
using namespace std;

// Line: an assistant class for data pre-processing. Everything is
//       calculted upon Line object construction. Typical usage is:
//       call Line(data, col).cutoffValue() to get the cutoff value.
class Line
{
public:
  Line(const vector<double>& line, int col);
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

// Matrix: the actual data structure that store binary X, Y
class Matrix
{
public:
  Matrix();
  ~Matrix();

  bool load(ifstream& ifs, bool raw = true);
  string outputClassType() const;

  int col() const {return m_col;}
  int row() const {return m_row;}
  double p0() const {return m_p0;}
  double p1() const {return m_p1;}

  const map<set<int>, double>& modules() const {return m_modules;}

  void findModules(int trialTimes, int startingSize, bool filter = false); // TO BE DONE!!!!

private:
  // Load Matrix Methods
  bool generate01Matrix(ifstream& ifs);
  bool getMatrixInfo(ifstream& ifs);
  bool getFirstLine(ifstream& ifs);

  // Find Most Influential Subset Methods, given a orgin set of variables
  double pow(double num, int exp);
  double I_stat(const set<int>& s);
  pair<set<int>, double> dropOneVariable(const set<int>& orgin);
  pair<set<int>, double> findMaxSubset(const set<int>& origin);

  // Filter mothods
  bool isSubset(const set<int>& subset, const set<int>& superset);
  void doFilter(); // TO BE DONE!!!!

  // Modules detected with corresponding I_stat
  map<set<int>, double> m_modules;

  // m_classType is Y and m_matrix is X
  vector<int> m_classType;
  vector<int*> m_matrix;

  int m_col;
  int m_row;
  double m_p0;
  double m_p1;
};

#endif // MATRIXm_H

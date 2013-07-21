#include "matrix.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>

Line::Line(const vector<double>& line, int col)
  : m_line(NULL), m_col(col), m_cutoff(0.0),
    m_varianceFromBegin(NULL), m_varianceFromEnd(NULL)
{
  if (line.size() != static_cast<unsigned int>(col) )
    return;

  m_line = new double[m_col];
  copy(line.begin(), line.end(), m_line);

  m_varianceFromBegin = new double[m_col];
  m_varianceFromEnd = new double[m_col];
  generateCutoff();
}

Line::~Line()
{
  if (m_line)
    delete [] m_line;
  if (m_varianceFromBegin)
    delete [] m_varianceFromBegin;
  if (m_varianceFromEnd)
    delete [] m_varianceFromEnd;
}
//=======================================================================
// generateCutoff: divide the (sorted) array into two parts by
// calculating the cutoff value. See the loop below for details.

void Line::generateCutoff()
{
  // sort m_line before we find the break point
  sort(m_line, m_line + m_col);

  calVarianceFromBegin(m_varianceFromBegin);
  calVarianceFromEnd(m_varianceFromEnd);

  if (!m_varianceFromBegin || !m_varianceFromEnd)
    return;

  double minWeight = m_varianceFromEnd[0] * m_col * m_col;
  int position = 0;

  // find the positition k such that
  // [Var_k * k^2 + Var_(n-k) * (n-k)^2] is minimized
  for (int i = 1; i < m_col; ++i) {

      double Weight1 = m_varianceFromBegin[i-1] * i * i;
      double Weight2 = m_varianceFromEnd[i] * (m_col-i) * (m_col-i);

      double Weight = Weight1 + Weight2;

      if (Weight < minWeight) {
          minWeight = Weight;
          position = i;
        }
    }

  // the cutoff value just lies between m_line[k-1] and m_line[k]
  m_cutoff = ( m_line[position - 1] + m_line[position] ) / 2;
}
//====================================================================
// calVarianceFromBegin: calcute the variance for first k elements.
// An array is used to store all possible results for 0 <= k < n.
// Calculate only once and repeatedly used.

void Line::calVarianceFromBegin(double* variance)
{
  double mean[m_col];
  mean[0] = m_line[0];

  for (int i = 1; i != m_col; ++i)
    mean[i] = mean[i-1] + m_line[i];

  for (int i = 1; i != m_col; ++i)
    mean[i] /= (i+1);

  for (int i = 0; i != m_col; ++i) {
      variance[i] = 0.0;

      for (int j = 0; j != i+1; ++j) {
          variance[i] += (m_line[j] - mean[i]) * (m_line[j] - mean[i]);
        }

      variance[i] /= (i+1);
    }
}
//====================================================================
// calVarianceFromEnd: calcute the variance for last k elements.
// An array is used to store all possible results for 0 <= k < n.
// Calculate only once and repeatedly used.

void Line::calVarianceFromEnd(double* variance)
{
  double mean[m_col];
  mean[m_col-1] = m_line[m_col-1];

  for (int i = m_col-1; i != 0; --i)
    mean[i-1]  = mean[i] + m_line[i-1];

  for (int i = m_col-1; i != 0; --i)
    mean[i-1] /= (m_col-i+1);

  for (int i = m_col; i != 0; --i) {
      variance[i-1] = 0.0;

      for (int j = m_col; j != i-1; --j) {
          variance[i-1] += (m_line[j-1]-mean[i-1]) * (m_line[j-1]-mean[i-1]);
        }

      variance[i-1] /= (m_col-i+1);
    }
}

Matrix::Matrix() :
  m_col(0), m_row(0), m_p0(0.0), m_p1(0.0) {}

Matrix::~Matrix()
{
  for(vector<int*>::iterator iter = m_matrix.begin();
      iter != m_matrix.end(); ++iter)
    if (*iter) delete (*iter);
}

bool Matrix::load(ifstream& ifs, bool raw)
{
  if (!getFirstLine(ifs))
    return false;

  if (raw) {
      return generate01Matrix(ifs);
    }
  else {
      return getMatrixInfo(ifs);
    }
}
//findModules: find influential modules according to samplingSize given.
map<set<int>, double> findModules(int samplingSize, bool filter = false)
{


}
//generate01Matrix: import m_matrix from raw file WITH pre-processing
bool Matrix::generate01Matrix(ifstream &ifs)
{
  while (ifs.good()) {
      stringstream ss;
      getLine(ifs, ss);

      vector<double> row(m_col);
      istream_iterator<double> ii(ss);    // make input iterator for stream
      istream_iterator<double> eos;   // input sentinel, end of stream
      copy(ii, eos, row.begin());

      double cutoffValue = Line(row, m_col).cutoffValue();

      int* new_row = new int[m_col];

      for (int i = 0; i != m_col; ++i)
        new_row[i] = (row[i] < cutoffValue)? 0 : 1;

      m_matrix.push_back(new_row);
    }

  m_row = m_matrix.size();

  return true;
}
//getMatrixInfo: import m_matrix from file WITHOUT pre-processing
bool Matrix::getMatrixInfo(ifstream &ifs)
{
  while (ifs.good()) {
      stringstream ss;
      getLine(ifs, ss);

      int* new_row = new int[m_col];
      copy(istream_iterator<int>(ss), istream_iterator<int>(), new_row);

      m_matrix.push_back(new_row);
    }

  m_row = m_matrix.size();
  return true;
}
//getFirstLine: get m_classType initialized
bool Matrix::getFirstLine(ifstream& ifs)
{
  stringstream ss;
  getLine(ifs, ss);

  int temp(0);
  int num_zero(0), num_one(0);

  while (ss >> temp) {
      m_classType.push_back(temp);
      if (temp == 1)
        ++num_one;
      else if (temp == 0)
        ++num_zero;
    }

  m_col = m_classType.size();

  m_p0 = static_cast<double>(num_zero) / m_col;
  m_p1 = static_cast<double>(num_one) / m_col;

  return true;
}
//outputClassType: an example and a test for correctly data loading
string Matrix::outputClassType() const
{
  stringstream ss;

  copy(m_classType.begin(), m_classType.end(), ostream_iterator<int>(ss, " "));
  return ss.str();
}

//=============================================================================
// pow: power of num to exp
double Matrix::pow(double num, int exp)
{
  if (exp < 1) return 1;
  return pow(num, exp - 1) * num;
}
//=============================================================================
// I_stat: calculate the influential statistic according to
// partions of binary X_k where k belongs to given set s.
// Return I stat.
double Matrix::I_stat(const set<int>& s)
{
  // use binary number to differentiate different partition elements
  // e.g. No.0 element has a index 000, No. 7 element has a index 111 (binary)
  int numPartitions = pow(2, s.size());
  vector<list<int> > partitions( numPartitions, list<int>() );

  for (int i = 0; i != m_row; ++i) {

      size_t partition_idx = 0;

      for (set<int>::iterator iter = s.begin();
           iter != s.end(); ++iter) {
          partition_idx *= 2; // add a bit at the end
          partition_idx += m_matrix[i][*iter];
        }

      // add the observation into the partition where it belongs to
      partitions[partition_idx].push_back(i);
    }

  double i_stat = 0;

  // for each partition, add it to i_stat
  for (vector<list<int> >::iterator iter1 = partitions.begin();
       iter1 != partitions.end(); ++iter1) {

      // (*iter1) is a partition
      double expect_Y1 = m_p1 * (*iter1).size();

      int Y1 = 0;
      for (list<int>::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); ++iter2) {
          Y1 += m_classType[*iter2];
        }

      i_stat = i_stat + (expect_Y1 - Y1) * (expect_Y1 - Y1);
    }

  return i_stat;
}
//=============================================================================
// dropOneVariable: drop one X_i such that after dropping, the I_stat is the max
// of all possible droppings.
// Return a pair of (I_stat, corresponding set after dropping).
pair<set<int>, double> Matrix::dropOneVariable(const set<int>& orgin)
{
  map<set<int>, double> candidateList;

  // find all possible droppings
  for (set<int>::iterator iter = orgin.begin();
       iter != orgin.end(); ++iter) {

      set<int> candidate = orgin;
      candidate.erase(*iter);
      candidateList.insert(pair<set<int>, double>(candidate, I_stat(candidate)));

    }

  // find the max I_stat and corresponding dropping
  map<set<int>, double>::iterator max = candidateList.begin();
  for (map<set<int>, double>::iterator iter = candidateList.begin();
       iter != candidateList.end(); ++iter) {
      if (iter->second > max->second)
        max = iter;
    }

  return *max;
}
//=============================================================================
// findMaxSubset: find the subset with the max influential statistic
// along the whole dropping process (e.g. drop until one variable is left).
pair<set<int>, double> Matrix::findMaxSubset(const set<int>& origin)
{
  map<set<int>, double> candidateList;
  candidateList.insert(pair<set<int>, double> (origin, I_stat(origin)));

  // find all subset until the candidate size is minimized (e.g. 1)
  set<int> candidate = origin;
  while (candidate.size() > 1) {

      pair<set<int>, double> nextStep = dropOneVariable(candidate);
      candidateList.insert(nextStep);
      candidate = nextStep.first;

    }

  // find the max I_stat during all droppings
  map<set<int>, double>::iterator max = candidateList.begin();
  for (map<set<int>, double>::iterator iter = candidateList.begin();
       iter != candidateList.end(); ++iter) {
      if (iter->second > max->second)
        max = iter;
    }

  return *max;
}
//=============================================================================
// doFilter: if both module a and module b are detected, check if a is subset
// of b while I_stat(a) is less than I_stat(b). If so, kick out a from m_modules.
bool Matrix::isSubset(const set<int>& subset, const set<int>& set)
{
  for (set<int>::iterator iter = subset.begin(); iter != subset.end(); ++iter) {
      if (set.find(*iter) == set::end)
        return false;
    }
  return true;
}

//=============================================================================
// doFilter: if both module a and module b are detected, check if a is subset
// of b. If so, kick a out a from m_modules.
void Matrix::doFilter()
{

}

#include "matrix.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>

Line::Line(double* line, int col)
  : m_line(NULL), m_col(col), m_cutoff(0.0),
    m_varianceFromBegin(NULL), m_varianceFromEnd(NULL)
{
  if (!line)
    return;

  m_line = new double[m_col];
  copy(line, line + col, m_line);

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

void Line::generateCutoff()
{
  sort(m_line, m_line + m_col);

  calVarianceFromBegin(m_varianceFromBegin);
  calVarianceFromEnd(m_varianceFromEnd);

  if (!m_varianceFromBegin || !m_varianceFromEnd)
    return;

  double minWeight = m_varianceFromEnd[0] * m_col * m_col;
  int position = 0;

  for (int i = 1; i < m_col; ++i) {

      double Weight1 = m_varianceFromBegin[i-1] * i * i;
      double Weight2 = m_varianceFromEnd[i] * (m_col-i) * (m_col-i);

      double Weight = Weight1 + Weight2;

      if (Weight < minWeight) {
          minWeight = Weight;
          position = i;
        }
    }

  m_cutoff = ( m_line[position - 1] + m_line[position] ) / 2;
}

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

bool Matrix::generate01Matrix(ifstream &ifs)
{
  while (ifs.good()) {
      stringstream ss;
      getLine(ifs, ss);

      double row[m_col];
      copy(istream_iterator<double>(ss), istream_iterator<double>(), row);
      double cutoffValue = Line(row, m_col).cutoffValue();

      int* new_row = new int[m_col];

      for (int i = 0; i != m_col; ++i)
        new_row[i] = (row[i] < cutoffValue)? 0 : 1;

      m_matrix.push_back(new_row);
    }

  m_row = m_matrix.size();

  return true;
}

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

string Matrix::outputFirstLine()
{
  stringstream ss;

  copy(m_classType.begin(), m_classType.end(), ostream_iterator<int>(ss, " "));
  return ss.str();
}

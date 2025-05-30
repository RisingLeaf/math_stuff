#include "matrix.h"

#include <cassert>


linalg::matrix::matrix(const unsigned int dim)
: dim_x(dim), dim_y(dim)
{
  values.resize(dim_x * dim_y, 0.);
}

linalg::matrix::matrix(const unsigned int d_x, const unsigned int d_y)
: dim_x(d_x), dim_y(d_y)
{
  values.resize(dim_x * dim_y, 0.);
}

void linalg::matrix::set_row_zero(const unsigned int x)
{
  assert(x < dim_x && "Access out of bounds");
  for(unsigned int i = 0; i < dim_y; i++)
    values[x * dim_y + i] = 0.;
}

void linalg::matrix::set_col_zero(const unsigned int y)
{
  assert(y < dim_y && "Access out of bounds");
  for(unsigned int i = 0; i < dim_x; i++)
    values[i * dim_y + y] = 0.;
}


void linalg::matrix::add_row(const std::vector<double> &row)
{
  assert(row.size() == dim_y && "Wrong dimension for appending");
  dim_x += 1;
  for(unsigned int i = 0; i < dim_y; ++i)
    values.emplace_back(row[i]);
}


linalg::matrix linalg::matrix::operator+(const double value) const
{
  matrix res = *this;
  for(auto &v : res.values)
    v += value;
  return res;
}

linalg::matrix linalg::matrix::operator-(const double value) const
{
  matrix res = *this;
  for(auto &v : res.values)
    v -= value;
  return res;
}

void linalg::matrix::operator+=(const double value)
{
  for(auto &v : values)
    v += value;
}

void   linalg::matrix::operator-=(const double value)
{
  for(auto &v : values)
    v -= value;
}

linalg::matrix linalg::matrix::operator*(const double value) const
{
  matrix res = *this;
  for(auto &v : res.values)
    v *= value;
  return res;
}

linalg::matrix linalg::matrix::operator/(const double value) const
{
  matrix res = *this;
  for(auto &v : res.values)
    v /= value;
  return res;
}

void linalg::matrix::operator*=(const double value)
{
  for(auto &v : values)
    v *= value;
}

void linalg::matrix::operator/=(const double value)
{
  for(auto &v : values)
    v /= value;
}

linalg::matrix linalg::matrix::operator+(const matrix &other)
{
  assert(this->dim_x == other.dim_x && this->dim_y == other.dim_y && "Trying to add matrices with invalid shapes.");
  matrix res(dim_x, dim_y);
  for(unsigned int x = 0; x < dim_x; x++)
    for(unsigned int y = 0; y < dim_y; y++)
      res(x, y) = (*this)(x, y) + other(x, y);
  return res;
}

void linalg::matrix::operator+=(const matrix &other)
{
  assert(this->dim_x == other.dim_x && this->dim_y == other.dim_y && "Trying to add matrices with invalid shapes.");
  for(unsigned int x = 0; x < dim_x; x++)
    for(unsigned int y = 0; y < dim_y; y++)
      (*this)(x, y) += other(x, y);
}

linalg::matrix linalg::matrix::operator*(const matrix &other) const
{
  assert(this->dim_y == other.dim_x && "Trying to multiply matrices with invalid shapes.");
  matrix res(this->dim_x, other.dim_y);

  for(unsigned int x = 0; x < res.dim_x; x++)
    for(unsigned int y = 0; y < res.dim_y; y++)
      for(unsigned int k = 0; k < this->dim_y; k++)
        res(x, y) += (*this)(x, k) * other(k, y);

  return res;
}

void linalg::matrix::operator*=(const matrix &other)
{
  assert(this->dim_y == other.dim_x && "Trying to multiply matrices with invalid shapes.");
  matrix res(this->dim_x, other.dim_y);

  for(unsigned int x = 0; x < res.dim_x; x++)
    for(unsigned int y = 0; y < res.dim_y; y++)
      for(unsigned int k = 0; k < this->dim_y; k++)
        res(x, y) += (*this)(x, k) * other(k, y);

  *this = res;
}

double &linalg::matrix::operator()(const unsigned int x, const unsigned int y)
{
  assert(x < dim_x && y < dim_y && "Access out of bounds");
  return values[x * dim_y + y];
}

double linalg::matrix::operator()(const unsigned int x, const unsigned int y) const
{
  assert(x < dim_x && y < dim_y && "Access out of bounds");
  return values[x * dim_y + y];
}


linalg::matrix linalg::matrix::partial_matrix(const unsigned int s_x, const unsigned int e_x, const unsigned int s_y, const unsigned int e_y) const
{
  assert(e_x > s_x && e_y > s_y && "Lower bound must be smaller than upper bound.");
  assert(e_x <= dim_x && e_y <= dim_y && "Access out of bounds");
  matrix result(e_x - s_x, e_y - s_y);
  for(unsigned int x = s_x; x < e_x; x++)
    for(unsigned int y = s_y; y < e_y; y++)
      result(x - s_x, y - s_y) = (*this)(x, y);

  return result;
}

std::pair<linalg::matrix, linalg::matrix> linalg::matrix::lu_decompose() const
{
  assert(dim_x == dim_y && "LU decomposition is only available for square matrices");
  matrix L(dim_x), U(dim_x);

  for (unsigned int i = 0; i < dim_x; ++i)
  {
    for (unsigned int k = i; k < dim_y; ++k)
    {
      double sum = 0;
      for (unsigned int j = 0; j < i; ++j)
        sum += (L(i, j) * U(j, k));
      U(i, k) = (*this)(i, k) - sum;
    }

    for (unsigned int k = i; k < dim_y; ++k)
    {
      if (i == k)
        L(i, i) = 1;
      else
      {
        double sum = 0;
        for (unsigned int j = 0; j < i; ++j)
          sum += (L(k, j) * U(j, i));
        L(k, i) = ((*this)(k, i) - sum) / U(i, i);
      }
    }
  }

  return std::make_pair(L, U);
}


std::string linalg::matrix::to_string() const
{
  std::string res = "matrix<" + std::to_string(dim_x) + "," + std::to_string(dim_y) + ">(\n";
  for(unsigned int x = 0; x < dim_x; x++)
  {
    for(unsigned int y = 0; y < dim_y; y++)
      res += ((*this)(x, y) > 0 ? " " : "" ) + std::to_string((*this)(x, y)) + ", ";
    res += "\n";
  }
  return res + ");";
}
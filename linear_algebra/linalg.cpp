#include "linalg.h"

#include <cassert>
#include <stdexcept>

linalg::matrix linalg::forward_substitution(const matrix &L, const matrix &b)
{
  assert(L.get_dimension_x() == L.get_dimension_y() && L.get_dimension_x() == b.get_dimension_x() && "Wrong dimensions");
  const size_t n = L.get_dimension_x();
  matrix y(n, 1);

  for (size_t i = 0; i < n; ++i)
  {
    double sum = 0.0;
    for (size_t j = 0; j < i; ++j)
      sum += L(i, j) * y(j, 0);
    y(i, 0) = b(i, 0) - sum;
  }

  return y;
}

linalg::matrix linalg::backward_substitution(const matrix &U, const matrix &y)
{
  assert(U.get_dimension_x() == U.get_dimension_y() && U.get_dimension_x() == y.get_dimension_x() && "Wrong dimensions");
  const size_t n = U.get_dimension_x();
  matrix x(n, 1);

  for (int i = static_cast<int>(n) - 1; i >= 0; --i)
  {
    double sum = 0.0;
    for (size_t j = i + 1; j < n; ++j)
      sum += U(i, j) * x(j, 0);

    const double denominator = U(i, i);
    if (std::abs(denominator) < 1e-12)
    {
      throw std::runtime_error("Singular matrix: zero pivot encountered in U");
    }

    x(i, 0) = (y(i, 0) - sum) / denominator;
  }

  return x;
}

linalg::matrix linalg::solve_LU(const matrix &A, const matrix &b)
{
  assert(A.get_dimension_x() == A.get_dimension_y() && A.get_dimension_x() == b.get_dimension_x() && "Wrong dimensions");
  const auto [L, U] = A.lu_decompose();

  const matrix y = forward_substitution(L, b);
  const matrix x = backward_substitution(U, y);

  return x;
}

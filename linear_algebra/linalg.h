#ifndef LINALG_H
#define LINALG_H

#include "matrix.h"

namespace linalg
{
  matrix forward_substitution( const matrix &L, const matrix &b);
  matrix backward_substitution(const matrix &U, const matrix &y);
  matrix solve_LU(             const matrix &A, const matrix &b);
}

#endif //LINALG_H

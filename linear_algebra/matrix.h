#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <utility>
#include <vector>


namespace linalg
{
  class matrix
  {
    std::vector<double> values;
    unsigned int dim_x;
    unsigned int dim_y;
  public:
    explicit matrix(unsigned int dim);
    explicit matrix(unsigned int d_x, unsigned int d_y);

    void set_row_zero(unsigned int x);
    void set_col_zero(unsigned int y);

    void add_row(const std::vector<double> &row);

    [[nodiscard]] matrix operator+( double value) const;
    [[nodiscard]] matrix operator-( double value) const;
    void operator+=(double value);
    void operator-=(double value);

    [[nodiscard]] matrix operator*( double value) const;
    [[nodiscard]] matrix operator/( double value) const;
    void operator*=(double value);
    void operator/=(double value);

    [[nodiscard]] matrix operator+( const matrix &other);
    void operator+=(const matrix &other);

    [[nodiscard]] matrix operator*( const matrix &other) const;
    void operator*=(const matrix &other);

    double &operator()(unsigned int x, unsigned int y);
    double operator()( unsigned int x, unsigned int y) const;

    [[nodiscard]] unsigned int get_dimension_x() const { return dim_x; }
    [[nodiscard]] unsigned int get_dimension_y() const { return dim_y; }
    [[nodiscard]] matrix partial_matrix(unsigned int s_x, unsigned int e_x, unsigned int s_y, unsigned int e_y) const;
    [[nodiscard]] std::pair<matrix, matrix> lu_decompose() const;

    std::string to_string() const;
  };

}



#endif //MATRIX_H

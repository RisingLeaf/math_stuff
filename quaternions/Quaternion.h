//
// Created by pi on 5/13/25.
//

#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>


template<class T>
class Quaternion {
public:
  T x, i, j, k;

  Quaternion(T x, T i, T j, T k) : x(x), i(i), j(j), k(k) {}
  Quaternion(T x, T i, T j)      : x(x), i(i), j(j), k(0) {}
  Quaternion(T x, T i)           : x(x), i(i), j(0), k(0) {}
  explicit Quaternion(T v)       : x(v), i(v), j(v), k(v) {}
  Quaternion()                   : x(0), i(0), j(0), k(0) {}
  static Quaternion Rotation(T phi, T i, T j, T k)
  {
    T length = std::sqrt(i*i + j*j + k*k);

    T scalar = std::cos(phi/2.);
    T vector = std::sin(phi/2.) / length;


    return Quaternion(scalar, vector * i, vector * j, vector * k);
  }

  // Addition with Quaternion
  template<class U>
  Quaternion operator+(const Quaternion<U> &other) const
  {
    return Quaternion(x + other.x, i + other.i, j + other.j, k + other.k);
  }
  template<class U>
  void operator+=(const Quaternion<U> &other)
  {
    this->x += other.x; this->i += other.i; this->j += other.j; this->k += other.k;
  }

  // Addition of Scalar, treated as Quaternion
  template<class U>
  Quaternion operator+(const U &v) const
  {
    return Quaternion(x + v, i + v, j + v, k + v);
  }
  template<class U>
  void operator+=(const U &v)
  {
    this->x += v; this->i += v; this->j += v; this->k += v;
  }

  // Multiplication with scalar
  template<class U>
  Quaternion operator*(const U &v) const
  {
    return Quaternion(x * v, i * v, j * v, k * v);
  }
  template<class U>
  void operator*=(const U &v)
  {
    this->x *= v; this->i *= v; this->j *= v; this->k *= v;
  }

  // Methods needed for definition of complete multiplication.
  Quaternion Conjugation() const
  {
    return Quaternion(x, -i, -j, -k);
  }

  T Norm() const
  {
    return x*x + i*i + j*j + k*k;
  }

  T Length() const
  {
    return std::sqrt(Norm());
  }

  Quaternion Inverse() const
  {
    return Quaternion(x, -i, -j, -k) * (1 / Norm());
  }

  Quaternion Normalize() const
  {
    return *this * (1 / Length());
  }

  // Multiplication with Quaternion
  template<class U>
  Quaternion operator*(const Quaternion<U> &other) const
  {
    T nx = this->x * other.x - this->i * other.i - this->j * other.j - this->k * other.k;
    T ni = this->x * other.i + this->i * other.x + this->j * other.k - this->k * other.j;
    T nj = this->x * other.j - this->i * other.k + this->j * other.x + this->k * other.i;
    T nk = this->x * other.k + this->i * other.j - this->j * other.i + this->k * other.x;
    return Quaternion(nx, ni, nj, nk);
  }
  template<class U>
  void operator*=(const Quaternion<U> &other)
  {
    T nx = this->x * other.x - this->i * other.i - this->j * other.j - this->k * other.k;
    T ni = this->x * other.i + this->i * other.x + this->j * other.k - this->k * other.j;
    T nj = this->x * other.j - this->i * other.k + this->j * other.x + this->k * other.i;
    T nk = this->x * other.k + this->i * other.j - this->j * other.i + this->k * other.x;
    x = nx; i = ni; j = nj; k = nk;
  }

  // Division with Quaternion
  template<class U>
  Quaternion operator/(const Quaternion<U> &other)
  {
    return *(this * other.Inverse());
  }
  template<class U>
  void operator/=(const Quaternion<U> &other)
  {
    this = this * other.Inverse();
  }

  // Debug functionality:
  [[nodiscard]] std::string ToString() const
  {
    return "Quat(" + std::to_string(x) + "," + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + ")";
  }
  [[nodiscard]] std::string ToRotationString() const
  {
    return "Quat(phi:" + std::to_string(std::acos(x) * (180/M_PI)) + ";axis:" + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + ")";
  }
};


typedef Quaternion<double> QuatD;
typedef Quaternion<double> QuatF;


#endif //QUATERNION_H

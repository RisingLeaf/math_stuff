cmake_minimum_required(VERSION 3.30)
project(quaternions)

set(CMAKE_CXX_STANDARD 20)

add_executable(quaternions main.cpp
  quaternions/Quaternion.h
  linear_algebra/matrix.cpp
  linear_algebra/matrix.h
  linear_algebra/linalg.h
  linear_algebra/linalg.cpp
  loading/WavefrontObj.cpp
  loading/WavefrontObj.h

  system/Log.cpp
  system/Log.h
)


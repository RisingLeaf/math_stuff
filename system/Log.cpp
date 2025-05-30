#include "Log.h"

#include <iostream>
#include <ostream>

namespace Log {
  void ExitError(const std::string &message) {
    std::cerr << message << std::endl;
    exit(1);
  }

  void Msg(const std::string &message)
  {
    std::cout << message << std::endl;
  }

  void MsgF(const double value)
  {
    Msg(std::to_string(value));
  }

  void MsgI(const int value)
  {
    Msg(std::to_string(value));
  }
}

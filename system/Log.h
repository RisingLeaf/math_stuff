#ifndef LOG_H
#define LOG_H

#include <string>


namespace Log {
    void ExitError(const std::string &message);

    void Msg(const std::string &message);
    void MsgF(double value);
    void MsgI(int value);
}



#endif //LOG_H

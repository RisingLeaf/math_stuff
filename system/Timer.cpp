#include <sys/time.h>
#include <unistd.h>

#include "Timer.h"

static double time_program_start = 0;

int tmsystimer::Init()
{
    time_program_start = 0;
    time_program_start = tmsystimer::TimerSecD();
    return 1;
}
void tmsystimer::Sleep(const int milli_seconds)
{
    usleep(1000 * milli_seconds);
}
void tmsystimer::SleepUntil(const double time)
{
    double time_now = TimerSecD();

    while (time_now < time)
    {
        usleep(100);
        time_now = TimerSecD();
    }
}
double tmsystimer::TimerSecD()
{
  timeval tt{};
  gettimeofday(&tt, nullptr);
  return tt.tv_sec + 0.000001 * tt.tv_usec - time_program_start;
}
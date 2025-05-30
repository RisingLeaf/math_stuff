#ifndef TIMER_H
#define TIMER_H

class tmsystimer final
{
public:
    static int Init();                          // this function has to be called at least once before calling the timer functions
    static void Sleep(int milli_seconds); // sleep for milli_seconds
    static void SleepUntil(double time);  // sleep at least until absolute time has been reached
    static double TimerSecD();                  // return current time in seconds since call to Init
};

#endif
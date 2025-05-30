#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <vector>

namespace Events
{
  enum class Action
  {
    NONE,
    RESIZE,
    MOUSEBUTTON,
    MOUSEMOVE,
    KEY
  };

  struct Event
  {
    Action action = Action::NONE;
    bool press = false;
    int key    = -1;
    double x   = 0.;
    double y   = 0.;
  };

  void AddEvent(Event event);
  const std::vector<Event> &GetCurrentEvents();
};

#endif //EVENTS_HPP

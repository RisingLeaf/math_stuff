#include "Events.h"

#include <mutex>


namespace
{
  std::mutex event_mutex;
  std::vector<Events::Event> event_list;
}


void Events::AddEvent(Event event)
{
  std::lock_guard guard(event_mutex);

  event_list.emplace_back(event);
}

const std::vector<Events::Event> &Events::GetCurrentEvents()
{
  std::lock_guard guard(event_mutex);

  static std::vector<Event> copy;
  copy.clear();
  event_list.swap(copy);

  return copy;
}

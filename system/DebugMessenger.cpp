#include "DebugMessenger.h"

#include <mutex>
#include <unordered_map>


namespace Messenger
{
  std::mutex MessageMutex;
  std::unordered_map<std::string, std::pair<void *, size_t>> Messages;
}


void Messenger::SubmitMessage(const std::string &id, const void *data, size_t size)
{
  std::lock_guard guard(MessageMutex);
  auto it = Messages[id];
  if(it.first)
    free(it.first);

  it.first = new unsigned char[size];
  memcpy(it.first, data, size);
}


std::pair<const void *, size_t> Messenger::RetreiveMessage(const std::string &id)
{
  return Messages[id];
}


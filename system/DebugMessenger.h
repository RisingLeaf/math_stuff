#ifndef DEBUGMESSENGER_H
#define DEBUGMESSENGER_H

#include <string>

namespace Messenger
{
  void SubmitMessage(const std::string &id, const void *data, size_t size);
  std::pair<const void *, size_t> RetreiveMessage(const std::string &id);
}

#endif //DEBUGMESSENGER_H

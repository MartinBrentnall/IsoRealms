#ifndef I_FILE_SELECTION_LISTENER_H
#define I_FILE_SELECTION_LISTENER_H

#include <string>

class IFileSelectionListener {
  public:
  virtual void fileSelectionAsserted(const std::string&) = 0;
};

#endif

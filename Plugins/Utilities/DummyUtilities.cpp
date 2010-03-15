#include "DummyUtilities.h"

extern "C" IPlugin* create() {
  std::cout << "Cannot instantiate utilities dummy!" << std::endl;
  return NULL;
}


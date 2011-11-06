#ifndef ENGINE_ARGUMENTS_H
#define ENGINE_ARGUMENTS_H

#include <string>

#include "IEngineArguments.h"

class EngineArguments:public IEngineArguments {
  private:
  std::string cArguments;

  public:

  /*******************************\
   * Implements IEngineArguments *
  \*******************************/
  void set(const std::string&);
  std::string get();
};

#endif

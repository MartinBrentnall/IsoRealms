#ifndef ARGUMENT_SOURCE_H
#define ARGUMENT_SOURCE_H

#include <IsoRealms/Resources/IResourceLocator.h>

#include "IArgumentSource.h"
#include "LuaArgument.h"

template <class T> class ArgumentSource:public IArgumentSource {
  private:
  T* cValue;

  public:
  ArgumentSource(T* value) {
    cValue = value;
  }

  /******************************\
   * Implements IArgumentSource *
  \******************************/
  ILuaFunctionArgument* createArgument(bool defaultArg) {
    return new LuaArgument<T>(cValue, defaultArg);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resources) {
    std::string mPath = resources->getPath(cValue);
    node->addAttribute("value", mPath);
  }
};

#endif

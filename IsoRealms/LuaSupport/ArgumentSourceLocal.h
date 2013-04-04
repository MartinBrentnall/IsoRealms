#ifndef ARGUMENT_SOURCE_LOCAL_H
#define ARGUMENT_SOURCE_LOCAL_H

#include "IArgumentLocator.h"
#include "IArgumentValue.h"
#include "IArgumentSource.h"
#include "LuaArgumentLocal.h"
#include "../Utils.h"

template <class T> class ArgumentSourceLocal:public IArgumentSource {
  private:
  ArgumentValue<T>* cValue;
  IArgumentLocator* cLocalResources;

  public:
  ArgumentSourceLocal(ArgumentValue<T>* value, IArgumentLocator* localResources) {
    cLocalResources = localResources;
    cValue = value;
  }

  /******************************\
   * Implements IArgumentSource *
  \******************************/
  ILuaFunctionArgument* createArgument(bool defaultArg) {
    return new LuaArgumentLocal<T>(cValue, cLocalResources);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    // TODO: Implement this?
  }
};

#endif

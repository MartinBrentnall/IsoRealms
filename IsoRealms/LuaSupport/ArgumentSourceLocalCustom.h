#ifndef ARGUMENT_SOURCE_LOCAL_CUSTOM_H
#define ARGUMENT_SOURCE_LOCAL_CUSTOM_H

#include "IArgumentValue.h"
#include "IArgumentSource.h"
#include "LuaArgumentLocal.h"
#include "../Utils.h"

template <class T> class ArgumentSourceLocalCustom:public IArgumentSource {
  private:
  IArgumentValue<T>* cValue;

  public:
  ArgumentSourceLocalCustom(IArgumentValue<T>* value) {
    cValue = value;
  }

  /******************************\
   * Implements IArgumentSource *
  \******************************/
  ILuaFunctionArgument* createArgument() {
    return new LuaArgumentLocalCustom<T>(cValue);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
    // TODO: Implement this?
  }
};

#endif

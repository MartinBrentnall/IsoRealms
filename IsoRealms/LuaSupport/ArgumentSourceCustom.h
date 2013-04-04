#ifndef ARGUMENT_SOURCE_CUSTOM_H
#define ARGUMENT_SOURCE_CUSTOM_H

#include <IsoRealms/Resources/IResourceLocator.h>

#include "IArgumentSource.h"
#include "LuaArgumentCustom.h"

template <class T> class ArgumentSourceCustom:public IArgumentSource {
  private:
  T* cValue;

  public:
  ArgumentSourceCustom(T* value) {
    cValue = value;
  }

  /******************************\
   * Implements IArgumentSource *
  \******************************/
  ILuaFunctionArgument* createArgument(bool defaultArgument) {
    return new LuaArgumentCustom<T>(cValue, this, defaultArgument);
  }
  
  void save(DOMNodeWriter* node, IResourceLocator* resources) {
    std::string mPath = resources->getPath(this);
    mPath = mPath.substr(mPath.find_last_of('/') + 1);
    node->addAttribute("value", mPath);
  }
};

#endif

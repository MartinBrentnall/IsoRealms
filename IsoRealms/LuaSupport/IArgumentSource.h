#ifndef I_ARGUMENT_SOURCE_H
#define I_ARGUMENT_SOURCE_H

#include <string>

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceLocator.h>

#include "ILuaFunctionArgument.h"

class IArgumentSource {
  public:
  virtual ILuaFunctionArgument* createArgument(bool) = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
};

#endif

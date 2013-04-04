#ifndef I_ARGUMENT_DEFINITION_H
#define I_ARGUMENT_DEFINITION_H

#include <IsoRealms/Persistence/DOMNodeWriter.h>
#include <IsoRealms/Resources/IResourceLocator.h>

#include "ILuaFunctionArgument.h"

class IArgumentDefinition {
  public:
  virtual std::string getName() = 0;
  virtual std::string getCode(const std::string&, unsigned int) = 0;
  virtual std::string getCleanup() = 0;
  virtual void save(DOMNodeWriter*) = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
  virtual ILuaFunctionArgument* createDefaultArgumentValue() = 0;
};

#endif

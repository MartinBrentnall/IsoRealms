#ifndef I_ARGUMENT_DEFINITION_TYPE_H
#define I_ARGUMENT_DEFINITION_TYPE_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

#include "IArgumentDefinition.h"

class IArgumentDefinitionType {
  public:
  virtual void setType(std::string) = 0;
  virtual IArgumentDefinition* createArgumentDefinition(DOMNodeWrapper*, IResourceAccessor*) = 0;
};

#endif

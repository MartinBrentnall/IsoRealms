#ifndef I_SCRIPT_H
#define I_SCRIPT_H

#include "Persistence/DOMNodeWriter.h"
#include "Resources/IResourceLocator.h"

class IScript {
  public:
  virtual void execute() = 0;
  virtual void save(DOMNodeWriter*, IResourceLocator*) = 0;
};

#endif

#ifndef I_RESOURCE_H
#define I_RESOURCE_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>

class IResourceAccessor;

class IResource {
  public:
  virtual void initialiseResource(DOMNodeWrapper*, IResourceAccessor*) = 0;
};

#endif

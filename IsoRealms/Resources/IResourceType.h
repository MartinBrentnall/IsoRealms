#ifndef I_RESOURCE_TYPE_H
#define I_RESOURCE_TYPE_H

#include "IResourceInstanceListener.h"
#include "IRuntimeContext.h"

template <class T> class IResourceType {
  public:
  virtual void createResource(IResourceAccessor*) = 0;
  virtual void editResource(T*, IResourceAccessor*) = 0;
  virtual void removeResource(T*, IResourceAccessor*, IResourceInstanceListener<T>*) = 0;
};

#endif

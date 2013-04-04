#ifndef I_RESOURCE_LISTENER_COLOURS_H
#define I_RESOURCE_LISTENER_COLOURS_H

#include "IResourceType.h"

template <class T> class IResourceListener {
  public:
  virtual void resourceAdded(T*) = 0;
  virtual void resourceDeleted(T*) = 0;
  virtual void resourceTypeAdded(IResourceType<T>*, const std::string&) = 0;
};

#endif

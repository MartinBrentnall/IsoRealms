#ifndef I_RESOURCE_INSTANCE_LISTENER_H
#define I_RESOURCE_INSTANCE_LISTENER_H

template <class T> class IResourceInstanceListener {
  public:
  virtual void resourcePendingDestruction(T*) = 0;
};

#endif

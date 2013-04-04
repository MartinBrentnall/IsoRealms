#ifndef I_RESOURCE_SELECTION_LISTENER_H
#define I_RESOURCE_SELECTION_LISTENER_H

template <class T> class IResourceSelectionListener {
  public:
  virtual void resourceSelected(T*) = 0;
};

#endif

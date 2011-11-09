#ifndef REGISTRY_PROXY_H
#define REGISTRY_PROXY_H

#include <string>
#include <vector>

#include "Registry.h"

template<class T1, class T2 = T1> class RegistryProxy {
  private:
  Registry<T1, T2>* cRoot;
  Registry<T1, T2>* cRegistration;
  
  public:
  RegistryProxy(Registry<T1, T2>* root, std::vector<std::string> location) {
    cRoot = root;
    Registry<T1, T2>* mCurrent = cRoot;
    for (unsigned int i = 0; i < location.size(); i++) {
      Registry<T1, T2>* mSubDirectory = mCurrent->getSubDirectory(location[i]);
      mCurrent = mSubDirectory != NULL ? mSubDirectory : mCurrent->createSubDirectory(location[i]);
    }
    cRegistration = mCurrent;
  }
    
  std::string getLocation(T1* t) {
    return cRegistration->getLocation(t);
  }
  
  void add(T1* t, const std::string& name) {
    cRegistration->add(t, name);
  }
  
  void remove(T1* t) {
    cRegistration->remove(t);
  }
};

#endif

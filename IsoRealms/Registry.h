#ifndef REGISTRY_H
#define REGISTRY_H

#include <map>
#include <string>

template<class T1, class T2 = T1> class Registry {
  private:
  std::map<std::string, T2*> cObjects;
  std::map<std::string, Registry<T1, T2>*> cSubDirectories;
  
  public:
  Registry* getSubDirectory(const std::string& name) {
    typename std::map<std::string, Registry*>::iterator i = cSubDirectories.find(name);
    return i == cSubDirectories.end() ? NULL : i->second;
  }

  Registry* createSubDirectory(const std::string& name) {
    typename std::map<std::string, Registry*>::iterator i = cSubDirectories.find(name);
    if (i != cSubDirectories.end()) {
      return i->second;
    }
    cSubDirectories[name] = new Registry<T1, T2>();
    return cSubDirectories[name];
  }
    
  std::string getLocation(T1* t) {
    for (typename std::map<std::string, T2*>::iterator i = cObjects.begin(); i != cObjects.end(); i++) {
      if (i->second == t) {
        return t->getName();
      }
    }
    for (typename std::map<std::string, Registry<T1, T2>*>::iterator i = cSubDirectories.begin(); i != cSubDirectories.end(); i++) {
      std::string mName = i->second->getLocation(t);
      if (mName != "") {
        return i->first + "/" + mName;
      }
    }
    return "";
  }
  
  T2* getProxy(const std::string& name) {
    T2* mProxy = cObjects[name];
    if (mProxy == NULL) {
      mProxy = new T2();
      cObjects[name] = mProxy;
    }
    return mProxy;
  }

  T1* get(std::string path) {
    std::string::size_type mDirectorySeparator = path.find("/");
    Registry<T1, T2>* mDir = this;
    while (mDirectorySeparator != std::string::npos) {
      std::string mDirectory = path.substr(0, mDirectorySeparator);
      mDir->createSubDirectory(mDirectory);
      mDir = mDir->getSubDirectory(mDirectory);
      path = path.substr(mDirectorySeparator + 1, path.length());
      mDirectorySeparator = path.find("/");
    }
    return mDir->getProxy(path);
  }

  void add(T1* t) {
    // TODO: Check if name already in use.
    std::string mName = t->getName();
    T2* mProxy = getProxy(mName);
    mProxy->setImplementation(t);
  }
  
  void remove(T1* t) {
    // TODO: Check if name already in use.
    std::string mName = t->getName();
    T2* mProxy = getProxy(mName);
    mProxy->setImplementation(NULL);
  }
};

#endif

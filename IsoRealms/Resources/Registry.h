/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef REGISTRY_H
#define REGISTRY_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

template<class T1> class Registry {
  private:
  std::map<std::string, T1*> cObjects;
  std::map<std::string, Registry<T1>*> cSubDirectories;
  
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
    cSubDirectories[name] = new Registry<T1>();
    return cSubDirectories[name];
  }
    
  std::string getLocation(T1* t) {
    for (typename std::map<std::string, T1*>::iterator i = cObjects.begin(); i != cObjects.end(); i++) {
      if (i->second == t) {
        return i->first;
      }
    }
    for (typename std::map<std::string, Registry<T1>*>::iterator i = cSubDirectories.begin(); i != cSubDirectories.end(); i++) {
      std::string mName = i->second->getLocation(t);
      if (mName != "") {
        return i->first + "/" + mName;
      }
    }
    return "";
  }
  
  T1* get(std::string path) {
    std::string::size_type mDirectorySeparator = path.find("/");
    Registry<T1>* mDir = this;
    while (mDirectorySeparator != std::string::npos) {
      std::string mDirectory = path.substr(0, mDirectorySeparator);
      mDir->createSubDirectory(mDirectory);
      mDir = mDir->getSubDirectory(mDirectory);
      path = path.substr(mDirectorySeparator + 1, path.length());
      mDirectorySeparator = path.find("/");
    }
    typename std::map<std::string, T1*>::iterator i = mDir->cObjects.find(path);
    return i != mDir->cObjects.end() ? i->second : NULL;
  }

  void add(T1* t, const std::string& name) {
    // TODO: Check if name already in use.
    cObjects[name] = t;
  }
  
  void remove(T1* t) {
    // TODO: Remove the thing
/*    T2* mProxy = getProxy(t);
    mProxy->setImplementation(NULL);*/
  }
  
  std::map<std::string, T1*> getAll() {
    return cObjects;
  }
  
  std::vector<T1*> getAllVector() {
    std::vector<T1*> mVector;
    for (typename std::map<std::string, T1*>::iterator i = cObjects.begin(); i != cObjects.end(); i++) {
      mVector.push_back(i->second);
    }
    for (typename std::map<std::string, Registry<T1>*>::iterator i = cSubDirectories.begin(); i != cSubDirectories.end(); i++) {
      std::vector<T1*> mSubVector = i->second->getAllVector();
      for (unsigned int j = 0; j < mSubVector.size(); j++) {
        mVector.push_back(mSubVector[j]);
      }
    }
    return mVector;
  }
  
  void debug() {
    for (typename std::map<std::string, T1*>::iterator i = cObjects.begin(); i != cObjects.end(); i++) {
      std::cout << "  Object found: " << i->first << std::endl;
    }
    for (typename std::map<std::string, Registry<T1>*>::iterator i = cSubDirectories.begin(); i != cSubDirectories.end(); i++) {
      std::cout << "Entering module: " << i->first << std::endl;
      i->second->debug();
      std::cout << "Leaving module" << std::endl;
    }
  }
};

#endif

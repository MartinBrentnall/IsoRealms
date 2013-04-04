#ifndef TIME_LIMITS_H
#define TIME_LIMITS_H

#include <IsoRealms/IPlugin.h>
#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>

#include "ILock.h"
#include "TimeLimit.h"

class TimeLimits:public IPlugin,
                 public ILock {
  private:
  std::vector<TimeLimit*> cTimers;
  unsigned int cLocks;
    
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  void addLock();
  void removeLock();
  bool isLocked();
};

#endif

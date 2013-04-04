#ifndef I_PROJECT_MANAGER_LISTENER_H
#define I_PROJECT_MANAGER_LISTENER_H

#include <IsoRealms/IProject.h>

class IProjectManagerListener {
  public:
  virtual void projectOpened(IProject*) = 0;
};

#endif

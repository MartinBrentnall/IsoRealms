#ifndef I_PROJECT_MANAGER_H
#define I_PROJECT_MANAGER_H

#include <IsoRealms/Resources/IResourceSelector.h>

#include "IProjectManagerListener.h"

class IProjectManager {
  public:
  virtual void addProjectListener(IProjectManagerListener*) = 0;
  virtual IResourceSelector* getResourceSelector() = 0;
};

#endif

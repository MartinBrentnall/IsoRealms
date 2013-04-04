#ifndef SPINDIZZY_CAMERAS_H
#define SPINDIZZY_CAMERAS_H

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/IPlugin.h>

#include "SpindizzyCamera.h"

class SpindizzyCameras:public IPlugin {
  private:
  std::vector<SpindizzyCamera*> cCameras;
  
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*t);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif

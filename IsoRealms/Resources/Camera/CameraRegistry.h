#ifndef CAMERA_REGISTRY_H
#define CAMERA_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "ICamera.h"
#include "WrappedCamera.h"

class CameraRegistry:public ResourceRegistry<ICamera> {
  private:
  ICamera* cEditingCamera;
  std::map<ICamera*, ICamera*> cCameras;
  
  public:
  CameraRegistry();
    
  void setEditingCamera(ICamera*);  
  ICamera* wrapResource(ICamera*);
  ICamera* unwrapResource(ICamera*);
};

#endif

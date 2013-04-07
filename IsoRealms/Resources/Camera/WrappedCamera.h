#ifndef WRAPPER_CAMERA_H
#define WRAPPER_CAMERA_H

#include "ICamera.h"

class WrappedCamera:public ICamera {
  private:
  ICamera* cCamera;
  
  public:
  WrappedCamera(ICamera*);

  /**********************\
   * Implements ICamera *
  \**********************/
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  float getAngle();
  float getTilt();
  void update(int);
  void render();
};

#endif

#ifndef DUMMY_CAMERA_H
#define DUMMY_CAMERA_H

#include "../../Global/DummyPlugin.h"

#include "ICamera.h"

class DummyCamera:public ICamera,
                  public DummyPlugin {
  public:

  /**********************\
   * Implements ICamera *
  \**********************/
  float getAngle();
  float getTilt();
};

#endif

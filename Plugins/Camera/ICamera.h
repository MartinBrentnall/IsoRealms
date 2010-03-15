#ifndef I_CAMERA_H
#define I_CAMERA_H

#include "../../Global/IPlugin.h"

class ICamera:public virtual IPlugin {
  public:
  virtual float getAngle() = 0;
  virtual float getTilt() = 0;
};

#endif

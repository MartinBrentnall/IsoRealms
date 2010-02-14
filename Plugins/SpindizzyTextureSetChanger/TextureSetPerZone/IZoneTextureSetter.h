#ifndef I_ZONE_TEXTURE_SETTER_H
#define I_ZONE_TEXTURE_SETTER_H

#include "../../SpindizzyTextureSet/ISpindizzyTextureSet.h"

class IZoneTextureSetter {
  public:
  virtual void setTextureSet(ISpindizzyTextureSet*) = 0;
};

#endif

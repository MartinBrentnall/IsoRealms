#ifndef I_ZONE_COLLECTABLES_H
#define I_ZONE_COLLECTABLES_H

#include <IsoRealms/IZone.h>

class IZoneCollectables {
  public:
  virtual IZone* getEditingZone() = 0;
  virtual IZone* getRuntimeZone() = 0;
};

#endif

#ifndef I_ZONE_CONTEXT_LISTENER_H
#define I_ZONE_CONTEXT_LISTENER_H

class IZoneContextListener {
  public:
  virtual void zoneContextChanged(IZone*) = 0;
};

#endif

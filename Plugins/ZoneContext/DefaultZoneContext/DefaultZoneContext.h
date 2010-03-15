#ifndef DEFAULT_ZONE_CONTEXT_H
#define DEFAULT_ZONE_CONTEXT_H

#include "../IZoneContext.h"

class DefaultZoneContext:public IZoneContext {
  private:
  std::vector<IZoneContextListener*> cListeners;
  IZone* cContext;

  public:

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();

  /***************************\
   * Implements IZoneContext *
  \***************************/
  void setZoneContext(IZone*);
  IZone* getZoneContext();
  void addZoneContextListener(IZoneContextListener*);
  void removeZoneContextListener(IZoneContextListener*);
};

#endif

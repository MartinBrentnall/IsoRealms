#ifndef ZONE_OUTLINE_RENDERER_H
#define ZONE_OUTLINE_RENDERER_H

#include <map>
#include <set>

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>

class ZoneOutlineRenderer:public IZoneHandler {
  private:
  IColour* cColour;
  
  public:
    
  /**************************\
   * Implement IZoneHandler *
  \**************************/
  void render(std::vector<IZone*>&, IProject*);
  void update(std::vector<IZone*>&, unsigned int);
  void updateRuntime(std::vector<IZone*>&, unsigned int);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif

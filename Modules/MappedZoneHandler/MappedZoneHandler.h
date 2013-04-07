#ifndef MAPPED_ZONE_HANDLER_H
#define MAPPED_ZONE_HANDLER_H

#include <map>
#include <set>

#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>
#include <IsoRealms/Map.h>

class MappedZoneHandler:public IZoneHandler {
  private:
  std::vector<IZoneHandler*> cDefaultHandlers;
  std::map<IZone*, std::set<IZoneHandler*> > cMappedHandlers;
  
  public:
  void setDefaultHandlers(IZone*);
    
  bool addHandler(IZone*, IZoneHandler*);
  void removeHandler(IZone*, IZoneHandler*);
    
  /**************************\
   * Implement IZoneHandler *
  \**************************/
  void render(std::vector<IZone*>&, IProject*);
  void updateEditing(std::vector<IZone*>&, unsigned int);
  void updateRuntime(std::vector<IZone*>&, unsigned int);

  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void save(DOMNodeWriter*, IResourceLocator*);
};

#endif

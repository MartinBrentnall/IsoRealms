#ifndef ZONE_MODEL_RENDERER_H
#define ZONE_MODEL_RENDERER_H

#include <map>

#include <IsoRealms/IsoRealmsConstants.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/ZoneHandler/IZoneHandler.h>

class ZoneModelRenderer:public IZoneHandler {
  private:
  std::map<IZone*, I3DModel*> cZoneModels;
  I3DModelFactory* cModelType;
  
  I3DModel* createModel(IZone*);
  
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

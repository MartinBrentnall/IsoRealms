#ifndef I_PROJECT_H
#define I_PROJECT_H

#include "BlockLocation.h"
#include "Persistence/DOMNodeWrapper.h"
#include "IEditingContext.h"
#include <IsoRealms/Resources/ElementType/IElement.h>
#include "IZone.h"

class ElementSetRegistry;
class IResourceManager;
class IZoneHandler;
class Map;
class PluginRegistry;
class Zone;

class IProject {
  public:
  virtual void initPlugins(IZone*, unsigned int) = 0;
  virtual void renderPreZone(IZone*) = 0;
  virtual void savePluginData(DOMNodeWriter*, IMap*, IZone*) = 0;

  virtual bool* registerDigitalInput(const std::string&) = 0;
  
  // TODO: These were needed for Zone.  Investigate whether a separate interface may be more appropriate
  virtual void zoneContextChanged(IMap*, IZone*) = 0;
  virtual void loadPluginData(DOMNodeWrapper*, IZone*) = 0;

  // TODO: These required by GERALD.  Investigate whether a separate interface may be more appropriate
  virtual IZone* getZone(Vertex&) = 0;
  virtual std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&) = 0;
  
  
  virtual std::vector<IZone*> getAdjacentZones(IZone*) = 0;

  // TODO: Editor functions should probably not be here
  virtual void addZone(Zone*) = 0;
  virtual void removeZone(Zone*) = 0;
  virtual void removeElement(IElement*) = 0;

  virtual void update(unsigned int) = 0;
  virtual void render() = 0;
  virtual void renderEditing() = 0;
  virtual Zone* getZone(BlockLocation&) = 0;
  virtual Map* getMap() = 0;
  virtual void save() = 0;
  virtual PluginRegistry* getPluginRegistry() = 0;
  virtual IResourceManager* getResourceManager() = 0;
  
  virtual void setEditingContext(IEditingContext*) = 0;
  virtual void staticChanged() = 0;
  
  /*************\
   * Lua API's *
  \*************/
  virtual void setZoneHandler(IZoneHandler*) = 0;
  virtual float getEast() = 0;
  virtual float getWest() = 0;
  virtual float getNorth() = 0;
  virtual float getSouth() = 0;
  virtual float getTop() = 0;
  virtual float getBottom() = 0;
  virtual float getAspectRatio() = 0;
  virtual int getZoneCount() = 0;
};

#endif

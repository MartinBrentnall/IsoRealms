#ifndef I_PROJECT_H
#define I_PROJECT_H

#include "BlockLocation.h"
#include "DOMNodeWrapper.h"
#include "IElement.h"
#include "IElementContainer.h"
#include "IElementSet.h"
#include "IPluginRegistryListener.h"
#include "IZone.h"

class ElementSetRegistry;
class IZoneRenderer;
class Map;

class IProject {
  public:
  virtual std::vector<IElement*> loadElements(DOMNodeWrapper*, BlockLocation*, IElementContainer*) = 0;
  virtual IZoneRenderer* getZoneRenderer(DOMNodeWrapper*) = 0;
  virtual void initPlugins(IZone*, unsigned int) = 0;
  virtual void renderPreZone(IZone*) = 0;
  virtual void initElementsComplete() = 0;
  virtual std::string getInstanceName(IElementSet*) = 0;
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
  virtual void addPluginRegistryListener(IPluginRegistryListener*) = 0;
  virtual void save() = 0;
  virtual PluginRegistry* getPluginRegistry() = 0;
  virtual ElementSetRegistry* getElementSetRegistry() = 0;
};

#endif

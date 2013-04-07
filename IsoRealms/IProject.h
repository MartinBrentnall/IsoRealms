#ifndef I_PROJECT_H
#define I_PROJECT_H

#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

#include "BlockLocation.h"
#include "Persistence/DOMNodeWrapper.h"
#include "IEditingContext.h"
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

  // TODO: These were needed for Zone.  Investigate whether a separate interface may be more appropriate
  virtual void zoneContextChanged(IMap*, IZone*) = 0;
  virtual void loadPluginData(DOMNodeWrapper*, IZone*) = 0;

  // TODO: These required by GERALD.  Investigate whether a separate interface may be more appropriate

  // TODO: Editor functions should probably not be here
  virtual void removeElement(IElement*) = 0;

  virtual void update(unsigned int) = 0;
  virtual void renderRuntime() = 0;
  virtual void renderEditing() = 0;
  virtual void save() = 0;
  virtual PluginRegistry* getPluginRegistry() = 0;
  virtual IResourceManager* getResourceManager() = 0;
  
  virtual void setEditingContext(IEditingContext*) = 0;
  virtual void staticChanged() = 0;
};

#endif

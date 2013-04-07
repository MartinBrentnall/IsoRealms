#ifndef PROJECT_H
#define PROJECT_H

#include <string>

#include "Configuration.h"
#include "Persistence/DOMNodeWrapper.h"
#include "LuaSupport/IArgumentSource.h"
#include "ICommand.h"
#include "Layer.h"
#include "LuaSupport/ILuaFunctionArgument.h"
#include "LuaSupport/ILuaScript.h"
#include "Input/InputCommands.h"
#include "IProject.h"
#include "LuaSupport/LuaScript.h"
#include "LuaSupport/LuaScriptWithArgs.h"
#include "Map.h"
#include "PluginRegistry.h"
#include "Resources/IResources.h"
#include "Resources/Resources.h"

class Project:public IProject {
  private:
  std::vector<Layer*> cLayers;
  PluginRegistry cPluginRegistry;
  Resources cResources;
  IScript* cInitScript;
  
  std::vector<IDynamicElement*> cDynamicElements;
  
  public:
  Project();
  Project(DOMNodeWrapper*, const std::string&, IEditingContext*, ICamera*);

  void initRuntime();
  void input(SDL_Event&);
  void renderRuntime();
  void renderEditing();
  void update(unsigned int);
  void updateRuntime(unsigned int);

  void initPlugins(IZone*, unsigned int);
  void renderPreZone(IZone*);
  void zoneContextChanged(IMap*, IZone*);
  void loadPluginData(DOMNodeWrapper*, IZone*);
  void savePluginData(DOMNodeWriter*, IMap*, IZone*);
  
  std::vector<IZone*> getAdjacentZones(IZone*);
  
  void save();

  void removeElement(IElement*);
  PluginRegistry* getPluginRegistry();
  IResourceManager* getResourceManager();
  void staticChanged();

  float getAspectRatio();
  void setEditingContext(IEditingContext*);
};

#endif

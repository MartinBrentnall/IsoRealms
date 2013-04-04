#ifndef PROJECT_H
#define PROJECT_H

#include <string>

#include "Configuration.h"
#include "Persistence/DOMNodeWrapper.h"
#include "LuaSupport/IArgumentSource.h"
#include "ICommand.h"
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
  Map* cMap;
  PluginRegistry cPluginRegistry;
  Resources cResources;
  ICamera* cCamera;
  IScript* cInitScript;
  
  InputCommands cInputCommands;

  std::vector<IDynamicElement*> cDynamicElements;
  std::vector<IDynamicElement*> cPostLoopCommands;
  std::vector<IVisualElement*> cPostLoopRenderers;
  std::vector<IInteractiveElement*> cInteractivePlugins;
  
  public:
  Project();
  Project(DOMNodeWrapper*, const std::string&, IEditingContext*);

  void initRuntime();
  void input(SDL_Event&);
  void render();
  void renderEditing();
  void update(unsigned int);
  void updateRuntime(unsigned int);
  
  void executePreLoopCommands(int);
  void executePostLoopCommands(int);
  void executePreLoopRenderers();
  void executePostLoopRenderers();

                // TODO: Some interface?
  bool* registerDigitalInput(const std::string&);
  void initPlugins(IZone*, unsigned int);
  void renderPreZone(IZone*);
  void zoneContextChanged(IMap*, IZone*);
  void loadPluginData(DOMNodeWrapper*, IZone*);
  void savePluginData(DOMNodeWriter*, IMap*, IZone*);
  
  std::vector<IZone*> getAdjacentZones(IZone*);
  
  void save();

  IZone* getZone(Vertex&);
  std::vector<ZoneEvent*> getZoneEvents(Vertex&, Vertex&);

                
  void addZone(Zone*);
  void removeZone(Zone*);
  void removeElement(IElement*);
  Zone* getZone(BlockLocation&);
  Map* getMap();
  PluginRegistry* getPluginRegistry();
  IResourceManager* getResourceManager();
  void staticChanged();
  
  void setZoneHandler(IZoneHandler*);
  float getEast();
  float getWest();
  float getNorth();
  float getSouth();
  float getTop();
  float getBottom();
  float getAspectRatio();
  int getZoneCount();
  void setEditingContext(IEditingContext*);
};

#endif

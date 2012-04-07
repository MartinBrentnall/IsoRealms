#ifndef PROJECT_H
#define PROJECT_H

#include <string>

#include "ColourProxy.h"
#include "CommandProxy.h"
#include "DOMNodeWrapper.h"
#include "ElementSetRegistry.h"
#include "I3DModel.h"
#include "I3DModelFactory.h"
#include "IColour.h"
#include "ICommand.h"
#include "IElementRegistryListener.h"
#include "IInteger.h"
#include "ILuaFunctionArgument.h"
#include "ILuaScript.h"
#include "InputCommands.h"
#include "IntegerProxy.h"
#include "IPluginRegistryListener.h"
#include "IProject.h"
#include "IResources.h"
#include "ISound.h"
#include "ITexture.h"
#include "LuaArgument.h"
#include "LuaIntegerArgument.h"
#include "LuaScript.h"
#include "LuaScriptProxy.h"
#include "Map.h"
#include "ModelFactoryProxy.h"
#include "PluginRegistry.h"
#include "Registry.h"
#include "SoundProxy.h"
#include "TextureProxy.h"

class Project:public IProject,
              public IResources,
              public IPluginRegistryListener {
  private:
  bool cEditing;
  Map* cMap;
  PluginRegistry cPluginRegistry;
  ElementSetRegistry cElementSetRegistry;
  Registry<ILuaScript, LuaScriptProxy> cScriptRegistry;
  Registry<ICommand, CommandProxy> cCommandRegistry;
  Registry<IColour, ColourProxy> cColourRegistry;
  Registry<ITexture, TextureProxy> cTextureRegistry;
  Registry<I3DModelFactory, ModelFactoryProxy> c3DModelRegistry;
  Registry<ISound, SoundProxy> cSoundRegistry;
  Registry<IInteger, IntegerProxy> cIntegerRegistry;
  InputCommands cInputCommands;

  std::vector<IDynamicElement*> cPreLoopCommands;
  std::vector<IDynamicElement*> cPostLoopCommands;
  std::vector<IVisualElement*> cPreLoopRenderers;
  std::vector<IVisualElement*> cPostLoopRenderers;
  std::vector<IInteractiveElement*> cInteractivePlugins;
  
  /**
   * Register listeners on registries.
   */
  void registerListeners();
  
  template <class T> T* getDirectory(T*, std::vector<std::string>);

  void loadScript(DOMNodeWrapper*);
  
  public:
  Project();
  Project(DOMNodeWrapper*, IPluginRegistryListener*, IElementRegistryListener*, const std::string&, bool);

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
  
  /*************************\
   * Implements IResources *
  \*************************/
  IProject* getProject();
  bool isEditing();
  I3DModel* getModel(DOMNodeWrapper*, Vertex*);
  I3DModel* getModel(const std::string&, Vertex*);  
  IColour* getColour(DOMNodeWrapper*);
  ILuaScript* getLuaScript(const std::string&);
  Script* getScript(DOMNodeWrapper*);
  ITexture* getTexture(DOMNodeWrapper*);
  ISound* getSound(DOMNodeWrapper*);
  IInteger* getInteger(const std::string&);
  void add(ICommand*, std::vector<std::string>, std::string);
  void add(IColour*, std::vector<std::string>, std::string);
  void add(ITexture*, std::vector<std::string>, std::string);
  void add(I3DModelFactory*, std::vector<std::string>, std::string);
  void add(ISound*, std::vector<std::string>, std::string);
  void add(IInteger*, std::vector<std::string>, std::string);

  /**************************************\
   * Implements IPluginRegistryListener *
  \**************************************/
  void pluginInstanceAdded(PluginRegistry*, std::string, std::string);
  void pluginInstanceRemoved(IPlugin*, std::string);

                // TODO: Some interface?
  bool* registerDigitalInput(const std::string&);
  void initPlugins(IZone*, unsigned int);
  void renderPreZone(IZone*);
  void initElementsComplete();
  void zoneContextChanged(IMap*, IZone*);
  void loadPluginData(DOMNodeWrapper*, IZone*);
  std::vector<IElement*> loadElements(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  IZoneRenderer* getZoneRenderer(DOMNodeWrapper*);
  std::string getInstanceName(IElementSet*);
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
  void addPluginRegistryListener(IPluginRegistryListener*);
  PluginRegistry* getPluginRegistry();
  ElementSetRegistry* getElementSetRegistry();
};

#endif

#include "Project.h"

Project::Project() {
  cResources.setEditing(true);
  cResources.setProject(this);
  cMap = new Map(this);
  cCamera = NULL;
}

Project::Project(DOMNodeWrapper* node, const std::string& projectName, IEditingContext* editingContext) {
  cResources.setEditing(editingContext != NULL);
  cResources.setProject(this);
  cMap = NULL;
  cCamera = NULL;
  cInitScript = NULL;
  std::size_t mExtensionPosition = projectName.find_last_of('.');
  std::string mProjectName = projectName.substr(0, mExtensionPosition);

  std::map<std::string, std::string> mDefaultElementGroups;
  
  /*
   * First pass only loads plugin instances; we need to make sure all plugins
   * are available before we start connecting them together
   */
  std::cout << "Loading modules and Creating resources..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.registerPlugin(mNode, &cResources);
    } else if (mValueAsString == "Script") {
      cResources.loadScript(mNode);
    } else if (mValueAsString == "GlobalVariable") {
      cResources.registerGlobalVariable(mNode);
    } else if (mValueAsString == "DefaultElementGroup") {
      cResources.registerDefaultElementGroup(mNode);
    } else {
      // TODO: Throw something
    }
  }
  cResources.initialise();
  
  std::cout << "Configuring modules (old method)..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Configuration") {
      std::string mCameraPath = mNode->getAttribute("camera");
      cCamera = cResources.getCamera(mCameraPath);
      if (cCamera == NULL) {
	std::cout << "Project does not have a camera and cannot be run" << std::endl;
	exit(1);
      }
    } else if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = mProjectName + "/controls.config";
      std::string mGlobalConfigurationFile = "controls.config";
      std::vector<std::string> mConfigFiles;
      mConfigFiles.push_back(mProjectConfigurationFile);
      mConfigFiles.push_back(mGlobalConfigurationFile);
      cInputCommands.loadConfiguration(mNode, mConfigFiles, &cResources);
    } else if (mValueAsString == "InitScript") {
      cInitScript = cResources.getLuaScript(mNode, NULL);
    } else if (mValueAsString == "Map") {
      cMap = new Map(mNode, editingContext != NULL, this, &cResources);
    } else {
      // TODO: Throw something
    }
  }
  cDynamicElements = cResources.getDynamicElements();
  if (cInitScript != NULL) {
    cInitScript->execute();
  }
  std::cout << "Loaded Project!" << std::endl;
}

bool* Project::registerDigitalInput(const std::string& name) {
  return cInputCommands.registerDigitalInput(name);
}

void Project::render() {
  cMap->render();
}

void Project::renderEditing() {
  cMap->renderEditing();
}

void Project::update(unsigned int ticks) {
  cMap->update(ticks);
}

void Project::updateRuntime(unsigned int ticks) {
  cMap->updateRuntime(ticks);
  for (unsigned int i = 0; i < cDynamicElements.size(); i++) {
    cDynamicElements[i]->update(ticks);
  }
}

void Project::initPlugins(IZone* zone, unsigned int pass) {
  cPluginRegistry.initPlugins(zone, pass);
  cResources.initResources(zone, pass);
}

void Project::renderPreZone(IZone* zone) {
  cPluginRegistry.renderPreZone(zone);
}

void Project::input(SDL_Event& event) {
  cInputCommands.input(event);
  for (unsigned int i = 0; i < cInteractivePlugins.size(); i++) {
    cInteractivePlugins[i]->input(event);
  }
  cMap->input(event);
}

void Project::executePreLoopCommands(int ticks) {
  if (cCamera != NULL) {
    cCamera->update(ticks);
  }
}

void Project::executePostLoopCommands(int ticks) {
  for (unsigned int i = 0; i < cPostLoopCommands.size(); i++) {
    cPostLoopCommands[i]->update(ticks);
  }
}

void Project::executePreLoopRenderers() {
  if (cCamera != NULL) {
    cCamera->render();
  }
}

void Project::executePostLoopRenderers() {
  for (unsigned int i = 0; i < cPostLoopRenderers.size(); i++) {
    cPostLoopRenderers[i]->render();
  }
}

void Project::initRuntime() {
  cInteractivePlugins = cPluginRegistry.getInteractiveElements();
  cPostLoopCommands = cPluginRegistry.getPostLoopCommands();
  cPostLoopRenderers = cPluginRegistry.getPostLoopRenderers();
  cMap->initRuntime();
}

void Project::zoneContextChanged(IMap* map, IZone* zone) {
  cPluginRegistry.zoneContextChanged(map, zone);
  cResources.zoneContextChanged(map, zone);
}

void Project::loadPluginData(DOMNodeWrapper* node, IZone* zone) {
  cPluginRegistry.loadPluginData(node, zone);
}

void Project::savePluginData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  cPluginRegistry.saveData(node, map, zone);
}

std::vector<IZone*> Project::getAdjacentZones(IZone* zone) {
  return cMap->getAdjacentZones(zone);
}

void Project::save() {
  DOMNodeWriter* mProjectNode = new DOMNodeWriter("Project");
  DOMNodeWriter* mConfigurationBranch = mProjectNode->addBranch("Configuration");
  if (cCamera != NULL) {
    std::string mCameraPath = cResources.getPath(cCamera);
    mConfigurationBranch->addAttribute("camera", mCameraPath);
  }
  cResources.saveScripts(mProjectNode);
  DOMNodeWriter* mInputConfigurationNode = mProjectNode->addBranch("InputConfiguration");
  cPluginRegistry.save(mProjectNode, &cResources);
  cInputCommands.saveConfiguration(mInputConfigurationNode, &cResources);
  cResources.saveDefaultElementGroups(mProjectNode);
  DOMNodeWriter* mInitScriptNode = mProjectNode->addBranch("InitScript");
  cResources.saveGlobalVariables(mProjectNode);
  cInitScript->save(mInitScriptNode, &cResources);
  cMap->save(mProjectNode, &cResources);
  mProjectNode->save("Test.isorealms");
}

IZone* Project::getZone(Vertex& location) {
  return cMap->getZone(location);
}

std::vector<ZoneEvent*> Project::getZoneEvents(Vertex& startLocation, Vertex& endLocation) {
  return cMap->getZoneEvents(startLocation, endLocation);
}

void Project::addZone(Zone* zone) {
  cMap->addZone(zone);
}

void Project::removeZone(Zone* zone) {
  cMap->removeZone(zone);
}

void Project::removeElement(IElement* element) {
  cMap->removeElement(element);
}

Zone* Project::getZone(BlockLocation& location) {
  return cMap->getZone(location);
}

Map* Project::getMap() {
  return cMap;
}

PluginRegistry* Project::getPluginRegistry() {
  return &cPluginRegistry;
}

IResourceManager* Project::getResourceManager() {
  return &cResources;
}

void Project::staticChanged() {
  cMap->staticChanged();
}

void Project::setZoneHandler(IZoneHandler* zoneHandler) {
  cMap->setZoneHandler(zoneHandler);
}

float Project::getEast() {
  return cMap->getEast();
}

float Project::getWest() {
  return cMap->getWest();
}

float Project::getNorth() {
  return cMap->getNorth();
}

float Project::getSouth() {
  return cMap->getSouth();
}

float Project::getTop() {
  return cMap->getTop();
}

float Project::getBottom() {
  return cMap->getBottom();
}

float Project::getAspectRatio() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  return mScreenConfiguration->getAspectRatio();
}

int Project::getZoneCount() {
  return cMap->getZoneCount();
}

void Project::setEditingContext(IEditingContext* editingContext) {
  cPluginRegistry.setEditingContext(editingContext, &cResources);
}


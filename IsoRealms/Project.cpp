#include "Project.h"

Project::Project() {
  cResources.setEditing(true, this, NULL);
}

Project::Project(DOMNodeWrapper* node, const std::string& projectName, IEditingContext* editingContext, ICamera* editingCamera) {
  cResources.setEditing(editingContext != NULL, this, editingCamera);
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
    } else if (mValueAsString == "EngineData") {
      cResources.loadInstances(mNode);
    } else {
      // TODO: Throw something
    }
  }
  cResources.initialise();
  
  std::cout << "Configuring modules (old method)..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = mProjectName + "/controls.config";
      std::string mGlobalConfigurationFile = "controls.config";
      std::vector<std::string> mConfigFiles;
      mConfigFiles.push_back(mProjectConfigurationFile);
      mConfigFiles.push_back(mGlobalConfigurationFile);
      cResources.loadInputConfiguration(mNode, mConfigFiles);
    } else if (mValueAsString == "InitScript") {
      cInitScript = cResources.getLuaScript(mNode, NULL);
    } else if (mValueAsString == "Layer") {
      cLayers.push_back(new Layer(mNode, &cResources));
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

void Project::renderRuntime() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->renderRuntime();
  }
}

void Project::renderEditing() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->renderEditing();
  }
}

void Project::update(unsigned int ticks) {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->updateEditing(ticks);
  }
}

void Project::updateRuntime(unsigned int ticks) {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->updateRuntime(ticks);
  }
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
  cResources.input(event);
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->input(event);
  }
}

void Project::initRuntime() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->initRuntime();
  }
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

void Project::save() {
  DOMNodeWriter* mProjectNode = new DOMNodeWriter("Project");
  cResources.saveScripts(mProjectNode);
  DOMNodeWriter* mInputConfigurationNode = mProjectNode->addBranch("InputConfiguration");
  cPluginRegistry.save(mProjectNode, &cResources);
  cResources.saveInputConfiguration(mInputConfigurationNode);
  cResources.saveDefaultElementGroups(mProjectNode);
  DOMNodeWriter* mInitScriptNode = mProjectNode->addBranch("InitScript");
  cResources.saveGlobalVariables(mProjectNode);
  cInitScript->save(mInitScriptNode, &cResources);
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->save(mProjectNode, &cResources);
  }
  mProjectNode->save("Test.isorealms");
}

void Project::removeElement(IElement* element) {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->removeElement(element);
  }
}

PluginRegistry* Project::getPluginRegistry() {
  return &cPluginRegistry;
}

IResourceManager* Project::getResourceManager() {
  return &cResources;
}

void Project::staticChanged() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->staticChanged();
  }
}

void Project::setEditingContext(IEditingContext* editingContext) {
  cPluginRegistry.setEditingContext(editingContext, &cResources);
}


/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Map.h"

Map::Map() {
  registerListeners();
  cZoneRenderers.push_back(new DefaultZoneRenderer());
}

Map::Map(DOMNodeWrapper* node, IPluginRegistryListener* pluginRegistryListener, IElementRegistryListener* elementRegistryListener, const std::string& projectName) {
  if (pluginRegistryListener != NULL) {
    cPluginRegistry.addListener(pluginRegistryListener);
  }
  if (elementRegistryListener != NULL) {
    cElementSetRegistry.addElementRegistryListener(elementRegistryListener);
  }
  BlockLocation mStartLocation(0, 0, 0);

  /*
   * First pass only loads plugin instances; we need to make sure all plugins
   * are available before we start connecting them together
   */
  std::cout << "Loading plugins..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.registerPlugin(mNode, &cCommandRegistry, this);
    } else if (mValueAsString == "ElementSet") {
      cElementSetRegistry.registerElementSet(mNode, &cCommandRegistry);
    } else {
      // TODO: Throw something
    }
  }

  std::cout << "Connecting plugins..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.connectPlugin(mNode);
    } else if (mValueAsString == "ElementSet") {
      cElementSetRegistry.connectPlugin(&cPluginRegistry, mNode);
    } else {
      // TODO: Throw something
    }
  }
  
  std::cout << "Loading plugin configurations..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Elements") {
      cElements = cElementSetRegistry.loadElements(mNode, &mStartLocation, this);
      cDirtyElements = cElements;
    } else if (mValueAsString == "ElementSet") {
      cElementSetRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "Plugin") {
      cPluginRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "Plugin") {
      cElementSetRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "ZoneRenderer") {
      IZoneRenderer* mZoneRenderer = cPluginRegistry.getZoneRenderer(mNode, &cCommandRegistry);
      cZoneRenderers.push_back(mZoneRenderer);
    } else if (mValueAsString == "Zone") {
      Zone* mZone = new Zone(mNode, cElementSetRegistry, cPluginRegistry);
      addZone(mZone);
    } else if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = System::getProjectResource(projectName, "controls.config");
      std::string mGlobalConfigurationFile = System::getResource("controls.config");
      std::vector<DOMNodeWrapper*> mConfigNodes;
      DOMNodeWrapper* mProjectConfigurationRoot = new DOMNodeWrapper(mProjectConfigurationFile);
      DOMNodeWrapper* mGlobalConfigurationRoot = new DOMNodeWrapper(mGlobalConfigurationFile);
      mConfigNodes.push_back(getConfigurationNode(mProjectConfigurationRoot));
      mConfigNodes.push_back(getConfigurationNode(mGlobalConfigurationRoot));
      cInputCommands.loadConfiguration(mNode, mConfigNodes, &cCommandRegistry);
    } else {
      // TODO: Throw something
    }
  }
  registerListeners();
  std::cout << "Done!" << std::endl;
}

DOMNodeWrapper* Map::getConfigurationNode(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      return mNode;
    }
  }
  return NULL;
}

void Map::registerListeners() {
  cPluginRegistry.addListener(this);
//  cElementSetRegistry.addElementRegistryListener(this);
}

void Map::addZone(Zone* zone) {
  cDirtyZones.push_back(zone);
  cZones.push_back(zone);
  zone->addChangeListener(this);
}

Zone* Map::getZone(BlockLocation& location) {
  // TODO: Divide map into abstract segments using a B-tree in order to do more efficient searching
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->contains(location)) {
      return cZones[i];
    }
  }
  return NULL;
}

IZone* Map::getZone(Vertex& location) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->contains(location)) {
      return cZones[i];
    }
  }
  return NULL;
}

std::vector<ZoneEvent*> Map::getZoneEvents(Vertex& start, Vertex& end) {
  std::vector<ZoneEvent*> mAllZoneEvents;
  // TODO: Divide map into abstract segments using a B-tree in order to do more efficient searching
  for (unsigned int i = 0; i < cZones.size(); i++) {
    std::vector<ZoneEvent*> mZoneEvents = cZones[i]->getZoneEvents(start, end);
    for (unsigned int j = 0; j < mZoneEvents.size(); j++) {
      bool mInserted = false;
      for (unsigned int k = 0; k < mAllZoneEvents.size(); k++) {
        if (*mZoneEvents[j] < *mAllZoneEvents[k]) {
          mAllZoneEvents.insert(mAllZoneEvents.begin() + k, 1, mZoneEvents[j]);
          mInserted = true;
          break;
        }
      }
      if (!mInserted) {
        mAllZoneEvents.push_back(mZoneEvents[j]);
      }
    }
  }
  return mAllZoneEvents;
}

bool* Map::registerDigitalInput(const std::string& name) {
  return cInputCommands.registerDigitalInput(name);
}

std::vector<IZone*> Map::getAdjacentZones(IZone* zone) {
  std::vector<IZone*> mAdjacentZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->isAdjacent(*(dynamic_cast<Zone*>(zone)))) { // TODO: THIS DYNAMIC CAST DEFINITELY SHOULD NOT BE HERE
      mAdjacentZones.push_back(cZones[i]);
    }
  }
  return mAdjacentZones;
}

bool Map::overlaps(BlockArea& blockArea) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->overlaps(blockArea)) {
      return true;
    }
  }
  return false;
} 

int Map::getZoneIndex(IZone* zone) {
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

int Map::getElementIndex(IElement* element) {
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return i;
    }
  }
  // TODO: Throw exception
  return -1;
}

void Map::notifyZoneAction(Zone* zone) {
  cPluginRegistry.notifyZoneAction(zone);
}

void Map::initMap(unsigned int pass) {
  std::vector<IZone*> mCleanZones;
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    cPluginRegistry.initPlugins(cDirtyZones[i], pass);
    cPluginRegistry.renderPreZone(cDirtyZones[i]);
    if (cDirtyZones[i]->initZone(pass)) {
      mCleanZones.push_back(cDirtyZones[i]);
    }
  }
  
  for (unsigned int i = 0; i < mCleanZones.size(); i++) {
    int mIndexToRemove = getZoneIndex(mCleanZones[i]);
    cDirtyZones.erase(cDirtyZones.begin() + mIndexToRemove);
  }

  std::vector<IElement*> mCleanElements;
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i]->initElement(pass)) {
      mCleanElements.push_back(cDirtyElements[i]);
    }
  }

  for (unsigned int i = 0; i < mCleanElements.size(); i++) {
    int mIndexToRemove = getElementIndex(mCleanElements[i]);
    cDirtyElements.erase(cDirtyElements.begin() + mIndexToRemove);
  }

  if (cDirtyElements.empty()) {

    // Game rendering
    glDeleteLists(cDisplayList, 1);
    cDisplayList = glGenLists(1);
    glNewList(cDisplayList, GL_COMPILE);
    for (unsigned int i = 0; i < cElements.size(); i++) {
      cElements[i]->renderStatic();
    }
    glEndList();

    // Editor-only rendering
    glDeleteLists(cEditingDisplayList, 1);
    cEditingDisplayList = glGenLists(1);
    glNewList(cEditingDisplayList, GL_COMPILE);
    for (unsigned int i = 0; i < cElements.size(); i++) {
      cElements[i]->renderStaticEditing();
    }
    glEndList();
    
    cElementSetRegistry.initElementsComplete();
  }
  std::cout << "Init map done!" << std::endl;
}

void Map::initRuntime() {
  cInteractivePlugins = cPluginRegistry.getInteractiveElements();
  cPreLoopCommands = cPluginRegistry.getPreLoopCommands();
  cPostLoopCommands = cPluginRegistry.getPostLoopCommands();
  cPreLoopRenderers = cPluginRegistry.getPreLoopRenderers();
  cPostLoopRenderers = cPluginRegistry.getPostLoopRenderers();
  for (unsigned int i = 0; i < cElements.size(); i++) {
    cElements[i]->setRuntimeContext(this);
  }
  initMap();
}

void Map::input(SDL_Event& event) {
  cInputCommands.input(event);
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->input(event);
  }
  for (unsigned int i = 0; i < cInteractivePlugins.size(); i++) {
    cInteractivePlugins[i]->input(event);
  }
  for (unsigned int i = 0; i < cElements.size(); i++) {
    std::vector<IInteractiveElement*> mInteractiveElements = cElements[i]->getInteractiveElements();
    for (unsigned int j = 0; j < mInteractiveElements.size(); j++) {
      mInteractiveElements[j]->input(event);
    }
  }
}

void Map::initMap() {
  unsigned int mInitPass = 0;
  while (!cDirtyZones.empty() || !cDirtyElements.empty()) {
    initMap(mInitPass);
    mInitPass++;
  }
}

void Map::update(int milliseconds) {
  initMap();
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.

  for (unsigned int i = 0; i < cZoneRenderers.size(); i++) {
    cZoneRenderers[i]->update(mZones, milliseconds);
  }
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->update(milliseconds);
  }
}

void Map::updateRuntime(int milliseconds) {
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.
  
  for (unsigned int i = 0; i < cZoneRenderers.size(); i++) {
    cZoneRenderers[i]->updateRuntime(mZones, milliseconds);
  }
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->updateRuntime(milliseconds);
  }
}

void Map::render() {
  glColor3f(1.0f, 1.0f, 1.0f);
  // TODO: Need a more permanent solution for better performance.
  std::vector<IZone*> mZones;
  for (unsigned int i = 0; i < cZones.size(); i++) {
    mZones.push_back(cZones[i]);
  }
  // TODO: End.

  for (unsigned int i = 0; i < cZoneRenderers.size(); i++) {
    cZoneRenderers[i]->render(mZones, cPluginRegistry);
  }
  glCallList(cDisplayList);
  for (unsigned int i = 0; i < cElementHandlers.size(); i++) {
    cElementHandlers[i]->render();
  }
}

void Map::renderEditing() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    cZones[i]->renderEditing();
  }
  glCallList(cEditingDisplayList);
}

void Map::executePreLoopCommands(int ticks) {
  for (unsigned int i = 0; i < cPreLoopCommands.size(); i++) {
    cPreLoopCommands[i]->update(ticks);
  }
}

void Map::executePostLoopCommands(int ticks) {
  for (unsigned int i = 0; i < cPostLoopCommands.size(); i++) {
    cPostLoopCommands[i]->update(ticks);
  }
}

void Map::executePreLoopRenderers() {
  for (unsigned int i = 0; i < cPreLoopRenderers.size(); i++) {
    cPreLoopRenderers[i]->render();
  }
}

void Map::executePostLoopRenderers() {
  for (unsigned int i = 0; i < cPostLoopRenderers.size(); i++) {
    cPostLoopRenderers[i]->render();
  }
}

void Map::zoneChanged(IZone* zone) {
  // TODO: More efficient way of checking if the zone is already dirty
  for (unsigned int i = 0; i < cDirtyZones.size(); i++) {
    if (cDirtyZones[i] == zone) {
      return;
    }
  }
  cDirtyZones.push_back(zone);
}

void Map::save() {
  DOMNodeWriter* mMapNode = new DOMNodeWriter("Map");
  cPluginRegistry.save(mMapNode);
  cElementSetRegistry.save(&cPluginRegistry, mMapNode);
  DOMNodeWriter* mElementsNode = mMapNode->addBranch("Elements");
  BlockLocation mStartLocation(0, 0, 0);
  for (unsigned int i = 0; i < cElements.size(); i++) {
    IElementSet* mElementSet = cElements[i]->getElementSet();
    std::string mElementSetName = cElementSetRegistry.getInstanceName(mElementSet);
    DOMNodeWriter* mElementNode = mElementsNode->addBranch("Element");
    mElementNode->addAttribute("set", mElementSetName);

    // TODO: Enable this! (you'll need to implement some functions)
    IElementFactory* mElementFactory = cElements[i]->getElementFactory();
    std::string mElementTypeName = mElementFactory->getName();
    mElementNode->addAttribute("type", mElementTypeName);
    cElements[i]->save(mElementNode, mStartLocation);
  }
  for (unsigned int i = 0; i < cZones.size(); i++) {
    DOMNodeWriter* mZoneNode = mMapNode->addBranch("Zone");
    DOMNodeWriter* mPluginsNode = mZoneNode->addBranch("Plugins");
    cPluginRegistry.saveData(mPluginsNode, this, cZones[i]);
    cZones[i]->save(&cElementSetRegistry, mZoneNode);
  }
  mMapNode->save("Test.isorealms");
}

void Map::pushElement(IElement* element) {
  element->setElementContainer(this);
  cElements.push_back(element);
  cDirtyElements.push_back(element);
//  zoneChanged();
}

Zone* Map::removeElement(IElement* element) {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->removeElement(element)) {
      zoneChanged(cZones[i]);
      return cZones[i];
    }
  }
  return NULL;
}

ElementSetRegistry* Map::getElementSetRegistry() {
  return &cElementSetRegistry;
}

PluginRegistry* Map::getPluginRegistry() {
  return &cPluginRegistry;
}

CommandDirectory* Map::getCommandRegistry() {
  return &cCommandRegistry;
}

void Map::pluginInstanceAdded(PluginRegistry* registry, std::string, std::string) {
  // Nothing to do.
}

void Map::pluginInstanceRemoved(IPlugin* instance, std::string type) {
  cElementSetRegistry.pluginRemoved(instance);
}

bool Map::containsElement(IElement* element) {
  for (unsigned int i = 0; i < cElements.size(); i++) {
    if (element == cElements[i]) {
      return true;
    }
  }
  return false;
}

void Map::elementDirty(IElement* element) {
  if (!containsElement(element)) {
    std::cout << "WARNING: Specified dirty element is not a member of this Map!" << std::endl;
    std::cout << "WARNING: Map contains " << cElements.size() << std::endl;
    return;
  }
  for (unsigned int i = 0; i < cDirtyElements.size(); i++) {
    if (cDirtyElements[i] == element) {
      return;
    }
  }
  // TODO: Does order matter?
  cDirtyElements.push_back(element);
// TODO  zoneChanged();
}

void Map::addElementHandler(IElementHandler* elementHandler) {
  cElementHandlers.push_back(elementHandler);
}

void Map::setHandlerActive(IElementHandler*, bool) {
  // TODO: Implement this
}

Map::~Map() {
  for (unsigned int i = 0; i < cZones.size(); i++) {
    delete cZones[i];
  }
  cZones.clear();
}

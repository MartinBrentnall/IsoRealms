#include "Project.h"

Project::Project() {
  cEditing = true;
  cMap = new Map();
}

Project::Project(DOMNodeWrapper* node, IPluginRegistryListener* pluginRegistryListener, IElementRegistryListener* elementRegistryListener, const std::string& projectName, bool editing) {
  cEditing = editing;
  cMap = NULL;
  if (pluginRegistryListener != NULL) {
    cPluginRegistry.addListener(pluginRegistryListener);
  }
  if (elementRegistryListener != NULL) {
    cElementSetRegistry.addElementRegistryListener(elementRegistryListener);
  }
  std::size_t mExtensionPosition = projectName.find_last_of('.');
  std::string mProjectName = projectName.substr(0, mExtensionPosition);

  /*
   * First pass only loads plugin instances; we need to make sure all plugins
   * are available before we start connecting them together
   */
  std::cout << "Loading modules..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.registerPlugin(mNode, this);
    } else if (mValueAsString == "ElementSet") {
      cElementSetRegistry.registerElementSet(mNode, this);
    } else if (mValueAsString == "Script") {
      loadScript(mNode);
    } else {
      // TODO: Throw something
    }
  }

  std::cout << "Connecting modules..." << std::endl;
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
  
  std::cout << "Configuring modules..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ElementSet") {
      cElementSetRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "Plugin") {
      cPluginRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = System::getUserResource("Projects/" + mProjectName + "/controls.config");
      std::string mGlobalConfigurationFile = System::getUserResource("controls.config");
      std::vector<std::string> mConfigFiles;
      mConfigFiles.push_back(mProjectConfigurationFile);
      mConfigFiles.push_back(mGlobalConfigurationFile);
      cInputCommands.loadConfiguration(mNode, mConfigFiles, this);
      std::cout << "Loaded input configuration..." << std::endl;
    } else if (mValueAsString == "Map") {
      cMap = new Map(mNode, editing, this);
    } else {
      // TODO: Throw something
    }
  }
  registerListeners();
}

void Project::loadScript(DOMNodeWrapper* node) {
  std::string mFunctionName = node->getAttribute("name");
  LuaScript* mLuaScript = new LuaScript(mFunctionName);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Argument") {
      std::string mType = mNode->getAttribute("type");
      std::string mArgumentName = mNode->getAttribute("name");
      if (mType == "Sound") {
        ISound* mSound = getSound(mNode);
        ILuaFunctionArgument* mArgument = new LuaArgument<ISound>(mArgumentName, mSound);
        mLuaScript->addArgument(mArgument);
      } else if (mType == "Integer") {
        std::string mPath = mNode->getAttribute("instance");
        IInteger* mInteger = getInteger(mPath);
        ILuaFunctionArgument* mArgument = new LuaIntegerArgument(mArgumentName, mInteger);
        mLuaScript->addArgument(mArgument);
      }
    } else if (mValueAsString == "Code") {
      std::string mCode = mNode->getStringValue();
      mLuaScript->setCode(mCode);
    }
  }
  mLuaScript->registerScript();
  cScriptRegistry.add(mLuaScript, mFunctionName);
}

IProject* Project::getProject() {
  return this;
}

bool Project::isEditing() {
  return cEditing;
}

ILuaScript* Project::getLuaScript(const std::string& name) {
  return cScriptRegistry.get(name);
}

Script* Project::getScript(DOMNodeWrapper* node) {
  std::vector<ICommand*> mCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Command") {
      std::string mCommandName = mNode->getStringValue();
      ICommand* mCommand = cCommandRegistry.get(mCommandName);
      mCommands.push_back(mCommand);
    } else {
      // TODO: Throw
    }
  }
  return new Script(mCommands, &cCommandRegistry);
}

IColour* Project::getColour(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  if (mType == "Palette") {
    std::string mColourPath = node->getAttribute("name");
    return cColourRegistry.get(mColourPath);
  } else if (mType == "Absolute") {
    return new Colour(node);
  }
  // TODO: Throw
  return NULL;
}

ITexture* Project::getTexture(DOMNodeWrapper* node) {
  std::string mTexturePath = node->getAttribute("name");
  return cTextureRegistry.get(mTexturePath);
}

I3DModel* Project::getModel(DOMNodeWrapper* node, Vertex* location) {
  std::string mModelPath = node->getAttribute("name");
  return getModel(mModelPath, location);
}

I3DModel* Project::getModel(const std::string& path, Vertex* location) {
  I3DModelFactory* mModelFactory = c3DModelRegistry.get(path);
  return mModelFactory->createModel(location);
}

ISound* Project::getSound(DOMNodeWrapper* node) {
  std::string mSoundPath = node->getAttribute("instance");
  return cSoundRegistry.get(mSoundPath);
}

IInteger* Project::getInteger(const std::string& path) {
  return cIntegerRegistry.get(path);
}

template <class T> T* Project::getDirectory(T* root, std::vector<std::string> location) {
  T* mCurrent = root;
  for (unsigned int i = 0; i < location.size(); i++) {
    T* mSubDirectory = mCurrent->getSubDirectory(location[i]);
    mCurrent = mSubDirectory != NULL ? mSubDirectory : mCurrent->createSubDirectory(location[i]);
  }
  return mCurrent;
}
    
void Project::add(ICommand* command, std::vector<std::string> path, std::string name) {
  Registry<ICommand, CommandProxy>* mDirectory = getDirectory(&cCommandRegistry, path);
  mDirectory->add(command, name);
}

void Project::add(IColour* colour, std::vector<std::string> path, std::string name) {
  Registry<IColour, ColourProxy>* mDirectory = getDirectory(&cColourRegistry, path);
  mDirectory->add(colour, name);
}

void Project::add(ITexture* texture, std::vector<std::string> path, std::string name) {
  Registry<ITexture, TextureProxy>* mDirectory = getDirectory(&cTextureRegistry, path);
  mDirectory->add(texture, name);
}

void Project::add(I3DModelFactory* modelFactory, std::vector<std::string> path, std::string name) {
  Registry<I3DModelFactory, ModelFactoryProxy>* mDirectory = getDirectory(&c3DModelRegistry, path);
  mDirectory->add(modelFactory, name);
}

void Project::add(ISound* sound, std::vector<std::string> path, std::string name) {
  Registry<ISound, SoundProxy>* mDirectory = getDirectory(&cSoundRegistry, path);
  mDirectory->add(sound, name);
}

void Project::add(IInteger* value, std::vector<std::string> path, std::string name) {
  Registry<IInteger, IntegerProxy>* mDirectory = getDirectory(&cIntegerRegistry, path);
  mDirectory->add(value, name);
}

void Project::registerListeners() {
  cPluginRegistry.addListener(this);
//  cElementSetRegistry.addElementRegistryListener(this);
}

void Project::pluginInstanceRemoved(IPlugin* instance, std::string type) {
  cElementSetRegistry.pluginRemoved(instance);
}

void Project::pluginInstanceAdded(PluginRegistry* registry, std::string, std::string) {
  // Nothing to do.
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
}

void Project::initPlugins(IZone* zone, unsigned int pass) {
  cPluginRegistry.initPlugins(zone, pass);
}

void Project::renderPreZone(IZone* zone) {
  cPluginRegistry.renderPreZone(zone);
}

void Project::initElementsComplete() {
  cElementSetRegistry.initElementsComplete();
}

void Project::input(SDL_Event& event) {
  cInputCommands.input(event);
  for (unsigned int i = 0; i < cInteractivePlugins.size(); i++) {
    cInteractivePlugins[i]->input(event);
  }
  cMap->input(event);
}

void Project::executePreLoopCommands(int ticks) {
  for (unsigned int i = 0; i < cPreLoopCommands.size(); i++) {
    cPreLoopCommands[i]->update(ticks);
  }
}

void Project::executePostLoopCommands(int ticks) {
  for (unsigned int i = 0; i < cPostLoopCommands.size(); i++) {
    cPostLoopCommands[i]->update(ticks);
  }
}

void Project::executePreLoopRenderers() {
  for (unsigned int i = 0; i < cPreLoopRenderers.size(); i++) {
    cPreLoopRenderers[i]->render();
  }
}

void Project::executePostLoopRenderers() {
  for (unsigned int i = 0; i < cPostLoopRenderers.size(); i++) {
    cPostLoopRenderers[i]->render();
  }
}

void Project::initRuntime() {
  cInteractivePlugins = cPluginRegistry.getInteractiveElements();
  cPreLoopCommands = cPluginRegistry.getPreLoopCommands();
  cPostLoopCommands = cPluginRegistry.getPostLoopCommands();
  cPreLoopRenderers = cPluginRegistry.getPreLoopRenderers();
  cPostLoopRenderers = cPluginRegistry.getPostLoopRenderers();
  cMap->initRuntime();
}

void Project::zoneContextChanged(IMap* map, IZone* zone) {
  cPluginRegistry.zoneContextChanged(map, zone);
}

void Project::loadPluginData(DOMNodeWrapper* node, IZone* zone) {
  cPluginRegistry.loadPluginData(node, zone);
}

std::vector<IElement*> Project::loadElements(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container) {
  return cElementSetRegistry.loadElements(node, location, container);
}

IZoneRenderer* Project::getZoneRenderer(DOMNodeWrapper* node) {
  return cPluginRegistry.getZoneRenderer(node, &cCommandRegistry);
}

std::string Project::getInstanceName(IElementSet* elementSet) {
  return cElementSetRegistry.getInstanceName(elementSet);
}

void Project::savePluginData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  cPluginRegistry.saveData(node, map, zone);
}

std::vector<IZone*> Project::getAdjacentZones(IZone* zone) {
  return cMap->getAdjacentZones(zone);
}

void Project::save() {
  DOMNodeWriter* mProjectNode = new DOMNodeWriter("Project");
  cPluginRegistry.save(mProjectNode);
  cElementSetRegistry.save(&cPluginRegistry, mProjectNode);
  cPluginRegistry.saveZoneRenderers(mProjectNode);
  cMap->save(mProjectNode);
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

void Project::addPluginRegistryListener(IPluginRegistryListener* listener) {
  cPluginRegistry.addListener(listener);
}

PluginRegistry* Project::getPluginRegistry() {
  return &cPluginRegistry;
}

ElementSetRegistry* Project::getElementSetRegistry() {
  return &cElementSetRegistry;
}

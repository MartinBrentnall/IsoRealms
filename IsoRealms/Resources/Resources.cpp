#include "Resources.h"

Resources::Resources() {
  cEditing = false;
  
  // Allow conversions
  cStrings.setIntegers(&cIntegerRegistry);
  
  // Set dummies
  cCameras.setDummyResource(new DummyCamera());
  cColourRegistry.setDummyResource(new Colour(1.0f, 0.0f, 1.0f));
  cFontRegistry.setDummyResource(new DummyFont());
  cSoundRegistry.setDummyResource(new DummySound());
}

void Resources::setProject(IProject* project) {
  cProject = project;
}

void Resources::setEditing(bool editing) {
  cEditing = editing;
}

void Resources::loadScript(DOMNodeWrapper* node) {
  std::string mFunctionName = node->getAttribute("name");
  LuaScript* mLuaScript = new LuaScript(mFunctionName);
  cScriptRegistry[mFunctionName] = mLuaScript;
  addResourceToInitialise(mLuaScript, node);
}

void Resources::saveScripts(DOMNodeWriter* node) {
  for (std::map<std::string, ILuaScript*>::iterator i = cScriptRegistry.begin(); i != cScriptRegistry.end(); i++) {
    i->second->save(node, this);
  }
}

void Resources::saveDefaultElementGroups(DOMNodeWriter* node) {
  for (std::map<IElementType*, IElementGroupType*>::iterator i = cDefaultElementGroups.begin(); i != cDefaultElementGroups.end(); i++) {
    DOMNodeWriter* mDefaultElementGroupNode = node->addBranch("DefaultElementGroup");
    std::string mElementType = getPath(i->first);
    std::string mElementGroupType = getPath(i->second);
    mDefaultElementGroupNode->addAttribute("element", mElementType);
    mDefaultElementGroupNode->addAttribute("group", mElementGroupType);
  }
}

void Resources::saveGlobalVariables(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cGlobalVariables.size(); i++) {
    cGlobalVariables[i]->save(node, this);
  }
}

void Resources::registerGlobalVariable(DOMNodeWrapper* node) {
  LuaGlobalVariable* mGlobalVariable = new LuaGlobalVariable();
  cGlobalVariables.push_back(mGlobalVariable);
  addResourceToInitialise(mGlobalVariable, node);
}

void Resources::registerDefaultElementGroup(DOMNodeWrapper* node) {
  std::string mElementGroupName = node->getAttribute("group");
  std::string mElementTypeName = node->getAttribute("element");
  IElementGroupType* mElementGroup = cElementGroupTypeRegistry.get(mElementGroupName);
  IElementType* mElementType = cElementTypeRegistry.get(mElementTypeName);
  cDefaultElementGroups[mElementType] = mElementGroup;
}

void Resources::initialise() {
  for (unsigned int i = 0; i < cResourcesToInitialise.size(); i++) {
    cResourcesToInitialise[i]->initialiseResource(this);
    delete cResourcesToInitialise[i];
  }
  cResourcesToInitialise.clear();
}

std::vector<IDynamicElement*> Resources::getDynamicElements() {
  return cDynamicElements;
}

void Resources::zoneContextChanged(IMap* map, IZone* zone) {
  for (unsigned int i = 0; i < cZoneContextListeners.size(); i++) {
    cZoneContextListeners[i]->zoneContextChanged(map, zone);
  }
}
  
void Resources::initResources(IZone* zone, unsigned int pass) { // TODO: THIS IS DEFINITELY WRONG
  for (unsigned int i = 0; i < cZoneContextListeners.size(); i++) {
    cZoneContextListeners[i]->zoneContextChanged(NULL, zone);
  }
}
  
IProject* Resources::getProject() {
  return cProject;
}

bool Resources::isEditing() {
  return cEditing;
}

IScript* Resources::getLuaScript(DOMNodeWrapper* node, IArgumentGenerator* localArgs) {
  std::string mScriptName = node->getAttribute("name");
  ILuaScript* mScript = cScriptRegistry.find(mScriptName)->second;
  return new LuaScriptWithArgs(mScript, node, this, localArgs);
}


I3DModel* Resources::getModel(const std::string& path, Vertex* location, float scale) {
  if (path[0] == ':') {
    std::size_t mSplit = path.find('/');
    std::string mType = path.substr(1, mSplit - 1);
    if (mType == "Model") {
      std::string mModelInstancePath = path.substr(mSplit + 1);
      I3DModel* mModel = getModelInstance(mModelInstancePath);
      return mModel;
    } else {
      std::cout << "Unknown type \"" << mType << "\"" << std::endl;
      exit(1);
    }
  }
  I3DModelFactory* mModelFactory = c3DModelFactoryRegistry.get(path);
  I3DModel* mModel = mModelFactory->createModel(location, scale);
  if (mModel != NULL) {
    std::vector<I3DModel*> mModels = cInstantiatedModels[path];
    mModels.push_back(mModel);
    cInstantiatedModels[path] = mModels;
  }
  return mModel;
}



IArgumentDefinition* Resources::getArgumentDefinition(DOMNodeWrapper* node) {
  Configuration* mConfiguration = Configuration::getInstance();
  return mConfiguration->createArgumentDefinition(node, this);
}

IArgumentDefinition* Resources::getRegisteredArgumentDefinition(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::cout << "Creating argument definition from custom type: \"" << mType << "\"..." << std::endl;
  IArgumentDefinitionType* mArgumentSource = cArgumentDefinitionTypes.get(mType);
  return mArgumentSource->createArgumentDefinition(node, this);
}

IArgumentSource* Resources::getArgument(DOMNodeWrapper* node) {
  Configuration* mConfiguration = Configuration::getInstance();
  return mConfiguration->createArgument(node, this);
}

IArgumentSource* Resources::getRegisteredArgument(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mValue = node->getAttribute("value");
  if (mValue[0] == '~') {
    mValue = mValue.substr(1);
  }
  std::cout << "Getting custom argument: \"" << mType << "/" << mValue + "\"..." << std::endl;
  return cArgumentValues.get(mType + "/" + mValue);
}

I3DModel* Resources::getModelInstance(const std::string& name)                     {return c3DModelRegistry.get(name);}
I3DModelFactory* Resources::getModelType(const std::string& path)                  {return c3DModelFactoryRegistry.get(path);}
ICamera* Resources::getCamera(const std::string& path)                             {return cCameras.get(path);}
ICollectables* Resources::getCollectablesRegistry(const std::string& path)         {return cCollectablesRegistry.get(path);}
ICollidableSurfaceRegistry* Resources::getSurfaceRegistry(const std::string& path) {return cSurfaceRegistries.get(path);}
IColour* Resources::getColour(const std::string& name)                             {return cColourRegistry.get(name);}
IComponentCustomType* Resources::getComponentCustomType(const std::string& path)   {return cComponentCustomTypes.get(path);}
IFloat* Resources::getFloat(const std::string& path)                               {return cFloatRegistry.get(path);}
IFont* Resources::getFont(const std::string& path)                                 {return cFontRegistry.get(path);}
IHUDComponentFactory* Resources::getHUDComponentType(const std::string& path)      {return cHUDComponentRegistry.get(path);}
IInteger* Resources::getInteger(const std::string& path)                           {return cIntegerRegistry.get(path);}
ISound* Resources::getSound(const std::string& path)                               {return cSoundRegistry.get(path);}
IString* Resources::getString(const std::string& path)                             {return cStrings.get(path);}
ISurfaceProcessor* Resources::getSurfaceProcessor(const std::string& path)         {return cSurfaceProcessors.get(path);}
ITexture* Resources::getTexture(const std::string& name)                           {return cTextureRegistry.get(name);}
IVertex* Resources::getLocation(const std::string& path)                           {return cLocations.get(path);}
IZoneHandler* Resources::getZoneHandler(const std::string& path)                   {return cZoneHandlers.get(path);}

void Resources::notifyChange(IColour* colour)   {cColourRegistry.notifyChange(colour);}
void Resources::notifyChange(ITexture* texture) {cTextureRegistry.notifyChange(texture);}

std::vector<I3DModelFactory*> Resources::getAllModelTypes()                   {return c3DModelFactoryRegistry.getAllVector();}
std::vector<IBoolean*> Resources::getAllBooleans()                            {return cBooleanRegistry.getAllVector();}
std::vector<ICamera*> Resources::getAllCameras()                              {return cCameras.getAllVector();}
std::vector<ICollidableSurfaceRegistry*> Resources::getAllSurfaceRegistries() {return cSurfaceRegistries.getAllVector();}
std::vector<ICollectables*> Resources::getAllCollectableRegistries()          {return cCollectablesRegistry.getAllVector();}
std::vector<IColour*> Resources::getAllColours()                              {return cColourRegistry.getAllVector();}
std::vector<IComponentCustomType*> Resources::getAllComponentCustomTypes()    {return cComponentCustomTypes.getAllVector();}
std::vector<IElementType*> Resources::getAllElementTypes()                    {return cElementTypeRegistry.getAllVector();}
std::vector<IFloat*> Resources::getAllFloats()                                {return cFloatRegistry.getAllVector();}
std::vector<IFont*> Resources::getAllFonts()                                  {return cFontRegistry.getAllVector();}
std::vector<IHUDComponentFactory*> Resources::getAllHUDComponentTypes()       {return cHUDComponentRegistry.getAllVector();}
std::vector<IInteger*> Resources::getAllIntegers()                            {return cIntegerRegistry.getAllVector();}
std::vector<ISound*> Resources::getAllSounds()                                {return cSoundRegistry.getAllVector();}
std::vector<IString*> Resources::getAllStrings()                              {return cStrings.getAllVector();}
std::vector<ISurfaceProcessor*> Resources::getAllSurfaceProcessors()          {return cSurfaceProcessors.getAllVector();}
std::vector<ITexture*> Resources::getAllTextures()                            {return cTextureRegistry.getAllVector();}
std::vector<IVertex*> Resources::getAllLocations()                            {return cLocations.getAllVector();}
std::vector<IZoneHandler*> Resources::getAllZoneHandlers()                    {return cZoneHandlers.getAllVector();}

void Resources::addResourceListener(IResourceListener<I3DModelFactory>* listener)            {c3DModelFactoryRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IBoolean>* listener)                   {cBooleanRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ICamera>* listener)                    {cCameras.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ICollidableSurfaceRegistry>* listener) {cSurfaceRegistries.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ICollectables>* listener)              {cCollectablesRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IColour>* listener)                    {cColourRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IComponentCustomType>* listener)       {cComponentCustomTypes.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IElementType>* listener)               {cElementTypeRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IFloat>* listener)                     {cFloatRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IFont>* listener)                      {cFontRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IHUDComponentFactory>* listener)       {cHUDComponentRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IInteger>* listener)                   {cIntegerRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ISound>* listener)                     {cSoundRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IString>* listener)                    {cStrings.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ISurfaceProcessor>* listener)          {cSurfaceProcessors.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ITexture>* listener)                   {cTextureRegistry.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IVertex>* listener)                    {cLocations.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IZoneHandler>* listener)               {cZoneHandlers.addResourceListener(listener);}

void Resources::addResourceType(IResourceType<I3DModelFactory>* resourceType,            const std::string& typeDescription) {c3DModelFactoryRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IBoolean>* resourceType,                   const std::string& typeDescription) {cBooleanRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ICamera>* resourceType,                    const std::string& typeDescription) {cCameras.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ICollidableSurfaceRegistry>* resourceType, const std::string& typeDescription) {cSurfaceRegistries.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ICollectables>* resourceType,              const std::string& typeDescription) {cCollectablesRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IColour>* resourceType,                    const std::string& typeDescription) {cColourRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IComponentCustomType>* resourceType,       const std::string& typeDescription) {cComponentCustomTypes.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IElementType>* resourceType,               const std::string& typeDescription) {cElementTypeRegistry.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IFloat>* resourceType,                     const std::string& typeDescription) {cFloatRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IFont>* resourceType,                      const std::string& typeDescription) {cFontRegistry.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IHUDComponentFactory>* resourceType,       const std::string& typeDescription) {cHUDComponentRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IInteger>* resourceType,                   const std::string& typeDescription) {cIntegerRegistry.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ISound>* resourceType,                     const std::string& typeDescription) {cSoundRegistry.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IString>* resourceType,                    const std::string& typeDescription) {cStrings.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ISurfaceProcessor>* resourceType,          const std::string& typeDescription) {cSurfaceProcessors.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ITexture>* resourceType,                   const std::string& typeDescription) {cTextureRegistry.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IVertex>* resourceType,                    const std::string& typeDescription) {cLocations.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IZoneHandler>* resourceType,               const std::string& typeDescription) {cZoneHandlers.addResourceType(resourceType, typeDescription);}  

void Resources::editResource(I3DModelFactory* resource,            IResourceAccessor* editingResources) {c3DModelFactoryRegistry.editResource(resource, this);}
void Resources::editResource(IBoolean* resource,                   IResourceAccessor* editingResources) {cBooleanRegistry.editResource(resource, this);}
void Resources::editResource(ICamera* resource,                    IResourceAccessor* editingResources) {cCameras.editResource(resource, this);}
void Resources::editResource(ICollidableSurfaceRegistry* resource, IResourceAccessor* editingResources) {cSurfaceRegistries.editResource(resource, this);}
void Resources::editResource(ICollectables* resource,              IResourceAccessor* editingResources) {cCollectablesRegistry.editResource(resource, this);}
void Resources::editResource(IColour* resource,                    IResourceAccessor* editingResources) {cColourRegistry.editResource(resource, this);}
void Resources::editResource(IComponentCustomType* resource,       IResourceAccessor* editingResources) {cComponentCustomTypes.editResource(resource, this);}
void Resources::editResource(IElementType* resource,               IResourceAccessor* editingResources) {cElementTypeRegistry.editResource(resource, this);}
void Resources::editResource(IFloat* resource,                     IResourceAccessor* editingResources) {cFloatRegistry.editResource(resource, this);}
void Resources::editResource(IFont* resource,                      IResourceAccessor* editingResources) {cFontRegistry.editResource(resource, this);}
void Resources::editResource(IHUDComponentFactory* resource,       IResourceAccessor* editingResources) {cHUDComponentRegistry.editResource(resource, this);}
void Resources::editResource(IInteger* resource,                   IResourceAccessor* editingResources) {cIntegerRegistry.editResource(resource, this);}
void Resources::editResource(ISound* resource,                     IResourceAccessor* editingResources) {cSoundRegistry.editResource(resource, this);}
void Resources::editResource(IString* resource,                    IResourceAccessor* editingResources) {cStrings.editResource(resource, this);}
void Resources::editResource(ISurfaceProcessor* resource,          IResourceAccessor* editingResources) {cSurfaceProcessors.editResource(resource, this);}
void Resources::editResource(ITexture* resource,                   IResourceAccessor* editingResources) {cTextureRegistry.editResource(resource, this);}
void Resources::editResource(IVertex* resource,                    IResourceAccessor* editingResources) {cLocations.editResource(resource, this);}
void Resources::editResource(IZoneHandler* resource,               IResourceAccessor* editingResources) {cZoneHandlers.editResource(resource, this);}

void Resources::removeResource(I3DModelFactory* resource,            IResourceAccessor* editingResources) {c3DModelFactoryRegistry.removeResource(resource, this);}
void Resources::removeResource(IBoolean* resource,                   IResourceAccessor* editingResources) {cBooleanRegistry.removeResource(resource, this);}
void Resources::removeResource(ICamera* resource,                    IResourceAccessor* editingResources) {cCameras.removeResource(resource, this);}
void Resources::removeResource(ICollidableSurfaceRegistry* resource, IResourceAccessor* editingResources) {cSurfaceRegistries.removeResource(resource, this);}
void Resources::removeResource(ICollectables* resource,              IResourceAccessor* editingResources) {cCollectablesRegistry.removeResource(resource, this);}
void Resources::removeResource(IColour* resource,                    IResourceAccessor* editingResources) {cColourRegistry.removeResource(resource, this);}
void Resources::removeResource(IComponentCustomType* resource,       IResourceAccessor* editingResources) {cComponentCustomTypes.removeResource(resource, this);}
void Resources::removeResource(IElementType* resource,               IResourceAccessor* editingResources) {cElementTypeRegistry.removeResource(resource, this);}
void Resources::removeResource(IFloat* resource,                     IResourceAccessor* editingResources) {cFloatRegistry.removeResource(resource, this);}
void Resources::removeResource(IFont* resource,                      IResourceAccessor* editingResources) {cFontRegistry.removeResource(resource, this);}
void Resources::removeResource(IHUDComponentFactory* resource,       IResourceAccessor* editingResources) {cHUDComponentRegistry.removeResource(resource, this);}
void Resources::removeResource(IInteger* resource,                   IResourceAccessor* editingResources) {cIntegerRegistry.removeResource(resource, this);}
void Resources::removeResource(ISound* resource,                     IResourceAccessor* editingResources) {cSoundRegistry.removeResource(resource, this);}
void Resources::removeResource(IString* resource,                    IResourceAccessor* editingResources) {cStrings.removeResource(resource, this);}
void Resources::removeResource(ISurfaceProcessor* resource,          IResourceAccessor* editingResources) {cSurfaceProcessors.removeResource(resource, this);}
void Resources::removeResource(ITexture* resource,                   IResourceAccessor* editingResources) {cTextureRegistry.removeResource(resource, this);}
void Resources::removeResource(IVertex* resource,                    IResourceAccessor* editingResources) {cLocations.removeResource(resource, this);}
void Resources::removeResource(IZoneHandler* resource,               IResourceAccessor* editingResources) {cZoneHandlers.removeResource(resource, this);}

void Resources::add(IResource* resource, DOMNodeWrapper* node) {
  addResourceToInitialise(resource, node);
}

void Resources::add(I3DModel* model,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {c3DModelRegistry.add(model, path, name);                          addResourceToInitialise(model, node);}
void Resources::add(I3DModelFactory* modelType,                      std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {c3DModelFactoryRegistry.add(modelType, path, name);               addResourceToInitialise(modelType, node);}
void Resources::add(IBoolean* value,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cBooleanRegistry.add(value, path, name);                          addResourceToInitialise(value, node);}
void Resources::add(ICamera* camera,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cCameras.add(camera, path, name);                                 addResourceToInitialise(camera, node);}
void Resources::add(ICollectables* collectables,                     std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cCollectablesRegistry.add(collectables, path, name);              addResourceToInitialise(collectables, node);}
void Resources::add(ICollidableSurfaceRegistry* surfaceRegistry,     std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cSurfaceRegistries.add(surfaceRegistry, path, name);              addResourceToInitialise(surfaceRegistry, node);}
void Resources::add(IColour* colour,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cColourRegistry.add(colour, path, name);                          addResourceToInitialise(colour, node);}
void Resources::add(IElementType* type,                              std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cElementTypeRegistry.add(type, path, name);                       addResourceToInitialise(type, node);}
void Resources::add(IFont* font,                                     std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cFontRegistry.add(font, path, name);                              addResourceToInitialise(font, node);}
void Resources::add(IHUDComponentFactory* type,                      std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cHUDComponentRegistry.add(type, path, name);                      addResourceToInitialise(type, node);}
void Resources::add(IInteger* value,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cIntegerRegistry.add(value, path, name);                          addResourceToInitialise(value, node);}
void Resources::add(ISound* sound,                                   std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cSoundRegistry.add(sound, path, name);                            addResourceToInitialise(sound, node);}
void Resources::add(IString* string,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cStrings.add(string, path, name);                                 addResourceToInitialise(string, node);}
void Resources::add(ISurfaceProcessor* surfaceProcessor,             std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cSurfaceProcessors.add(surfaceProcessor, path, name);             addResourceToInitialise(surfaceProcessor, node); cZoneContextListeners.push_back(surfaceProcessor);} // TODO: Shouldn't be done here
void Resources::add(ITexture* texture,                               std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cTextureRegistry.add(texture, path, name);                        addResourceToInitialise(texture, node);}
void Resources::add(IVertex* location,                               std::vector<std::string> path, const std::string& name                      ) {cLocations.add(location, path, name);}
void Resources::add(IZoneHandler* zoneHandler,                       std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cZoneHandlers.add(zoneHandler, path, name);                       addResourceToInitialise(zoneHandler, node);}
void Resources::add(IElementGroupType* type,                         std::vector<std::string> path, const std::string& name                      ) {cElementGroupTypeRegistry.add(type, path, name);}
void Resources::add(IArgumentDefinitionType* argumentDefinitionType, std::vector<std::string> path, const std::string& name                      ) {cArgumentDefinitionTypes.add(argumentDefinitionType, path, name);}
void Resources::add(IComponentCustomType* type,                      std::vector<std::string> path, const std::string& name                      ) {cComponentCustomTypes.add(type, path, name);}

void Resources::addResourceToInitialise(IResource* resource, DOMNodeWrapper* node) {
  if (node != NULL) {
    ResourceInitialiser* mResourceInitialiser = new ResourceInitialiser(resource, node);
    cResourcesToInitialise.push_back(mResourceInitialiser);
  }
}

void Resources::registerArgument(const std::string& type, IArgumentSource* source, std::vector<std::string> path, const std::string& name) {
   path.push_back(type);
   cArgumentValues.add(source, path, name);
}

void Resources::addDynamicElement(IDynamicElement* element) {
  cDynamicElements.push_back(element);
}

void Resources::loadDefaultElementGroups(IElementContainer* container) {
  std::set<IElementGroup*> mElementGroups;
  for (std::map<IElementType*, IElementGroupType*>::iterator i = cDefaultElementGroups.begin(); i != cDefaultElementGroups.end(); i++) {
    if (i->second != NULL) {
      IElementGroup* mElementGroup = i->second->getElementGroup(container);
      cImplicitElements.push_back(mElementGroup);
      mElementGroups.insert(mElementGroup);
    }
  }
  for (std::set<IElementGroup*>::iterator i = mElementGroups.begin(); i != mElementGroups.end(); i++) {
    container->addElement(*i);
  }
}

void Resources::loadElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container) {
  std::string mElementTypePath = node->getAttribute("type");
  IElementType* mElementType = cElementTypeRegistry.get(mElementTypePath);
  IElement* mElement = mElementType->getElement(node, location, container);
  std::map<IElementType*, IElementGroupType*>::iterator i = cDefaultElementGroups.find(mElementType);
  if (i != cDefaultElementGroups.end()) {
    IElementGroup* mElementGroup = i->second->getElementGroup(container);
    mElementGroup->addElement(mElement);
  } else {
    container->addElement(mElement);
  }
}

void Resources::addListener(IResourceUseListener<I3DModelFactory>* listener)            {c3DModelFactoryRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IBoolean>* listener)                   {cBooleanRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ICamera>* listener)                    {cCameras.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ICollectables>* listener)              {cCollectablesRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ICollidableSurfaceRegistry>* listener) {cSurfaceRegistries.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IColour>* listener)                    {cColourRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IComponentCustomType>* listener)       {cComponentCustomTypes.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IElementType>* listener)               {cElementTypeRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IFloat>* listener)                     {cFloatRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IFont>* listener)                      {cFontRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IHUDComponentFactory>* listener)       {cHUDComponentRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IInteger>* listener)                   {cIntegerRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ISound>* listener)                     {cSoundRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IString>* listener)                    {cStrings.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ISurfaceProcessor>* listener)          {cSurfaceProcessors.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ITexture>* listener)                   {cTextureRegistry.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IVertex>* listener)                    {cLocations.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IZoneHandler>* listener)               {cZoneHandlers.addResourceUseListener(listener);}

std::string Resources::getPath(IArgumentSource* argument)                   {return cArgumentValues.getLocation(argument);}
std::string Resources::getPath(I3DModelFactory* modelType)                  {return c3DModelFactoryRegistry.getLocation(modelType);}
std::string Resources::getPath(IBoolean* boolean)                           {return cBooleanRegistry.getLocation(boolean);}
std::string Resources::getPath(ICamera* camera)                             {return cCameras.getLocation(camera);}
std::string Resources::getPath(ICollectables* collectables)                 {return cCollectablesRegistry.getLocation(collectables);}
std::string Resources::getPath(ICollidableSurfaceRegistry* surfaceRegistry) {return cSurfaceRegistries.getLocation(surfaceRegistry);}
std::string Resources::getPath(IColour* colour)                             {return cColourRegistry.getLocation(colour);}
std::string Resources::getPath(IElementGroupType* elementGroupType)         {return cElementGroupTypeRegistry.getLocation(elementGroupType);}
std::string Resources::getPath(IElementType* elementType)                   {return cElementTypeRegistry.getLocation(elementType);}
std::string Resources::getPath(IFloat* afloat)                              {return cFloatRegistry.getLocation(afloat);}
std::string Resources::getPath(IFont* font)                                 {return cFontRegistry.getLocation(font);}
std::string Resources::getPath(IHUDComponentFactory* componentType)         {return cHUDComponentRegistry.getLocation(componentType);}
std::string Resources::getPath(IInteger* integer)                           {return cIntegerRegistry.getLocation(integer);}
std::string Resources::getPath(IProject* project)                           {return "";}
std::string Resources::getPath(ISound* sound)                               {return cSoundRegistry.getLocation(sound);}
std::string Resources::getPath(IString* string)                             {return cStrings.getLocation(string);}
std::string Resources::getPath(ISurfaceProcessor* surfaceProcessor)         {return cSurfaceProcessors.getLocation(surfaceProcessor);}
std::string Resources::getPath(ITexture* texture)                           {return cTextureRegistry.getLocation(texture);}
std::string Resources::getPath(IVertex* vertex)                             {return cLocations.getLocation(vertex);}
std::string Resources::getPath(IZoneHandler* zoneHandler)                   {return cZoneHandlers.getLocation(zoneHandler);}

std::string Resources::getPath(I3DModel* model) {
  std::string mModelInstance = c3DModelRegistry.getLocation(model);
  if (mModelInstance != "") {
    return ":Model/" + mModelInstance;
  }
  for (std::map<std::string, std::vector<I3DModel*> >::iterator i = cInstantiatedModels.begin(); i != cInstantiatedModels.end(); i++) {
    for (unsigned int j = 0; j < i->second.size(); j++) {
      if (model == i->second[j]) {
        return i->first;
      }
    }
  }
  std::cout << "WARNING: Model wasn't found!" << std::endl;
  exit(1);
}

void Resources::saveScript(DOMNodeWriter* node, const std::string& nodeName, IScript* script) {
  if (script != NULL) {
    DOMNodeWriter* mNode = node->addBranch(nodeName);
    script->save(mNode, this);
  }
}

bool Resources::isImplicit(IElement* element) {
  for (unsigned int i = 0; i < cImplicitElements.size(); i++) {
    if (cImplicitElements[i] == element) {
      return true;
    }
  }
  return false;
}

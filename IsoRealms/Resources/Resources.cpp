/*
 * Copyright 2015 Martin Brentnall
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
#include "Resources.h"

Resources::Resources() {
  cEditing = false;
  
  // Allow conversions
  cStrings.setIntegers(&cIntegers);
  
  // Set dummies
  cCameras.setDummyResource(new DummyCamera());
  cColours.setDummyResource(new Colour(1.0f, 0.0f, 1.0f));
  cFonts.setDummyResource  (new DummyFont());
  cSounds.setDummyResource (new DummySound());
}

void Resources::loadInputConfiguration(DOMNodeWrapper* node, std::vector<std::string> configFile) {
  cInputCommands.loadConfiguration(node, configFile, this);
}

void Resources::loadInstance(DOMNodeWrapper* node) {
  std::string mResourceType = node->getAttribute("type");
  std::string mResourceRef = node->getAttribute("ref");
  std::string mResourceName = node->getAttribute("name");
  if (mResourceType == "Model") {
    I3DModelType* mModelType = getModelType(mResourceRef);
    I3DModel* mModelInstance = mModelType->createModel(nullptr, 1.0f);
    std::vector<std::string> mEmptyPath;
    add(mModelInstance, mEmptyPath, mResourceName, nullptr);
  }
}

void Resources::input(SDL_Event& event) {
  cInputCommands.input(event);
}

void Resources::saveInputConfiguration(DOMNodeWriter* node) {
  cInputCommands.saveConfiguration(node, this);
}

void Resources::setEditing(bool editing, IProject* project) {
  cEditing = editing;
//   std::vector<std::string> mRoot;
//   add(new ArgumentDefinitionType<IMap>(), mRoot, "Map");
}

void Resources::reset() {
  std::vector<IBoolean*> mBooleans = cBooleans.getAllVector();
  for (IBoolean* mBoolean : mBooleans) {
    mBoolean->reset();
  }
  std::vector<IInteger*> mIntegers = cIntegers.getAllVector();
  for (IInteger* mInteger : mIntegers) {
    mInteger->reset();
  }
}

// void Resources::loadInstances(DOMNodeWrapper* node) {
//   for (int i = 0; i < node->getChildCount(); i++) {
//     DOMNodeWrapper *mNode = node->getChild(i);
//     std::string mValueAsString = mNode->getNodeName();
//     if (mValueAsString == "Element") {
//       Vertex mVertex;
//       loadElement(mNode, &mVertex, this);
// //       std::vector<std::string> mPath;
// //       RuntimeContext* mRuntimeContext = new RuntimeContext(this, mPath);
// //       cMapType->loadInstance(mNode, mRuntimeContext);
//     }
//   }
// }

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

bool Resources::isEditing() {
  return cEditing;
}

IScriptCall* Resources::getScriptCall(DOMNodeWrapper* node, IArgumentValueRegistry* localArgs) {
  std::string mScriptName = node->getAttribute("name");
  IScript* mScript = getScript(mScriptName);
  if (mScript == nullptr) {
    std::cout << "WARNING: Script \"" << mScriptName << "\" is NULL!" << std::endl;
  }
  return mScript->createScriptCall(node, this, localArgs);
}

bool* Resources::getDigitalInput(const std::string& name) {
  return cInputCommands.registerDigitalInput(name);
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
  I3DModelType* mModelType = c3DModelTypes.get(path);
  I3DModel* mModel = mModelType->createModel(location, scale);
  if (mModel != nullptr) {
    std::vector<I3DModel*> mModels = cInstantiatedModels[path];
    mModels.push_back(mModel);
    cInstantiatedModels[path] = mModels;
  }
  return mModel;
}

// IArgumentDefinition* Resources::getRegisteredArgumentDefinition(DOMNodeWrapper* node) {
//   std::string mType = node->getAttribute("type");
//   std::cout << "Creating argument definition from custom type: \"" << mType << "\"..." << std::endl;
//   IArgumentDefinitionType* mArgumentSource = cArgumentDefinitionTypes.get(mType);
//   return mArgumentSource->createArgumentDefinition(node, this);
//   return nullptr;
// }

IArgumentValue* Resources::getArgumentValue(DOMNodeWrapper* node) {
  Configuration* mConfiguration = Configuration::getInstance();
  return mConfiguration->getArgumentValue(node, this);
}

IArgumentValue* Resources::getArgumentValueCustom(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mValue = node->getAttribute("value");
  if (mValue[0] == '~') {
    mValue = mValue.substr(1);
  }
  std::cout << "Getting custom argument: \"" << mType << "/" << mValue + "\"..." << std::endl;
  return cArgumentValues.get(mType + "/" + mValue);
}

I3DModel* Resources::getModelInstance(const std::string& name)                   {return c3DModels.get(name);}
I3DModelType* Resources::getModelType(const std::string& path)                   {return c3DModelTypes.get(path);}
IBoolean* Resources::getBoolean(const std::string& path)                         {return cBooleans.get(path);}
IBoundaryHandler* Resources::getBoundaryHandler(DOMNodeWrapper* node)            {return new BoundaryHandler(node, this, cBoundaries.get(node->getAttribute("name")));}
ICamera* Resources::getCamera(const std::string& path)                           {return cCameras.get(path);}
IColour* Resources::getColour(const std::string& name)                           {return cColours.get(name);}
IComponentCustomType* Resources::getComponentCustomType(const std::string& path) {return cComponentCustomTypes.get(path);}
IFloat* Resources::getFloat(const std::string& path)                             {return cFloats.get(path);}
IFont* Resources::getFont(const std::string& path)                               {return cFonts.get(path);}
IInteger* Resources::getInteger(const std::string& path)                         {return cIntegers.get(path);}
ILayerType* Resources::getLayerType(const std::string& path)                     {return cLayerTypes.get(path);}
IScript* Resources::getScript(const std::string& path)                           {return cScripts.get(path);}
ISound* Resources::getSound(const std::string& path)                             {return cSounds.get(path);}
IString* Resources::getString(const std::string& path)                           {return cStrings.get(path);}
ITexture* Resources::getTexture(const std::string& name)                         {return cTextures.get(name);}
IVertex* Resources::getVertex(const std::string& path)                           {return cVertices.get(path);}

void Resources::notifyChange(IColour* colour)   {cColours.notifyChange(colour);}
void Resources::notifyChange(ITexture* texture) {cTextures.notifyChange(texture);}

std::vector<I3DModelType*> Resources::getAllModelTypes()                   {return c3DModelTypes.getAllVector();}
std::vector<IBoolean*> Resources::getAllBooleans()                         {return cBooleans.getAllVector();}
std::vector<ICamera*> Resources::getAllCameras()                           {return cCameras.getAllVector();}
std::vector<IBoundaries*> Resources::getAllBoundaries()                    {return cBoundaries.getAllVector();}
std::vector<IColour*> Resources::getAllColours()                           {return cColours.getAllVector();}
std::vector<IComponentCustomType*> Resources::getAllComponentCustomTypes() {return cComponentCustomTypes.getAllVector();}
std::vector<IElementType*> Resources::getAllElementTypes()                 {return cElementTypes.getAllVector();}
std::vector<IFloat*> Resources::getAllFloats()                             {return cFloats.getAllVector();}
std::vector<IFont*> Resources::getAllFonts()                               {return cFonts.getAllVector();}
std::vector<IInteger*> Resources::getAllIntegers()                         {return cIntegers.getAllVector();}
std::vector<ILayerType*> Resources::getAllLayerTypes()                     {return cLayerTypes.getAllVector();}
std::vector<IScript*> Resources::getAllScripts()                           {return cScripts.getAllVector();}
std::vector<ISound*> Resources::getAllSounds()                             {return cSounds.getAllVector();}
std::vector<IString*> Resources::getAllStrings()                           {return cStrings.getAllVector();}
std::vector<ITexture*> Resources::getAllTextures()                         {return cTextures.getAllVector();}
std::vector<IVertex*> Resources::getAllVertices()                          {return cVertices.getAllVector();}

void Resources::addResourceListener(IResourceListener<I3DModelType>* listener)               {c3DModelTypes.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IBoolean>* listener)                   {cBooleans.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IBoundaries>* listener)                {cBoundaries.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ICamera>* listener)                    {cCameras.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IColour>* listener)                    {cColours.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IComponentCustomType>* listener)       {cComponentCustomTypes.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IElementType>* listener)               {cElementTypes.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IFloat>* listener)                     {cFloats.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IFont>* listener)                      {cFonts.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IInteger>* listener)                   {cIntegers.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ILayerType>* listener)                 {cLayerTypes.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IScript>* listener)                    {cScripts.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ISound>* listener)                     {cSounds.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IString>* listener)                    {cStrings.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<ITexture>* listener)                   {cTextures.addResourceListener(listener);}
void Resources::addResourceListener(IResourceListener<IVertex>* listener)                    {cVertices.addResourceListener(listener);}

void Resources::addResourceType(IResourceType<I3DModelType>* resourceType,               const std::string& typeDescription) {c3DModelTypes.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IBoolean>* resourceType,                   const std::string& typeDescription) {cBooleans.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IBoundaries>* resourceType,                const std::string& typeDescription) {cBoundaries.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ICamera>* resourceType,                    const std::string& typeDescription) {cCameras.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IColour>* resourceType,                    const std::string& typeDescription) {cColours.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IComponentCustomType>* resourceType,       const std::string& typeDescription) {cComponentCustomTypes.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IElementType>* resourceType,               const std::string& typeDescription) {cElementTypes.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IFloat>* resourceType,                     const std::string& typeDescription) {cFloats.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IFont>* resourceType,                      const std::string& typeDescription) {cFonts.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IInteger>* resourceType,                   const std::string& typeDescription) {cIntegers.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ILayerType>* resourceType,                 const std::string& typeDescription) {cLayerTypes.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<IScript>* resourceType,                    const std::string& typeDescription) {cScripts.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<ISound>* resourceType,                     const std::string& typeDescription) {cSounds.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IString>* resourceType,                    const std::string& typeDescription) {cStrings.addResourceType(resourceType, typeDescription);}  
void Resources::addResourceType(IResourceType<ITexture>* resourceType,                   const std::string& typeDescription) {cTextures.addResourceType(resourceType, typeDescription);}
void Resources::addResourceType(IResourceType<IVertex>* resourceType,                    const std::string& typeDescription) {cVertices.addResourceType(resourceType, typeDescription);}  

void Resources::editResource(I3DModelType* resource,               IResourceAccessor* editingResources, IEditingContext* editingContext) {c3DModelTypes.editResource(        resource, this, editingContext);}
void Resources::editResource(IBoolean* resource,                   IResourceAccessor* editingResources, IEditingContext* editingContext) {cBooleans.editResource(            resource, this, editingContext);}
void Resources::editResource(IBoundaries* resource,                IResourceAccessor* editingResources, IEditingContext* editingContext) {cBoundaries.editResource(          resource, this, editingContext);}
void Resources::editResource(ICamera* resource,                    IResourceAccessor* editingResources, IEditingContext* editingContext) {cCameras.editResource(             resource, this, editingContext);}
void Resources::editResource(IColour* resource,                    IResourceAccessor* editingResources, IEditingContext* editingContext) {cColours.editResource(             resource, this, editingContext);}
void Resources::editResource(IComponentCustomType* resource,       IResourceAccessor* editingResources, IEditingContext* editingContext) {cComponentCustomTypes.editResource(resource, this, editingContext);}
void Resources::editResource(IElementType* resource,               IResourceAccessor* editingResources, IEditingContext* editingContext) {cElementTypes.editResource(        resource, this, editingContext);}
void Resources::editResource(IFloat* resource,                     IResourceAccessor* editingResources, IEditingContext* editingContext) {cFloats.editResource(              resource, this, editingContext);}
void Resources::editResource(IFont* resource,                      IResourceAccessor* editingResources, IEditingContext* editingContext) {cFonts.editResource(               resource, this, editingContext);}
void Resources::editResource(IInteger* resource,                   IResourceAccessor* editingResources, IEditingContext* editingContext) {cIntegers.editResource(            resource, this, editingContext);}
void Resources::editResource(ILayerType* resource,                 IResourceAccessor* editingResources, IEditingContext* editingContext) {cLayerTypes.editResource(          resource, this, editingContext);}
void Resources::editResource(IScript* resource,                    IResourceAccessor* editingResources, IEditingContext* editingContext) {cScripts.editResource(             resource, this, editingContext);}
void Resources::editResource(ISound* resource,                     IResourceAccessor* editingResources, IEditingContext* editingContext) {cSounds.editResource(              resource, this, editingContext);}
void Resources::editResource(IString* resource,                    IResourceAccessor* editingResources, IEditingContext* editingContext) {cStrings.editResource(             resource, this, editingContext);}
void Resources::editResource(ITexture* resource,                   IResourceAccessor* editingResources, IEditingContext* editingContext) {cTextures.editResource(            resource, this, editingContext);}
void Resources::editResource(IVertex* resource,                    IResourceAccessor* editingResources, IEditingContext* editingContext) {cVertices.editResource(            resource, this, editingContext);}

void Resources::removeResource(I3DModelType* resource,               IResourceAccessor* editingResources) {c3DModelTypes.removeResource(resource, this);}
void Resources::removeResource(IBoolean* resource,                   IResourceAccessor* editingResources) {cBooleans.removeResource(resource, this);}
void Resources::removeResource(IBoundaries* resource,                IResourceAccessor* editingResources) {cBoundaries.removeResource(resource, this);}
void Resources::removeResource(ICamera* resource,                    IResourceAccessor* editingResources) {cCameras.removeResource(resource, this);}
void Resources::removeResource(IColour* resource,                    IResourceAccessor* editingResources) {cColours.removeResource(resource, this);}
void Resources::removeResource(IComponentCustomType* resource,       IResourceAccessor* editingResources) {cComponentCustomTypes.removeResource(resource, this);}
void Resources::removeResource(IElementType* resource,               IResourceAccessor* editingResources) {cElementTypes.removeResource(resource, this);}
void Resources::removeResource(IFloat* resource,                     IResourceAccessor* editingResources) {cFloats.removeResource(resource, this);}
void Resources::removeResource(IFont* resource,                      IResourceAccessor* editingResources) {cFonts.removeResource(resource, this);}
void Resources::removeResource(IInteger* resource,                   IResourceAccessor* editingResources) {cIntegers.removeResource(resource, this);}
void Resources::removeResource(ILayerType* resource,                 IResourceAccessor* editingResources) {cLayerTypes.removeResource(resource, this);}
void Resources::removeResource(IScript* resource,                    IResourceAccessor* editingResources) {cScripts.removeResource(resource, this);}
void Resources::removeResource(ISound* resource,                     IResourceAccessor* editingResources) {cSounds.removeResource(resource, this);}
void Resources::removeResource(IString* resource,                    IResourceAccessor* editingResources) {cStrings.removeResource(resource, this);}
void Resources::removeResource(ITexture* resource,                   IResourceAccessor* editingResources) {cTextures.removeResource(resource, this);}
void Resources::removeResource(IVertex* resource,                    IResourceAccessor* editingResources) {cVertices.removeResource(resource, this);}

void Resources::add(IResource* resource, DOMNodeWrapper* node) {
  addResourceToInitialise(resource, node);
}

void Resources::add(I3DModel* model,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {c3DModels.add(model, path, name);                      addResourceToInitialise(model, node);}
void Resources::add(I3DModelType* modelType,                         std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {c3DModelTypes.add(modelType, path, name);              addResourceToInitialise(modelType, node);}
//void Resources::add(IArgumentDefinitionType* argumentDefinitionType, std::vector<std::string> path, const std::string& name                      ) {cArgumentDefinitionTypes.add(argumentDefinitionType, path, name);}
void Resources::add(IArgumentValue* value,                           std::vector<std::string> path, const std::string& type, const std::string& name) {path.push_back(type); cArgumentValues.add(value, path, name);}
void Resources::add(IBoolean* value,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cBooleans.add(value, path, name);                      addResourceToInitialise(value, node);}
void Resources::add(ICamera* camera,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cCameras.add(camera, path, name);                      addResourceToInitialise(camera, node);}
void Resources::add(IBoundaries* boundaries,                         std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cBoundaries.add(boundaries, path, name);               addResourceToInitialise(boundaries, node);}
void Resources::add(IColour* colour,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cColours.add(colour, path, name);                      addResourceToInitialise(colour, node);}
void Resources::add(IComponentCustomType* type,                      std::vector<std::string> path, const std::string& name                      ) {cComponentCustomTypes.add(type, path, name);}
void Resources::add(IElementType* type,                              std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cElementTypes.add(type, path, name);                   addResourceToInitialise(type, node);}
void Resources::add(IFont* font,                                     std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cFonts.add(font, path, name);                          addResourceToInitialise(font, node);}
void Resources::add(IGlobalVariable* variable,                       std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cGlobalVariables.add(variable, path, name);            addResourceToInitialise(variable, node);}
void Resources::add(IInteger* value,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cIntegers.add(value, path, name);                      addResourceToInitialise(value, node);}
void Resources::add(ILayerType* layerType,                           std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cLayerTypes.add(layerType, path, name);                addResourceToInitialise(layerType, node);}
void Resources::add(IScript* script,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cScripts.add(script, path, name);                      addResourceToInitialise(script, node);}
void Resources::add(ISound* sound,                                   std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cSounds.add(sound, path, name);                        addResourceToInitialise(sound, node);}
void Resources::add(IString* string,                                 std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cStrings.add(string, path, name);                      addResourceToInitialise(string, node);}
void Resources::add(ITexture* texture,                               std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cTextures.add(texture, path, name);                    addResourceToInitialise(texture, node);}
void Resources::add(IVertex* location,                               std::vector<std::string> path, const std::string& name, DOMNodeWrapper* node) {cVertices.add(location, path, name);                   addResourceToInitialise(location, node);}

void Resources::addResourceToInitialise(IResource* resource, DOMNodeWrapper* node) {
  if (node != nullptr) {
    ResourceInitialiser* mResourceInitialiser = new ResourceInitialiser(resource, node);
    cResourcesToInitialise.push_back(mResourceInitialiser);
  }
}
  
void Resources::addDynamicElement(IDynamicElement* element) {
  cDynamicElements.push_back(element);
}

void Resources::loadElement(DOMNodeWrapper* node, BlockLocation* location, IElementContainer* container, bool asTemplate) {
  std::string mElementTypePath = node->getAttribute("type");
  IElementType* mElementType = cElementTypes.get(mElementTypePath);
  mElementType->loadElement(node, location, container, this, asTemplate);
}

void Resources::addListener(IResourceUseListener<I3DModelType>* listener)               {c3DModelTypes.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IBoolean>* listener)                   {cBooleans.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IBoundaries>* listener)                {cBoundaries.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ICamera>* listener)                    {cCameras.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IColour>* listener)                    {cColours.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IComponentCustomType>* listener)       {cComponentCustomTypes.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IElementType>* listener)               {cElementTypes.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IFloat>* listener)                     {cFloats.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IFont>* listener)                      {cFonts.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IInteger>* listener)                   {cIntegers.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ILayerType>* listener)                 {cLayerTypes.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IScript>* listener)                    {cScripts.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ISound>* listener)                     {cSounds.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IString>* listener)                    {cStrings.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<ITexture>* listener)                   {cTextures.addResourceUseListener(listener);}
void Resources::addListener(IResourceUseListener<IVertex>* listener)                    {cVertices.addResourceUseListener(listener);}

std::string Resources::getPath(I3DModelType* modelType)                     {return c3DModelTypes.getLocation(modelType);}
std::string Resources::getPath(IArgumentValue* argument)                    {return cArgumentValues.getLocation(argument);}
std::string Resources::getPath(IBoolean* boolean)                           {return cBooleans.getLocation(boolean);}
std::string Resources::getPath(IBoundaries* boundaries)                     {return cBoundaries.getLocation(boundaries);}
std::string Resources::getPath(ICamera* camera)                             {return cCameras.getLocation(camera);}
std::string Resources::getPath(IColour* colour)                             {return cColours.getLocation(colour);}
std::string Resources::getPath(IComponentCustomType* customType)            {return cComponentCustomTypes.getLocation(customType);}
std::string Resources::getPath(IElementType* elementType)                   {return cElementTypes.getLocation(elementType);}
std::string Resources::getPath(IFloat* afloat)                              {return cFloats.getLocation(afloat);}
std::string Resources::getPath(IFont* font)                                 {return cFonts.getLocation(font);}
std::string Resources::getPath(IGlobalVariable* globalVariable)             {return cGlobalVariables.getLocation(globalVariable);}
std::string Resources::getPath(IInteger* integer)                           {return cIntegers.getLocation(integer);}
std::string Resources::getPath(ILayerType* layerType)                       {return cLayerTypes.getLocation(layerType);}
std::string Resources::getPath(IScript* script)                             {return cScripts.getLocation(script);}
std::string Resources::getPath(ISound* sound)                               {return cSounds.getLocation(sound);}
std::string Resources::getPath(IString* string)                             {return cStrings.getLocation(string);}
std::string Resources::getPath(ITexture* texture)                           {return cTextures.getLocation(texture);}
std::string Resources::getPath(IVertex* vertex)                             {return cVertices.getLocation(vertex);}

std::string Resources::getPath(I3DModel* model) {
  std::string mModelInstance = c3DModels.getLocation(model);
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

void Resources::saveScript(DOMNodeWriter* node, const std::string& nodeName, IScriptCall* script) {
  if (script != nullptr) {
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

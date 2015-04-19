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
#include "SpindizzyModule.h"

const std::string SpindizzyModule::TAG_RESOURCE_TYPE_CAMERA_SCRIPTABLE               = "CameraScriptable";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK         = "ElementSpindizzyBlock";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT         = "ElementSpindizzyCraft";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY         = "ElementSpindizzyEnemy";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM          = "ElementSpindizzyItem";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT          = "ElementSpindizzyLift";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER         = "ElementSpindizzyWater";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE          = "ElementSpindizzyZone";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_LAYER_BACKGROUND                = "LayerBackground";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP             = "LayerSpindizzyMap";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_BALL      = "ModelSpindizzyCraftBall";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE = "ModelSpindizzyCraftGyroscope";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID   = "ModelSpindizzyCraftPyramid";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL           = "ModelSpindizzyJewel";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64    = "TexturesSpindizzyBlocksC64";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64     = "TexturesSpindizzyLiftsC64";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME    = "TexturesSpindizzyZoneTheme";
  
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_CAMERA_SCRIPTABLE               = "Scriptable Camera";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK         = "Spindizzy Block";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT         = "Spindizzy Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY         = "Spindizzy Enemy";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM          = "Spindizzy Item";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT          = "Spindizzy Lift";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER         = "Spindizzy Water";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE          = "Spindizzy Zone";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_LAYER_BACKGROUND                = "Scriptable Coloured Background";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP             = "Spindizzy Map";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_BALL      = "Spindizzy Ball Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE = "Spindizzy Gyroscope Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID   = "Spindizzy Pyramid Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL           = "Spindizzy Jewel";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64    = "Spindizzy C64 Block Textures";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64     = "Spindizzy C64 Lift Textures";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME    = "Spindizzy Zone Theme";

SpindizzyModule::SpindizzyModule(IResourceTypeRegistry* resourceManager):cResourceTypeElementSpindizzyBlock(this),
                                                                         cResourceTypeElementSpindizzyCraft(this),
                                                                         cResourceTypeElementSpindizzyEnemy(this),
                                                                         cResourceTypeElementSpindizzyItem(this),
                                                                         cResourceTypeElementSpindizzyLift(this),
                                                                         cResourceTypeElementSpindizzyWater(this),
                                                                         cResourceTypeElementSpindizzyZone(this),
                                                                         cResourceTypeTextureSpindizzyBlocksC64(this) {
  if (resourceManager != nullptr) {
    resourceManager->addResourceType(&cResourceTypeCameraScriptable,             NAME_RESOURCE_TYPE_CAMERA_SCRIPTABLE);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyBlock,        NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyCraft,        NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyEnemy,        NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyItem,         NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyLift,         NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyWater,        NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER);
    resourceManager->addResourceType(&cResourceTypeElementSpindizzyZone,         NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE);
    resourceManager->addResourceType(&cResourceTypeLayerBackground,              NAME_RESOURCE_TYPE_LAYER_BACKGROUND);
    resourceManager->addResourceType(&cResourceTypeLayerSpindizzyMap,            NAME_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyCraftBall,      NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_BALL);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyCraftGyroscope, NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyCraftPyramid,   NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyJewel,          NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL);
    resourceManager->addResourceType(&cResourceTypeTextureSpindizzyBlocksC64,    NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64);
    resourceManager->addResourceType(&cResourceTypeTextureSpindizzyLiftsC64,     NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64);
    cEditing = true;
  } else {
    cEditing = false;
  }
  cOverview = false;
  cZoneCount = 0;
}

void SpindizzyModule::setOverview(bool overview) {
  cOverview = overview;
}

unsigned int SpindizzyModule::getZoneCount() {
  return cZoneCount;
}

void SpindizzyModule::load(DOMNodeWrapper* node, IResourceRegistry* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if      (mValueAsString == TAG_RESOURCE_TYPE_CAMERA_SCRIPTABLE)               {cResourceTypeCameraScriptable.loadResource(            mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK)         {cResourceTypeElementSpindizzyBlock.loadResource(       mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT)         {cResourceTypeElementSpindizzyCraft.loadResource(       mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY)         {cResourceTypeElementSpindizzyEnemy.loadResource(       mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM)          {cResourceTypeElementSpindizzyItem.loadResource(        mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT)          {cResourceTypeElementSpindizzyLift.loadResource(        mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER)         {cResourceTypeElementSpindizzyWater.loadResource(       mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE)          {cResourceTypeElementSpindizzyZone.loadResource(        mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_LAYER_BACKGROUND)                {cResourceTypeLayerBackground.loadResource(             mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP)             {cResourceTypeLayerSpindizzyMap.loadResource(           mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_BALL)      {cResourceTypeModelSpindizzyCraftBall.loadResource(     mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE) {cResourceTypeModelSpindizzyCraftGyroscope.loadResource(mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID)   {cResourceTypeModelSpindizzyCraftPyramid.loadResource(  mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL)           {cResourceTypeModelSpindizzyJewel.loadResource(         mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64)    {cResourceTypeTextureSpindizzyBlocksC64.loadResource(   mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64)     {cResourceTypeTextureSpindizzyLiftsC64.loadResource(    mNode, resources);}
//    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME)    {cResourceTypeTextureSpindizzyZoneTheme.loadResource(   mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME)    {
      std::string mThemeName = mNode->getAttribute("name");
      SpindizzyZoneTheme* mTheme = new SpindizzyZoneTheme(this);
      resources->add(mTheme, mNode);
      resources->addListener(static_cast<IResourceUseListener<ITexture>*>(mTheme));
      resources->addListener(static_cast<IResourceUseListener<IColour>*>(mTheme));
      cThemes[mThemeName] = mTheme;
      createThemeResources(mNode, resources);
    } else if (mValueAsString == "BlockState") {
      SpindizzyBlockState* mBlockState = new SpindizzyBlockState();
      std::string mName = mNode->getAttribute("name");
      cBlockStateData.push_back(mBlockState);
      IArgumentValue* mArgumentValue = new ArgumentValue<SpindizzyBlockState>(mBlockState);
      resources->add(mBlockState, mNode);
      resources->add(mArgumentValue, "BlockState", mName);
    }
    else                                                                          {/* TODO: Throw */}
  }
  IArgumentValue* mModuleArgumentValue = new ArgumentValue<SpindizzyModule>(this);
  resources->add(this, node);
  resources->add(&cElementHandlerSpindizzyBlock, "SpindizzyBlockHandler", "SpindizzyBlockHandler");
  resources->add(&cDynamicElementHandlerArgument, "DynamicElements", "DynamicElements");
  resources->add(&cZoneElementHandlerArgument, "ZoneHandler", "ZoneHandler");
  resources->add(&cItemElementHandlerArgument, "ItemHandler", "ItemHandler");
  resources->add(&cArgumentValueZone, "Zone", "Zone");
  resources->add(mModuleArgumentValue, "Spindizzy", "Spindizzy");  
}

void SpindizzyModule::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cVisualProcessor = resources->getGeometryProcessor(node->getAttribute("visualProcessor"));
  cPhysicalProcessor = resources->getGeometryProcessor(node->getAttribute("physicalProcessor"));
  cSurfaceRegistry = resources->getSurfaceRegistry(node->getAttribute("surfaceRegistry"));
  cLocked = resources->getInteger(node->getAttribute("locks"));
  cCamera = resources->getCamera(node->getAttribute("camera"));
  cCamera->addCameraAngleChangeListener(this);
  cameraAngleChanged(cCamera->getAngle());
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftMovedScript") {
      cLiftMovedScript = resources->getScriptCall(mNode);
    }
  }
}

void SpindizzyModule::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cResourceTypeCameraScriptable.saveResources(            node, resourceLocator, TAG_RESOURCE_TYPE_CAMERA_SCRIPTABLE);
  cResourceTypeElementSpindizzyBlock.saveResources(       node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK);
  cResourceTypeElementSpindizzyCraft.saveResources(       node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT);
  cResourceTypeElementSpindizzyEnemy.saveResources(       node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY);
  cResourceTypeElementSpindizzyItem.saveResources(        node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM);
  cResourceTypeElementSpindizzyLift.saveResources(        node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT);
  cResourceTypeElementSpindizzyWater.saveResources(       node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER);
  cResourceTypeElementSpindizzyZone.saveResources(        node, resourceLocator, TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE);
  cResourceTypeLayerBackground.saveResources(             node, resourceLocator, TAG_RESOURCE_TYPE_LAYER_BACKGROUND);
  cResourceTypeLayerSpindizzyMap.saveResources(           node, resourceLocator, TAG_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP);
  cResourceTypeModelSpindizzyCraftBall.saveResources(     node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_BALL);
  cResourceTypeModelSpindizzyCraftGyroscope.saveResources(node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE);
  cResourceTypeModelSpindizzyCraftPyramid.saveResources(  node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID);
  cResourceTypeModelSpindizzyJewel.saveResources(         node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL);
  cResourceTypeTextureSpindizzyBlocksC64.saveResources(   node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64);
  cResourceTypeTextureSpindizzyLiftsC64.saveResources(    node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64);
//  cResourceTypeTextureSpindizzyZoneTheme.saveResources(   node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME);
}

void SpindizzyModule::createThemeResources(DOMNodeWrapper* node, IResourceRegistry* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mTextureName = mNode->getAttribute("type");
      createThemeTexture(mTextureName, runtimeContext);
    } else if (mValueAsString == "Colour") {
      std::string mColourName = mNode->getAttribute("type");
      createThemeColour(mColourName, runtimeContext);
    }
  }
}

void SpindizzyModule::createThemeTexture(const std::string& type, IResourceRegistry* runtimeContext) {
  if (cTextures.find(type) == cTextures.end()) {
    cTextures[type] = new SpindizzyZoneThemeTexture();
    runtimeContext->add(cTextures[type], type);
  }
}

void SpindizzyModule::createThemeColour(const std::string& type, IResourceRegistry* runtimeContext) {
  if (cColours.find(type) == cColours.end()) {
    cColours[type] = new SpindizzyZoneThemeColour();
    IArgumentValue* mArgumentValue = new ArgumentValue<SpindizzyZoneThemeColour>(cColours[type]);
    runtimeContext->add(cColours[type], type);
    runtimeContext->add(mArgumentValue, "ThemeColour", type);
  }
}

  /*********************************\
   * Implements ISpindizzyBlockSet *
  \*********************************/
void SpindizzyModule::registerSurfaceProvider(IGeometricElement* element) {
  cVisualProcessor->registerGeometricElement(element);
  if (!cEditing) {
    cPhysicalProcessor->registerGeometricElement(element);
  }
}

void SpindizzyModule::unregisterSurfaceProvider(IGeometricElement* element) {
  cVisualProcessor->unregisterGeometricElement(element);
  if (!cEditing) {
    cPhysicalProcessor->unregisterGeometricElement(element);
  }
}

void SpindizzyModule::setDirty() {
  cVisualProcessor->setDirty();
  if (!cEditing) { // TODO: Is this necessary?  Does this function even get called during runtime?
    cPhysicalProcessor->setDirty();
  }
}

std::vector<ITileSurfaceTemplate*> SpindizzyModule::getTileSurfaces(IGeometricElement* element, ITileSurface::FaceDirection facing, bool visual) {
  return (visual ? cVisualProcessor : cPhysicalProcessor)->getTileSurfaces(element, facing);
}

std::vector<IWallSurfaceTemplate*> SpindizzyModule::getWallSurfaces(IGeometricElement* element, IWallSurface::FaceDirection facing, bool visual) {
  return (visual ? cVisualProcessor : cPhysicalProcessor)->getWallSurfaces(element, facing);
}

void SpindizzyModule::destroyTileTemplate(ITileSurfaceTemplate* tileTemplate, bool visual) {
  (visual ? cVisualProcessor : cPhysicalProcessor)->destroyTileTemplate(tileTemplate);
}

void SpindizzyModule::destroyWallTemplate(IWallSurfaceTemplate* wallTemplate, bool visual) {
  (visual ? cVisualProcessor : cPhysicalProcessor)->destroyWallTemplate(wallTemplate);
}

void SpindizzyModule::registerRollableSurface(IRollableSurface* rollableSurface) {
  cSurfaceRegistry->registerRollableSurface(rollableSurface, false);
}

void SpindizzyModule::registerWallSurface(ICollidableWallSurface* wallSurface) {
  cSurfaceRegistry->registerWallSurface(wallSurface);
}

std::vector<ConditionElement*> SpindizzyModule::getConditionElements() {
  std::vector<ConditionElement*> mConditionElements;
  for (unsigned int i = 0; i < cBlockStateData.size(); i++) {
    mConditionElements.push_back(cBlockStateData[i]->getConditionElement());
  }
  return mConditionElements;
}

void SpindizzyModule::staticChanged() {
  // TODO: cStaticChangeListener->staticChanged();
}

SpindizzyBlockState* SpindizzyModule::getBlockState(IBoolean* input) {
  for (unsigned int i = 0; i < cBlockStateData.size(); i++) {
    IBoolean* mInput = cBlockStateData[i]->getInputAddress();
    if (mInput == input) {
      return cBlockStateData[i];
    }
  }
  return nullptr;
}

void SpindizzyModule::setArgumentValue(ElementHandlerSpindizzyBlock* handler) {
  cElementHandlerSpindizzyBlock.setValue(handler);
}

void SpindizzyModule::registerElement(IElementContainer* container, ElementSpindizzyEnemy* enemy) {
  std::vector<ElementSpindizzyEnemy*> mEnemies = cEnemiesByContainer[container];
  mEnemies.push_back(enemy);
  cEnemiesByContainer[container] = mEnemies;
}
 
void SpindizzyModule::itemCollected() {
//  cCollectedCount->setValue(cCollectedCount->getValue() - 1);
}

ElementHandlerSpindizzyBlock* SpindizzyModule::getElementHandlerSpindizzyBlock(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerSpindizzyBlock*>::iterator i = cElementHandlersSpindizzyBlock.find(container);
  if (i == cElementHandlersSpindizzyBlock.end()) {
    ElementHandlerSpindizzyBlock* mHandler = new ElementHandlerSpindizzyBlock(this);
    cElementHandlersSpindizzyBlock[container] = mHandler;
    container->addElement(mHandler);
    container->addArgumentValue(mHandler);
  }
  return cElementHandlersSpindizzyBlock[container];
}

ElementHandlerItem* SpindizzyModule::getItemElementHandler(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerItem*>::iterator i = cItemElementHandlers.find(container);
  if (i == cItemElementHandlers.end()) {
    ElementHandlerItem* mItemElementHandler = new ElementHandlerItem(this);
    cItemElementHandlers[container] = mItemElementHandler;
    container->addElement(mItemElementHandler);
    container->addArgumentValue(mItemElementHandler);
  }
  return cItemElementHandlers[container];
}
  
void SpindizzyModule::setArgumentValue(ElementHandlerItem* itemElementHandler) {
  cItemElementHandlerArgument.setValue(itemElementHandler);
}
  
SpindizzyLiftProperties* SpindizzyModule::getSpindizzyLiftProperties() {
  return &cSpindizzyLiftProperties;
}

void SpindizzyModule::executeLiftMovedScript() {
  cLiftMovedScript->execute();
}

void SpindizzyModule::registerInterceptingSurface(IRollableSurface* surface) {
  cSurfaceRegistry->registerRollableSurface(surface, true);
}

void SpindizzyModule::registerElement(IElementContainer* container, ElementSpindizzyLift* lift) {
  std::vector<ElementSpindizzyLift*> mLifts = cLiftsByContainer[container];
  mLifts.push_back(lift);
  cLiftsByContainer[container] = mLifts;
}
  
SpindizzyZoneTheme* SpindizzyModule::getTheme(const std::string& type) {
  return cThemes[type];
}
  
bool SpindizzyModule::isOverview() {
  return cOverview;
}
  
IArgumentValue* SpindizzyModule::getArgumentValue(const std::string& argument) {
  if (argument == "zone") {return &cArgumentValueZone;}
  return nullptr;
}

void SpindizzyModule::setArgumentValue(ElementSpindizzyZone* zone) {
  cArgumentValueZone.setValue(zone);
}

SpindizzyZoneThemeTexture* SpindizzyModule::getThemeTexture(const std::string& type) {
  return cTextures[type];
}

SpindizzyZoneThemeColour* SpindizzyModule::getThemeColour(const std::string& type) {
  return cColours[type];
}

std::string SpindizzyModule::getThemeElement(SpindizzyZoneThemeTexture* themeTexture) {
  for (std::map<std::string, SpindizzyZoneThemeTexture*>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
    if (i->second == themeTexture) {
      return i->first;
    }
  }
  // TODO: Throw
  std::cout << "WARNING: Theme element not found" << std::endl;
  exit(1);
}

std::string SpindizzyModule::getThemeElement(SpindizzyZoneThemeColour* themeColour) {
  for (std::map<std::string, SpindizzyZoneThemeColour*>::iterator i = cColours.begin(); i != cColours.end(); i++) {
    if (i->second == themeColour) {
      return i->first;
    }
  }
  // TODO: Throw
  std::cout << "WARNING: Theme element not found" << std::endl;
  exit(1);
}

void SpindizzyModule::cameraAngleChanged(float angle) {
  for (unsigned int i = 0; i < cCameraAngleChangeListeners.size(); i++) {
    cCameraAngleChangeListeners[i]->cameraAngleChanged(angle);
  }
}

void SpindizzyModule::addCameraAngleChangeListener(ICameraAngleChangeListener* listener) {
  cCameraAngleChangeListeners.push_back(listener);
}

void SpindizzyModule::setArgumentValue(ElementHandlerSpindizzyDynamic* dynamicElementHandler) {
  cDynamicElementHandlerArgument.setValue(dynamicElementHandler);
}

void SpindizzyModule::setArgumentValue(ElementHandlerZone* zoneElementHandler) {
  cZoneElementHandlerArgument.setValue(zoneElementHandler);
}

bool SpindizzyModule::isEditing() {
  return cEditing;
}

bool SpindizzyModule::isLocked() {
  return cLocked->getValue() > 0;
}

ElementHandlerSpindizzyDynamic* SpindizzyModule::getDynamicElementHandler(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerSpindizzyDynamic*>::iterator i = cDynamicElementHandlers.find(container);
  if (i == cDynamicElementHandlers.end()) {
    ElementHandlerSpindizzyDynamic* mDynamicElementHandler = new ElementHandlerSpindizzyDynamic(this);
    cDynamicElementHandlers[container] = mDynamicElementHandler;
    container->addElement(mDynamicElementHandler);
    container->addArgumentValue(mDynamicElementHandler);
  }
  return cDynamicElementHandlers[container];
}

ElementHandlerZone* SpindizzyModule::getZoneElementHandler(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerZone*>::iterator i = cZoneElementHandlers.find(container);
  if (i == cZoneElementHandlers.end()) {
    ElementHandlerZone* mZoneElementHandler = new ElementHandlerZone(this);
    cZoneElementHandlers[container] = mZoneElementHandler;
    container->addElement(mZoneElementHandler);
    container->addArgumentValue(mZoneElementHandler);
    setArgumentValue(mZoneElementHandler); // TODO: This is a dirty great hack that only works for the last container (hint: Spindizzy only has one map, so it works)
  }
  cZoneCount++;
  return cZoneElementHandlers[container];
}

extern "C" IModule* create(IResourceTypeRegistry* resourceManager) {
  return new SpindizzyModule(resourceManager);
}

extern "C" void destroy(IModule* module) {
  delete module;
}

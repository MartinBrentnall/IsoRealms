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

const std::string SpindizzyModule::TAG_CUSTOM_TYPE_BLOCK_STATE                       = "BlockState";

const std::string SpindizzyModule::TAG_EDITOR_CONFIG_SPINDIZZY_ZONE_THEME_ICON       = "TexturesSpindizzyZoneThemeIcon";

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
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE = "ModelSpindizzyCraftGyroscope";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID   = "ModelSpindizzyCraftPyramid";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL           = "ModelSpindizzyJewel";
const std::string SpindizzyModule::TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL    = "TextureSpindizzyCraftBall";
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
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE = "Spindizzy Gyroscope Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID   = "Spindizzy Pyramid Craft";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL           = "Spindizzy Jewel";
const std::string SpindizzyModule::NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL    = "Spindizzy Ball Craft Texture";
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
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyCraftGyroscope, NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyCraftPyramid,   NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID);
    resourceManager->addResourceType(&cResourceTypeModelSpindizzyJewel,          NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL);
    resourceManager->addResourceType(&cResourceTypeTextureSpindizzyCraftBall,    NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL);
    resourceManager->addResourceType(&cResourceTypeTextureSpindizzyBlocksC64,    NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64);
    resourceManager->addResourceType(&cResourceTypeTextureSpindizzyLiftsC64,     NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64);
    
    resourceManager->registerCustomResourceManager(new DialogGeneratorSpindizzyZoneThemeSelector(this));
    cEditing = true;
  } else {
    cEditing = false;
  }
  cOverview = false;
  cZoneCount = 0;
  cDefaultTheme = nullptr;
  cSelectedZoneTheme = nullptr;
  cThemeModelIcon = nullptr;
  cActiveSurfaceRegistry = nullptr;
}

void SpindizzyModule::setOverview(bool overview) {
  cOverview = overview;
}

unsigned int SpindizzyModule::getZoneCount() {
  return cZoneCount;
}

void SpindizzyModule::setTheme(ISpindizzyZoneTheme* theme) {
  cDefaultTheme = theme;
}

void SpindizzyModule::setActiveUniverse(IUniverse* universe) {
  cActiveSurfaceRegistry = getSurfaceRegistry(universe);
}

void SpindizzyModule::load(DOMNodeWrapper* node, IResourceRegistry* resources, DOMNodeWrapper* options) {
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
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE) {cResourceTypeModelSpindizzyCraftGyroscope.loadResource(mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID)   {cResourceTypeModelSpindizzyCraftPyramid.loadResource(  mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL)           {cResourceTypeModelSpindizzyJewel.loadResource(         mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL)    {cResourceTypeTextureSpindizzyCraftBall.loadResource(   mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64)    {cResourceTypeTextureSpindizzyBlocksC64.loadResource(   mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64)     {cResourceTypeTextureSpindizzyLiftsC64.loadResource(    mNode, resources);}
//    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME)    {cResourceTypeTextureSpindizzyZoneTheme.loadResource(   mNode, resources);}
    else if (mValueAsString == TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME)    {
      std::string mThemeName = mNode->getAttribute("name");
      SpindizzyZoneTheme* mTheme = new SpindizzyZoneTheme(this);
      if (cSelectedZoneTheme == nullptr) {
        cSelectedZoneTheme = mTheme;
      }
      resources->add(mTheme, mNode);
      resources->addListener(static_cast<IResourceUseListener<ITexture>*>(mTheme));
      resources->addListener(static_cast<IResourceUseListener<IColour>*>(mTheme));
      cThemes[mThemeName] = mTheme;
      createThemeResources(mNode, resources);
    } else if (mValueAsString == TAG_CUSTOM_TYPE_BLOCK_STATE) {
      SpindizzyBlockState* mBlockState = new SpindizzyBlockState();
      std::string mName = mNode->getAttribute("name");
      cBlockStateData.push_back(mBlockState);
      IArgumentValue* mArgumentValue = new ArgumentValueCustomType<SpindizzyBlockState>(mBlockState);
      resources->add(mBlockState, mNode);
      resources->add(mArgumentValue, "BlockState", mName);
    }
    else                                                                          {/* TODO: Throw */}
  }
  IArgumentValue* mModuleArgumentValue = new ArgumentValueCustomType<SpindizzyModule>(this);
  resources->add(this, node);
  resources->add(&cElementHandlerSpindizzyBlock, "SpindizzyBlockHandler", "SpindizzyBlockHandler");
  resources->add(&cDynamicElementHandlerArgument, "DynamicElements", "DynamicElements");
  resources->add(&cZoneElementHandlerArgument, "ZoneHandler", "ZoneHandler");
  resources->add(&cItemElementHandlerArgument, "ItemHandler", "ItemHandler");
  resources->add(&cArgumentValueZone, "Zone", "Zone");
  resources->add(mModuleArgumentValue, "Spindizzy", "Spindizzy");
}

void SpindizzyModule::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cLocked = resources->getInteger(node->getAttribute("locks"));
  cCamera = resources->getCamera(node->getAttribute("camera"));
  cCamera->addCameraAngleChangeListener(this);
  cameraAngleChanged(cCamera->getAngle());
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "LiftMovedScript") {
      cLiftMovedScript = resources->getScriptCall(mNode);
    } else if (mValueAsString == TAG_EDITOR_CONFIG_SPINDIZZY_ZONE_THEME_ICON) {
      cThemeModelIconLocation.x = mNode->getFloatAttribute("x");
      cThemeModelIconLocation.y = mNode->getFloatAttribute("y");
      cThemeModelIconLocation.z = mNode->getFloatAttribute("z");
      cThemeModelIconScale = mNode->getFloatAttribute("scale");
      std::string mModelPath = mNode->getAttribute("model");
      cThemeModelIcon = resources->getModelType(mModelPath);
    }
  }
}

void SpindizzyModule::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  resourceLocator->saveScript(node, "LiftMovedScript", cLiftMovedScript);
  for (SpindizzyBlockState* mBlockState : cBlockStateData) {
    DOMNodeWriter* mBlockStateNode = node->addBranch("BlockState");
    mBlockState->save(mBlockStateNode, resourceLocator);
  }
  DOMNodeWriter* mThemeIconNode = node->addBranch(TAG_EDITOR_CONFIG_SPINDIZZY_ZONE_THEME_ICON);
  mThemeIconNode->addAttribute("model", resourceLocator->getPath(cThemeModelIcon));
  mThemeIconNode->addAttribute("x", cThemeModelIconLocation.x);
  mThemeIconNode->addAttribute("y", cThemeModelIconLocation.y);
  mThemeIconNode->addAttribute("z", cThemeModelIconLocation.z);
  mThemeIconNode->addAttribute("scale", cThemeModelIconScale);

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
  cResourceTypeModelSpindizzyCraftGyroscope.saveResources(node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE);
  cResourceTypeModelSpindizzyCraftPyramid.saveResources(  node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID);
  cResourceTypeModelSpindizzyJewel.saveResources(         node, resourceLocator, TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL);
  cResourceTypeTextureSpindizzyCraftBall.saveResources(   node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL);
  cResourceTypeTextureSpindizzyBlocksC64.saveResources(   node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64);
  cResourceTypeTextureSpindizzyLiftsC64.saveResources(    node, resourceLocator, TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64);
  
  for (std::pair<std::string, SpindizzyZoneTheme*> mTheme : cThemes) {
    DOMNodeWriter* mThemeNode = node->addBranch(TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME);
    mThemeNode->addAttribute("name", mTheme.first);
    mTheme.second->save(mThemeNode, resourceLocator);
  }

  std::string mLocksPath = resourceLocator->getPath(cLocked);
  std::string mCameraPath = resourceLocator->getPath(cCamera);
  node->addAttribute("locks", mLocksPath);
  node->addAttribute("camera", mCameraPath);
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
    IArgumentValue* mArgumentValue = new ArgumentValueCustomType<SpindizzyZoneThemeColour>(cColours[type]);
    runtimeContext->add(cColours[type], type);
    runtimeContext->add(mArgumentValue, "ThemeColour", type);
  }
}

I3DModel* SpindizzyModule::getConditionElementIcon(ConditionElement* element) {
  for (SpindizzyBlockState* mBlockState : cBlockStateData) {
    if (mBlockState->getInputAddress() == element->getInputAddress()) {
      return mBlockState->getModel();
    }
  }
  return nullptr; // TODO: Throw
}

std::map<IUniverse*, ResourceGeometryProcessor*>* SpindizzyModule::getGeometryProcessors(bool visual) {
  return visual ? &cVisualProcessors : &cPhysicalProcessors;
}

ResourceGeometryProcessor* SpindizzyModule::getGeometryProcessor(IUniverse* universe, bool visual) {
  if (universe == nullptr) {
    return nullptr; // TODO: Throw
  }
  std::map<IUniverse*, ResourceGeometryProcessor*>* mProcessors = getGeometryProcessors(visual);
  ResourceGeometryProcessor* mProcessor = (*mProcessors)[universe];
  if (mProcessor == nullptr) {
    mProcessor = new ResourceGeometryProcessor(!visual, !visual);
    (*mProcessors)[universe] = mProcessor;
  }
  return mProcessor;
}

  /*********************************\
   * Implements ISpindizzyBlockSet *
  \*********************************/
void SpindizzyModule::registerSurfaceProvider(IGeometricElement* element, bool recalculationSurroundings, IUniverse* universe) {
  getGeometryProcessor(universe, false)->registerGeometricElement(element, recalculationSurroundings);
  getGeometryProcessor(universe, true )->registerGeometricElement(element, recalculationSurroundings);
}

void SpindizzyModule::unregisterSurfaceProvider(IGeometricElement* element) {
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mVisualProcessor : cVisualProcessors) {
    mVisualProcessor.second->unregisterGeometricElement(element);
  }
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mPhysicalProcessor : cPhysicalProcessors) {
    mPhysicalProcessor.second->unregisterGeometricElement(element);
  }
}

void SpindizzyModule::setDirty() {
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mVisualProcessor : cVisualProcessors) {
    mVisualProcessor.second->setDirty();
  }
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mPhysicalProcessor : cPhysicalProcessors) {
    mPhysicalProcessor.second->setDirty();
  }
}

std::vector<ITileSurfaceTemplate*> SpindizzyModule::getTileSurfaces(IGeometricElement* element, ITileSurface::FaceDirection facing, bool visual) {
  std::map<IUniverse*, ResourceGeometryProcessor*>* mProcessors = getGeometryProcessors(visual);
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mProcessor : (*mProcessors)) {
    if (mProcessor.second->contains(element)) {
      return mProcessor.second->getTileSurfaces(element, facing);
    }
  }
  std::vector<ITileSurfaceTemplate*> mError;
  return mError; // TODO: Throw
}

std::vector<IWallSurfaceTemplate*> SpindizzyModule::getWallSurfaces(IGeometricElement* element, IWallSurface::FaceDirection facing, bool visual) {
  std::map<IUniverse*, ResourceGeometryProcessor*>* mProcessors = getGeometryProcessors(visual);
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mProcessor : (*mProcessors)) {
    if (mProcessor.second->contains(element)) {
      return mProcessor.second->getWallSurfaces(element, facing);
    }
  }
  std::vector<IWallSurfaceTemplate*> mError;
  return mError; // TODO: Throw
}

void SpindizzyModule::destroyTileTemplate(IGeometricElement* element, ITileSurfaceTemplate* tileTemplate, bool visual) {
  std::map<IUniverse*, ResourceGeometryProcessor*>* mProcessors = getGeometryProcessors(visual);
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mProcessor : (*mProcessors)) {
    if (mProcessor.second->contains(element)) {
      mProcessor.second->destroyTileTemplate(tileTemplate);
      return;
    }
  }
  // TODO: Throw
}

void SpindizzyModule::destroyWallTemplate(IGeometricElement* element, IWallSurfaceTemplate* wallTemplate, bool visual) {
  std::map<IUniverse*, ResourceGeometryProcessor*>* mProcessors = getGeometryProcessors(visual);
  for (std::pair<IUniverse*, ResourceGeometryProcessor*> mProcessor : (*mProcessors)) {
    if (mProcessor.second->contains(element)) {
      mProcessor.second->destroyWallTemplate(wallTemplate);
      return;
    }
  }
  // TODO: Throw
}

ResourceSurfaceRegistry* SpindizzyModule::getSurfaceRegistry(IUniverse* universe) {
  if (universe == nullptr) {
    return nullptr; // TODO: Throw
  }
  ResourceSurfaceRegistry* mSurfaceRegistry = cSurfaceRegistries[universe];
  if (mSurfaceRegistry == nullptr) {
    mSurfaceRegistry = new ResourceSurfaceRegistry();
    cSurfaceRegistries[universe] = mSurfaceRegistry;
  }
  return mSurfaceRegistry;
}

void SpindizzyModule::registerRollableSurface(ICollidableSurfaceElement* element, IRollableSurface* rollableSurface, IUniverse* universe) {
  getSurfaceRegistry(universe)->registerRollableSurface(element, rollableSurface, false);
}

void SpindizzyModule::registerWallSurface(ICollidableSurfaceElement* element, ICollidableWallSurface* wallSurface, IUniverse* universe) {
  getSurfaceRegistry(universe)->registerWallSurface(element, wallSurface);
}

void SpindizzyModule::unregisterSurfaces(ICollidableSurfaceElement* element) {
  for (std::pair<IUniverse*, ResourceSurfaceRegistry*> mSurfaceRegistry : cSurfaceRegistries) {
    mSurfaceRegistry.second->unregisterSurfaces(element);
  }
}

void SpindizzyModule::unregisterRollableSurface(IRollableSurface* rollableSurface) {
  for (std::pair<IUniverse*, ResourceSurfaceRegistry*> mSurfaceRegistry : cSurfaceRegistries) {
    mSurfaceRegistry.second->unregisterRollableSurface(rollableSurface);
  }
}

void SpindizzyModule::unregisterWallSurface(ICollidableWallSurface* wallSurface) {
  for (std::pair<IUniverse*, ResourceSurfaceRegistry*> mSurfaceRegistry : cSurfaceRegistries) {
    mSurfaceRegistry.second->unregisterWallSurface(wallSurface);
  }
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
    ElementHandlerSpindizzyBlock* mHandler = new ElementHandlerSpindizzyBlock(this, container);
    cElementHandlersSpindizzyBlock[container] = mHandler;
    container->addElement(mHandler);
    container->addArgumentValue(mHandler);
  }
  return cElementHandlersSpindizzyBlock[container];
}

void SpindizzyModule::removeElementHandlerSpindizzyBlock(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerSpindizzyBlock*>::iterator i = cElementHandlersSpindizzyBlock.find(container);
  if (i == cElementHandlersSpindizzyBlock.end()) {
    ElementHandlerSpindizzyBlock* mHandler = i->second;
    delete mHandler;
    cElementHandlersSpindizzyBlock.erase(container);
  }
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
  
void SpindizzyModule::removeElementHandlerItem(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerItem*>::iterator i = cItemElementHandlers.find(container);
  if (i == cItemElementHandlers.end()) {
    ElementHandlerItem* mHandler = i->second;
    delete mHandler;
    cItemElementHandlers.erase(container);
  }
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

void SpindizzyModule::registerInterceptingSurface(ICollidableSurfaceElement* element, IRollableSurface* surface, IUniverse* universe) {
  getSurfaceRegistry(universe)->registerRollableSurface(element, surface, true);
}

void SpindizzyModule::registerElement(IElementContainer* container, ElementSpindizzyLift* lift) {
  std::vector<ElementSpindizzyLift*> mLifts = cLiftsByContainer[container];
  mLifts.push_back(lift);
  cLiftsByContainer[container] = mLifts;
}
  
SpindizzyZoneTheme* SpindizzyModule::getTheme(const std::string& type) {
  return cThemes[type];
}

ISpindizzyZoneTheme* SpindizzyModule::getSelectedZoneTheme() {
  return cSelectedZoneTheme;
}

void SpindizzyModule::addSpindizzyZoneThemeListener(ISpindizzyZoneThemeListener* listener) {
  cZoneThemeSelectionListeners.push_back(listener);
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

void SpindizzyModule::applyDefaultTheme() {
  if (cDefaultTheme != nullptr) {
    cDefaultTheme->set();
  }
}

std::string SpindizzyModule::getThemeName(ISpindizzyZoneTheme* zoneTheme) {
  for (std::pair<std::string, SpindizzyZoneTheme*> mTheme : cThemes) {
    if (mTheme.second == zoneTheme) {
      return mTheme.first;
    }
  }
  return ""; // TODO: Throw
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

I3DModel* SpindizzyModule::createThemeIcon() {
  return cThemeModelIcon->createModel(&cThemeModelIconLocation, cThemeModelIconScale);
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

ICollisionData* SpindizzyModule::getNextEvent(Vertex& start, Vertex& end, IRollableSurface* currentSurface, float stepHeight) {
  return cActiveSurfaceRegistry->getNextEvent(start, end, currentSurface, stepHeight);
}

IRollableSurface* SpindizzyModule::getSurfaceAt(Vertex& location, float stepHeight) {
  return cActiveSurfaceRegistry->getSurfaceAt(location, stepHeight);
}

std::map<std::string, SpindizzyZoneTheme*> SpindizzyModule::getSpindizzyZoneThemes() {
  return cThemes;
}

void SpindizzyModule::spindizzyZoneThemeSelected(ISpindizzyZoneTheme* spindizzyZoneTheme) {
  cSelectedZoneTheme = spindizzyZoneTheme;
  cDefaultTheme = spindizzyZoneTheme;
  for (ISpindizzyZoneThemeListener* listener : cZoneThemeSelectionListeners) {
    listener->spindizzyZoneThemeSelected(cSelectedZoneTheme);
  }
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

void SpindizzyModule::removeElementHandlerSpindizzyDynamic(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerSpindizzyDynamic*>::iterator i = cDynamicElementHandlers.find(container);
  if (i == cDynamicElementHandlers.end()) {
    ElementHandlerSpindizzyDynamic* mHandler = i->second;
    delete mHandler;
    cDynamicElementHandlers.erase(container);
  }
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

void SpindizzyModule::removeElementHandlerZone(IElementContainer* container) {
  std::map<IElementContainer*, ElementHandlerZone*>::iterator i = cZoneElementHandlers.find(container);
  if (i == cZoneElementHandlers.end()) {
    ElementHandlerZone* mHandler = i->second;
    delete mHandler;
    cZoneElementHandlers.erase(container);
  }
}

extern "C" IModule* create(IResourceTypeRegistry* resourceManager) {
  return new SpindizzyModule(resourceManager);
}

extern "C" void destroy(IModule* module) {
  delete module;
}

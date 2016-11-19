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
#ifndef SPINDIZZY_MODULE_H
#define SPINDIZZY_MODULE_H

#include <IsoRealms/IModule.h>
#include <IsoRealms/LuaSupport/ArgumentValueCustomType.h>
#include <IsoRealms/Resources/ResourceType.h>
#include <IsoRealms/Resources/ResourceTypeSet.h>
#include <IsoRealms/Resources/Integer/Integer.h>

#include "DialogCameraScriptable.h"
#include "DialogElementSpindizzyBlock.h"
#include "DialogElementSpindizzyCraft.h"
#include "DialogElementSpindizzyEnemy.h"
#include "DialogElementSpindizzyItem.h"
#include "DialogElementSpindizzyLift.h"
#include "DialogElementSpindizzyWater.h"
#include "DialogElementSpindizzyZone.h"
#include "DialogGeneratorSpindizzyZoneThemeSelector.h"
#include "DialogLayerBackground.h"
#include "DialogLayerSpindizzyMap.h"
#include "DialogModelSpindizzyCraftGyroscope.h"
#include "DialogModelSpindizzyCraftPyramid.h"
#include "DialogModelSpindizzyJewel.h"
#include "DialogTextureSpindizzyCraftBall.h"
#include "DialogTexturesSpindizzyBlocksC64.h"
#include "DialogTexturesSpindizzyLiftsC64.h"
#include "ElementHandlerSpindizzyBlock.h"
#include "ElementHandlerSpindizzyDynamic.h"
//#include "DialogTexturesSpindizzyZoneTheme.h"
#include "ResourceCameraScriptable.h"
#include "ResourceElementSpindizzyBlock.h"
#include "ResourceElementSpindizzyCraft.h"
#include "ResourceElementSpindizzyEnemy.h"
#include "ResourceElementSpindizzyItem.h"
#include "ResourceElementSpindizzyLift.h"
#include "ResourceElementSpindizzyWater.h"
#include "ResourceElementSpindizzyZone.h"
#include "ResourceGeometryProcessor.h"
#include "ResourceLayerBackground.h"
#include "ResourceLayerSpindizzyMap.h"
#include "ResourceModelSpindizzyCraftGyroscope.h"
#include "ResourceModelSpindizzyCraftPyramid.h"
#include "ResourceModelSpindizzyJewel.h"
#include "ResourceSurfaceRegistry.h"
#include "ResourceTextureSpindizzyCraftBall.h"
#include "ResourceTexturesSpindizzyBlocksC64.h"
#include "ResourceTexturesSpindizzyLiftsC64.h"
// #include "ResourceTexturesSpindizzyZoneTheme.h"
#include "SpindizzyBlockState.h"

class SpindizzyModule:public IModule,
                      public ISpindizzyBlockSet,
                      public ISpindizzyGERALDSet,
                      public ISpindizzyEnemySet,
                      public ISpindizzyJewelSet,
                      public ISpindizzyLiftSet,
                      public ISpindizzyMapModule,
                      public ISpindizzyZoneModule,
                      public ISpindizzyZoneThemeAccessor,
                      public IThemeSource,
                      public IResource,
                      public ICameraAngleChangeListener, 
                      public ICameraSupplier,
                      public IModuleElementHandlerSpindizzyDynamic,
                      public IModuleElementHandlerZone {
  private:
  static const std::string TAG_CUSTOM_TYPE_BLOCK_STATE;

  static const std::string TAG_EDITOR_CONFIG_SPINDIZZY_ZONE_THEME_ICON;

  static const std::string TAG_RESOURCE_TYPE_CAMERA_SCRIPTABLE;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER;
  static const std::string TAG_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE;
  static const std::string TAG_RESOURCE_TYPE_LAYER_BACKGROUND;
  static const std::string TAG_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID;
  static const std::string TAG_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64;
  static const std::string TAG_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME;
  
  static const std::string NAME_RESOURCE_TYPE_CAMERA_SCRIPTABLE;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_BLOCK;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_CRAFT;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ENEMY;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ITEM;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_LIFT;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_WATER;
  static const std::string NAME_RESOURCE_TYPE_ELEMENT_SPINDIZZY_ZONE;
  static const std::string NAME_RESOURCE_TYPE_LAYER_BACKGROUND;
  static const std::string NAME_RESOURCE_TYPE_LAYER_SPINDIZZY_MAP;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_GYROSCOPE;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_CRAFT_PYRAMID;
  static const std::string NAME_RESOURCE_TYPE_MODEL_SPINDIZZY_JEWEL;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_CRAFT_BALL;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_BLOCKS_C64;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_LIFTS_C64;
  static const std::string NAME_RESOURCE_TYPE_TEXTURE_SPINDIZZY_ZONE_THEME;
  
  ResourceType<   ICamera,      ResourceCameraScriptable,             DialogCameraScriptable>                                   cResourceTypeCameraScriptable;
  ResourceType<   IElementType, ResourceElementSpindizzyBlock,        DialogElementSpindizzyBlock,        ISpindizzyBlockSet>   cResourceTypeElementSpindizzyBlock;
  ResourceType<   IElementType, ResourceElementSpindizzyCraft,        DialogElementSpindizzyCraft,        ISpindizzyGERALDSet>  cResourceTypeElementSpindizzyCraft;
  ResourceType<   IElementType, ResourceElementSpindizzyEnemy,        DialogElementSpindizzyEnemy,        ISpindizzyEnemySet>   cResourceTypeElementSpindizzyEnemy;
  ResourceType<   IElementType, ResourceElementSpindizzyItem,         DialogElementSpindizzyItem,         ISpindizzyJewelSet>   cResourceTypeElementSpindizzyItem;
  ResourceType<   IElementType, ResourceElementSpindizzyLift,         DialogElementSpindizzyLift,         ISpindizzyLiftSet>    cResourceTypeElementSpindizzyLift;
  ResourceType<   IElementType, ResourceElementSpindizzyWater,        DialogElementSpindizzyWater,        ISpindizzyBlockSet>   cResourceTypeElementSpindizzyWater;
  ResourceType<   IElementType, ResourceElementSpindizzyZone,         DialogElementSpindizzyZone,         ISpindizzyZoneModule> cResourceTypeElementSpindizzyZone;
  ResourceType<   ILayerType,   ResourceLayerBackground,              DialogLayerBackground>                                    cResourceTypeLayerBackground;
  ResourceType<   ILayerType,   ResourceLayerSpindizzyMap,            DialogLayerSpindizzyMap,            ISpindizzyMapModule>  cResourceTypeLayerSpindizzyMap;
  ResourceType<   I3DModelType, ResourceModelSpindizzyCraftGyroscope, DialogModelSpindizzyCraftGyroscope>                       cResourceTypeModelSpindizzyCraftGyroscope;
  ResourceType<   I3DModelType, ResourceModelSpindizzyCraftPyramid,   DialogModelSpindizzyCraftPyramid>                         cResourceTypeModelSpindizzyCraftPyramid;
  ResourceType<   I3DModelType, ResourceModelSpindizzyJewel,          DialogModelSpindizzyJewel>                                cResourceTypeModelSpindizzyJewel;
  ResourceType<   ITexture,     ResourceTextureSpindizzyCraftBall,    DialogTextureSpindizzyCraftBall>                          cResourceTypeTextureSpindizzyCraftBall;
  ResourceTypeSet<ITexture,     ResourceTexturesSpindizzyBlocksC64,   DialogTexturesSpindizzyBlocksC64,   ICameraSupplier>      cResourceTypeTextureSpindizzyBlocksC64;
  ResourceTypeSet<ITexture,     ResourceTexturesSpindizzyLiftsC64,    DialogTexturesSpindizzyLiftsC64>                          cResourceTypeTextureSpindizzyLiftsC64;
//  ResourceType<ITexture,     ResourceTexturesSpindizzyZoneTheme,   DialogTexturesSpindizzyZoneTheme>   cResourceTypeTextureSpindizzyZoneTheme;
    
  std::map<std::string, SpindizzyZoneTheme*> cThemes;
  std::map<std::string, SpindizzyZoneThemeTexture*> cTextures;
  std::map<std::string, SpindizzyZoneThemeColour*> cColours;
  std::map<IElementContainer*, std::vector<ElementSpindizzyLift*> > cLiftsByContainer;
  std::map<IElementContainer*, std::vector<ElementSpindizzyEnemy*> > cEnemiesByContainer;
  std::map<IElementContainer*, ElementHandlerSpindizzyDynamic*> cDynamicElementHandlers;
  std::map<IElementContainer*, ElementHandlerZone*> cZoneElementHandlers;
  std::map<IElementContainer*, ElementHandlerItem*> cItemElementHandlers;
  std::map<IElementContainer*, ElementHandlerSpindizzyBlock*> cElementHandlersSpindizzyBlock;
  std::map<IUniverse*, ResourceSurfaceRegistry*> cSurfaceRegistries;
  std::map<IUniverse*, ResourceGeometryProcessor*> cPhysicalProcessors;
  std::map<IUniverse*, ResourceGeometryProcessor*> cVisualProcessors;
  IScriptCall* cLiftMovedScript;
  SpindizzyLiftProperties cSpindizzyLiftProperties;
  Integer* cCollectedCount;
  IInteger* cLocked;
  ICamera* cCamera;
  bool cEditing;
  std::vector<SpindizzyBlockState*> cBlockStateData;
  std::vector<ICameraAngleChangeListener*> cCameraAngleChangeListeners;
  ArgumentValueCustomType<ElementHandlerSpindizzyBlock> cElementHandlerSpindizzyBlock;
  ArgumentValueCustomType<ElementHandlerSpindizzyDynamic> cDynamicElementHandlerArgument;
  ArgumentValueCustomType<ElementHandlerZone> cZoneElementHandlerArgument;
  ArgumentValueCustomType<ElementHandlerItem> cItemElementHandlerArgument;
  ArgumentValueCustomType<ElementSpindizzyZone> cArgumentValueZone;
  ISpindizzyZoneTheme* cSelectedZoneTheme;
  std::vector<ISpindizzyZoneThemeListener*> cZoneThemeSelectionListeners;
  
  bool cOverview;
  unsigned int cZoneCount;
  ISpindizzyZoneTheme* cDefaultTheme;
  I3DModelType* cThemeModelIcon;
  Vertex cThemeModelIconLocation;
  float cThemeModelIconScale;
  ResourceSurfaceRegistry* cActiveSurfaceRegistry;
  
  std::map<IUniverse*, ResourceGeometryProcessor*>* getGeometryProcessors(bool);
  ResourceGeometryProcessor* getGeometryProcessor(IUniverse*, bool);
  ResourceSurfaceRegistry* getSurfaceRegistry(IUniverse*);
  
  public:
  SpindizzyModule(IResourceTypeRegistry*);

  /***********************\
   * Scripting Interface *
  \***********************/
  void setOverview(bool);
  unsigned int getZoneCount();
  void setTheme(ISpindizzyZoneTheme*);
  void setActiveUniverse(IUniverse*);
  
  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*, IResourceRegistry*, DOMNodeWrapper*);
  void save(DOMNodeWriter*, IResourceLocator*);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  
  void createThemeResources(DOMNodeWrapper*, IResourceRegistry*);
  void createThemeTexture(const std::string&, IResourceRegistry*);
  void createThemeColour(const std::string&, IResourceRegistry*);

  /*************************************\
   * Implements IConditionElementIcons *
  \*************************************/
  I3DModel* getConditionElementIcon(ConditionElement*);
  
  /*********************************\
   * Implements ISpindizzyBlockSet *
  \*********************************/
  void registerSurfaceProvider(IGeometricElement*, bool, IUniverse*);
  void unregisterSurfaceProvider(IGeometricElement*);
  void setDirty();
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(IGeometricElement*, ITileSurface::FaceDirection, bool);
  std::vector<IWallSurfaceTemplate*> getWallSurfaces(IGeometricElement*, IWallSurface::FaceDirection, bool);
  void destroyTileTemplate(IGeometricElement*, ITileSurfaceTemplate*, bool);
  void destroyWallTemplate(IGeometricElement*, IWallSurfaceTemplate*, bool);
  void registerRollableSurface(ICollidableSurfaceElement*, IRollableSurface*, IUniverse*);
  void registerWallSurface(ICollidableSurfaceElement*, ICollidableWallSurface*, IUniverse*);
  void unregisterSurfaces(ICollidableSurfaceElement*);
  void unregisterRollableSurface(IRollableSurface*);
  void unregisterWallSurface(ICollidableWallSurface*);
  std::vector<ConditionElement*> getConditionElements();
  void staticChanged();
  SpindizzyBlockState* getBlockState(IBoolean*);
  ElementHandlerSpindizzyBlock* getElementHandlerSpindizzyBlock(IElementContainer*);
  void removeElementHandlerSpindizzyBlock(IElementContainer*);
  void setArgumentValue(ElementHandlerSpindizzyBlock*);

  /*********************************\
   * Implements ISpindizzyEnemySet *
  \*********************************/
  void registerElement(IElementContainer*, ElementSpindizzyEnemy*);
  
  /*********************************\
   * Implements ISpindizzyJewelSet *
  \*********************************/
  void itemCollected();
  ElementHandlerItem* getItemElementHandler(IElementContainer*);
  void removeElementHandlerItem(IElementContainer*);
  void setArgumentValue(ElementHandlerItem*);
  
  /********************************\
   * Implements ISpindizzyLiftSet *
  \********************************/
  SpindizzyLiftProperties* getSpindizzyLiftProperties();
  void executeLiftMovedScript();
  void registerInterceptingSurface(ICollidableSurfaceElement*, IRollableSurface*, IUniverse*);
  void registerElement(IElementContainer*, ElementSpindizzyLift*);
  
  /***********************************\
   * Implements ISpindizzyZoneModule *
  \***********************************/
  SpindizzyZoneTheme* getTheme(const std::string&);
  ISpindizzyZoneTheme* getSelectedZoneTheme();
  void addSpindizzyZoneThemeListener(ISpindizzyZoneThemeListener*);
  ElementHandlerZone* getZoneElementHandler(IElementContainer*);
  void removeElementHandlerZone(IElementContainer*);
  bool isOverview();
  IArgumentValue* getArgumentValue(const std::string&);
  void setArgumentValue(ElementSpindizzyZone*);
  void applyDefaultTheme();
  std::string getThemeName(ISpindizzyZoneTheme*);
  
  /***************************\
   * Implements IThemeSource *
  \***************************/
  SpindizzyZoneThemeTexture* getThemeTexture(const std::string&);
  SpindizzyZoneThemeColour* getThemeColour(const std::string&);
  std::string getThemeElement(SpindizzyZoneThemeTexture*);
  std::string getThemeElement(SpindizzyZoneThemeColour*);
  I3DModel* createThemeIcon();

  /*****************************************\
   * Implements ICameraAngleChangeListener *
  \*****************************************/
  void cameraAngleChanged(float);

  /******************************\
   * Implements ICameraSupplier *
  \******************************/
  void addCameraAngleChangeListener(ICameraAngleChangeListener*);

  /****************************************************\
   * Implements IModuleElementHandlerSpindizzyDynamic *
  \****************************************************/
  void setArgumentValue(ElementHandlerSpindizzyDynamic*);
  
  /****************************************\
   * Implements IModuleElementHandlerZone *
  \****************************************/
  void setArgumentValue(ElementHandlerZone*);
  
  /**********************************\
   * Implements ISpindizzyGERALDSet *
  \**********************************/
  ICollisionData* getNextEvent(Vertex&, Vertex&, IRollableSurface*, float);
  IRollableSurface* getSurfaceAt(Vertex&, float);

  /******************************************\
   * Implements ISpindizzyZoneThemeAccessor *
  \******************************************/
  std::map<std::string, SpindizzyZoneTheme*> getSpindizzyZoneThemes();
  void spindizzyZoneThemeSelected(ISpindizzyZoneTheme*);
  
  /************\
   * Multiple *
  \************/
  bool isEditing();
  bool isLocked();
  ElementHandlerSpindizzyDynamic* getDynamicElementHandler(IElementContainer*);
  void removeElementHandlerSpindizzyDynamic(IElementContainer*);
};

#endif

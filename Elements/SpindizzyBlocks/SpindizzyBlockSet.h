/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef SPINDIZZY_BLOCK_SET_H
#define SPINDIZZY_BLOCK_SET_H

#include <map>
#include <string>
#include <vector>

#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/HUD/IHUD.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSetChanger/ISpindizzyTextureSetChanger.h"
#include "../../Plugins/SpindizzyTextureSetChanger/IChangeableTextureSet.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

#include <IsoRealms/ConditionElement.h>
#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IElementSet.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/PluginRegistry.h>

#include "BlockStateCommand.h"
#include "HUDClue.h"
#include "ISpindizzyBlock.h"
#include "ISpindizzyBlockFactory.h"
#include "ISpindizzyBlockSet.h"
#include "SpindizzyBlock.h"
#include "SpindizzyBlockFactory.h"
#include "SpindizzyBlockHandler.h"
#include "SpindizzyWaterFactory.h"

class SpindizzyBlockSet:public ISpindizzyBlockSet,
                        public IChangeableTextureSet,
                        public IHUDComponentFactory,
                        public IZoneContextListener {
  private:
  class HUDClueData {
    private:
    std::string cName;
    Vertex cLocation;
    ISimpleModelFactory* cFactory;
    ISimpleModel* cModel;
    
    public:
    HUDClueData(ISimpleModelFactory*); 
    
    ISimpleModel* initClueData(DOMNodeWrapper*, const std::string&);
    
    ISimpleModelFactory* getFactory();
    
    void save(DOMNodeWriter*);
  };
    
  std::map<IElementContainer*, SpindizzyBlockHandler*> cElementHandlers;
  std::vector<IElementFactory*> cElementFactories;
  std::vector<IUserCommand*> cSpindizzyBlockCommands;
  std::vector<HUDClueData*> cHUDClueData;
  ICommandRegistry* cCommandRegistry;
  ICamera* cCamera;
  IHUD* cHUD;
  ISpindizzyTextureSet* cSpindizzyTextureSet;
  ISpindizzyTextureSet* cDummyTextureSet;
  ISpindizzyTextureSetChanger* cSpindizzyTextureSetController;
  ISurfaceProcessor* cPhysicalProcessor;
  ISurfaceProcessor* cVisualProcessor;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  IZoneContext* cZoneContext;
  std::vector<ConditionElement*> cBlockStates;
  std::vector<ISimpleModel*> cBlockStateClueModels;
  HUDClue* cHUDClue;

  void addBlockState(const std::string&, ISimpleModel*);
  ISpindizzyBlockFactory* getFactory(const std::string&);  
  
  public:
  SpindizzyBlockSet();

  /*************************\
   * Implements ElementSet *
  \*************************/
  SpindizzyBlockHandler* createHandler(IElementContainer*);
  
  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*, ICommandRegistry*);
  void destroy(IElement*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);
  void initElementsComplete();

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent(const std::string&);

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /*********************************\
   * Implements ISpindizzyBlockSet *
  \*********************************/
  void registerSurfaceProvider(ISurfaceProvider*);
  void unregisterSurfaceProvider(ISurfaceProvider*);
  void setDirty();
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection, bool);
  std::vector<IWallSurfaceTemplate*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection, bool);
  void destroyTileTemplate(ITileSurfaceTemplate*, bool);
  void destroyWallTemplate(IWallSurfaceTemplate*, bool);
  void notifyZoneAction(Zone*);

  void registerRollableSurface(IRollableSurface*);
  void registerWallSurface(ICollidableWallSurface*);
  std::vector<ConditionElement*> getConditionElements();
  void updateClue();

  /************************************\
   * Implements IChangeableTextureSet *
  \************************************/
  void setSpindizzyTextureSet(ISpindizzyTextureSet*);

  ~SpindizzyBlockSet();
};

#endif

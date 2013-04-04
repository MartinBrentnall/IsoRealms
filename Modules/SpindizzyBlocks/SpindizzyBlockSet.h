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

#include <IsoRealms/ConditionElement.h>
#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/OpenDialogCommand.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/Resources/3DModel/I3DModelFactory.h>
#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/HUDComponents/IHUDComponentFactory.h>
#include <IsoRealms/Resources/SurfaceProcessor/ISurfaceProcessor.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>

#include "HUDClue.h"
#include "ISpindizzyBlock.h"
#include "ISpindizzyBlockSet.h"
#include "ISpindizzyBlockType.h"
#include "SpindizzyBlock.h"
#include "SpindizzyBlockHandler.h"
#include "SpindizzyBlockState.h"
#include "SpindizzyBlockType.h"
#include "SpindizzyWaterType.h"

class SpindizzyBlockSet:public ISpindizzyBlockSet,
                        public IHUDComponentFactory {
  private:
  std::vector<SpindizzyBlockType*> cElementTypes;
  std::vector<SpindizzyWaterType*> cWaterTypes;
  std::vector<SpindizzyBlockState*> cBlockStateData;
  IEditingContext* cEditingContext;
  ICamera* cCamera;
  ISurfaceProcessor* cPhysicalProcessor;
  ISurfaceProcessor* cVisualProcessor;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  HUDClue* cHUDClue;
  bool cEditing;
  IComponentContainer* cComponentContainer;

  void addBlockState(const std::string&, I3DModel*);
  
  public:

  /*************************\
   * Implements ElementSet *
  \*************************/
  SpindizzyBlockHandler* createHandler();
  
  /**************************\
   * Implements IElementSet *
  \**************************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);
  void initElementsComplete();
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void setEditingContext(IEditingContext*, IResourceManager*);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent();

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
  std::vector<ConditionElement*> getConditionElements();

  void registerRollableSurface(IRollableSurface*);
  void registerWallSurface(ICollidableWallSurface*);
  void updateClue();
  bool isEditing();
  void staticChanged();

  ~SpindizzyBlockSet();
};

#endif

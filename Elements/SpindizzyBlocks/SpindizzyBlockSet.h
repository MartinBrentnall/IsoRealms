/*
 * Copyright 2009,2010 Martin Brentnall
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

#include <string>
#include <vector>

#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/SpindizzyTextureSetDummy.h"
#include "../../Plugins/SpindizzyTextureSetChanger/ISpindizzyTextureSetChanger.h"
#include "../../Plugins/SpindizzyTextureSetChanger/IChangeableTextureSet.h"
#include "../../Plugins/SurfaceProcessor/ISurfaceProcessor.h"

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IElementSet.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/PluginRegistry.h>

#include "BlockStateCommand.h"
#include "ISpindizzyBlockFactory.h"
#include "ISpindizzyBlockSet.h"
#include "SpindizzyBlock.h"
#include "SpindizzyBlockFactory.h"
#include "SpindizzyIceBlock.h"
#include "SpindizzySwitchBlock.h"
#include "SpindizzyTrampolineBlock.h"
#include "SpindizzyWaterFactory.h"

class SpindizzyBlockSet:public ISpindizzyBlockSet,
                        public IChangeableTextureSet {
  private:
  static const std::string PLAIN;
  static const std::string ARROW_NORTH;
  static const std::string ARROW_EAST;
  static const std::string ARROW_SOUTH;
  static const std::string ARROW_WEST;
  static const std::string ICE;
  static const std::string TRAMPOLINE;
  static const std::string SWITCH_CIRCLE_BOTH;
  static const std::string SWITCH_CIRCLE_LEFT;
  static const std::string SWITCH_CIRCLE_RIGHT;
  static const std::string SWITCH_CIRCLE_NONE;
  static const std::string SWITCH_SQUARE_BOTH;
  static const std::string SWITCH_SQUARE_LEFT;
  static const std::string SWITCH_SQUARE_RIGHT;
  static const std::string SWITCH_SQUARE_NONE;
  static const std::string SWITCH_DIAMOND_BOTH;
  static const std::string SWITCH_DIAMOND_LEFT;
  static const std::string SWITCH_DIAMOND_RIGHT;
  static const std::string SWITCH_DIAMOND_NONE;

  static const int BLOCK_STATES;

  std::vector<IElementFactory*> cElementFactories;
  std::vector<IUserCommand*> cSpindizzyBlockCommands;
  ICommandRegistry* cCommandRegistry;
  ISpindizzyTextureSet* cSpindizzyTextureSet;
  ISpindizzyTextureSet* cDummyTextureSet;
  ISpindizzyTextureSetChanger* cSpindizzyTextureSetController;
  ISurfaceProcessor* cSurfaceProcessor;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  std::map<std::string, bool*> cBlockStates;

  class BlockFactory:public SpindizzyBlockFactory {
    private:
    ISpindizzyTextureSet::TextureType cTileSurfaceTexture;

    public:
    BlockFactory(std::string, ISpindizzyBlockSet*, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class IceFactory:public SpindizzyBlockFactory {
    public:
    IceFactory(std::string, ISpindizzyBlockSet*, ISpindizzyTextureSet**);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class SwitchFactory:public SpindizzyBlockFactory {
    private:
    ISpindizzyTextureSet::TextureType cTileSurfaceTexture;

    public:
    SwitchFactory(std::string, ISpindizzyBlockSet*, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class TrampolineFactory:public SpindizzyBlockFactory {
    public:
    TrampolineFactory(std::string, ISpindizzyBlockSet*, ISpindizzyTextureSet**);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  void addBlockState(const std::string&);

  public:
  SpindizzyBlockSet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*, ICommandRegistry*);
  void destroy(IElement*);
  void save(DOMNodeWriter*);

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
  std::vector<ITileSurfaceTemplate*> getTileSurfaces(ISurfaceProvider*, ITileSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(ISurfaceProvider*, IWallSurface::FaceDirection);
  void notifyZoneAction(Zone*);

  void registerRollableSurface(IRollableSurface*);
  void registerWallSurface(ICollidableWallSurface*);

  /************************************\
   * Implements IChangeableTextureSet *
  \************************************/
  void setSpindizzyTextureSet(ISpindizzyTextureSet*);

  ~SpindizzyBlockSet();
};

#endif

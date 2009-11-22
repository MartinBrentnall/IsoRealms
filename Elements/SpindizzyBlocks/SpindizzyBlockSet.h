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
#ifndef SPINDIZZY_BLOCK_SET_H
#define SPINDIZZY_BLOCK_SET_H

#include <string>
#include <vector>

#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/SpindizzyTextureSet/SpindizzyTextureSetDummy.h"
#include "../../Plugins/SpindizzyTextureSetChanger/ISpindizzyTextureSetChanger.h"
#include "../../Plugins/SpindizzyTextureSetChanger/IChangeableTextureSet.h"
#include "../../Plugins/RollableSurfaceCalculator/IRollableSurfaceCalculator.h"

#include "../../Global/IElementSet.h"
#include "../../Global/IPlugin.h"
#include "../../Global/PlugSocket.h"
#include "../../Global/PluginRegistry.h"

#include "ISpindizzyBlockFactory.h"
#include "ISurfaceCalculator.h"
#include "SpindizzyBlock.h"
#include "SpindizzyBlockFactory.h"
#include "SpindizzyIceBlock.h"
#include "SpindizzySwitchBlock.h"
#include "SpindizzyTrampolineBlock.h"
#include "SpindizzyWaterFactory.h"

class SpindizzyBlockSet:public IElementSet,
                        public ISurfaceCalculator,
                        public IChangeableTextureSet {
  private:
  static const std::string PLAIN;
  static const std::string ARROW_NORTH;
  static const std::string ARROW_EAST;
  static const std::string ARROW_SOUTH;
  static const std::string ARROW_WEST;
  static const std::string ICE;
  static const std::string TRAMPOLINE;
  static const std::string SWITCH_CICRLE_BOTH;
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

  std::vector<IElementFactory*> cElementFactories;
  ISpindizzyTextureSet* cSpindizzyTextureSet;
  ISpindizzyTextureSetChanger* cSpindizzyTextureSetController;
  IRollableSurfaceCalculator* cRollableSurfaceCalculator;

  class BlockFactory:public SpindizzyBlockFactory {
    private:
    ISpindizzyTextureSet::TextureType cRollableSurfaceTexture;

    public:
    BlockFactory(std::string, IElementSet*, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class IceFactory:public SpindizzyBlockFactory {
    public:
    IceFactory(std::string, IElementSet*, ISpindizzyTextureSet**);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class SwitchFactory:public SpindizzyBlockFactory {
    private:
    ISpindizzyTextureSet::TextureType cRollableSurfaceTexture;

    public:
    SwitchFactory(std::string, IElementSet*, ISpindizzyTextureSet**, ISpindizzyTextureSet::TextureType);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  class TrampolineFactory:public SpindizzyBlockFactory {
    public:
    TrampolineFactory(std::string, IElementSet*, ISpindizzyTextureSet**);

    /************************************\
     * Implements SpindizzyBlockFactory *
    \************************************/
    AbstractSpindizzyBlock* createBlock(BlockLocation*, BlockLocation*, ISpindizzyTextureSet**, SpindizzyBlockProperties*, bool);
  };

  public:
  SpindizzyBlockSet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void destroy(IElement*);
  void save(DOMNodeWriter*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /*****************************************\
   * Implements IRollableSurfaceCalculator *
  \*****************************************/
  void registerRollableSurfaceProvider(IRollableSurfaceProvider*);
  void unregisterRollableSurfaceProvider(IRollableSurfaceProvider*);
  void setDirty();
  std::vector<IRollableSurface*> getRollableSurfaces(IRollableSurfaceProvider*, IRollableSurface::FaceDirection);
  std::vector<IWallSurface*> getWallSurfaces(IRollableSurfaceProvider*, IWallSurface::FaceDirection);

  /************************************\
   * Implements IChangeableTextureSet *
  \************************************/
  void setSpindizzyTextureSet(ISpindizzyTextureSet*);
};

#endif

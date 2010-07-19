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
#include "SpindizzyBlockSet.h"

const std::string SpindizzyBlockSet::PLAIN = "Plain";
const std::string SpindizzyBlockSet::ARROW_NORTH = "ArrowNorth";
const std::string SpindizzyBlockSet::ARROW_EAST = "ArrowEast";
const std::string SpindizzyBlockSet::ARROW_SOUTH = "ArrowSouth";
const std::string SpindizzyBlockSet::ARROW_WEST = "ArrowWest";
const std::string SpindizzyBlockSet::ICE = "Ice";
const std::string SpindizzyBlockSet::TRAMPOLINE = "Trampoline";
const std::string SpindizzyBlockSet::SWITCH_CIRCLE_BOTH = "SwitchCircleBoth";
const std::string SpindizzyBlockSet::SWITCH_CIRCLE_LEFT = "SwitchCircleLeft";
const std::string SpindizzyBlockSet::SWITCH_CIRCLE_RIGHT = "SwitchCircleRight";
const std::string SpindizzyBlockSet::SWITCH_CIRCLE_NONE = "SwitchCircleNone";
const std::string SpindizzyBlockSet::SWITCH_SQUARE_BOTH = "SwitchSquareBoth";
const std::string SpindizzyBlockSet::SWITCH_SQUARE_LEFT = "SwitchSquareLeft";
const std::string SpindizzyBlockSet::SWITCH_SQUARE_RIGHT = "SwitchSquareRight";
const std::string SpindizzyBlockSet::SWITCH_SQUARE_NONE = "SwitchSquareNone";
const std::string SpindizzyBlockSet::SWITCH_DIAMOND_BOTH = "SwitchDiamondBoth";
const std::string SpindizzyBlockSet::SWITCH_DIAMOND_LEFT = "SwitchDiamondLeft";
const std::string SpindizzyBlockSet::SWITCH_DIAMOND_RIGHT = "SwitchDiamondRight";
const std::string SpindizzyBlockSet::SWITCH_DIAMOND_NONE = "SwitchDiamondNone";

const int SpindizzyBlockSet::BLOCK_STATES = 11;

SpindizzyBlockSet::SpindizzyBlockSet() {
  cElementFactories.push_back(new BlockFactory(PLAIN, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::PLAIN));
  cElementFactories.push_back(new BlockFactory(ARROW_NORTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_NORTH));
  cElementFactories.push_back(new BlockFactory(ARROW_EAST, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_EAST));
  cElementFactories.push_back(new BlockFactory(ARROW_SOUTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_SOUTH));
  cElementFactories.push_back(new BlockFactory(ARROW_WEST, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_WEST));
  cElementFactories.push_back(new IceFactory(ICE, this, &cSpindizzyTextureSet));
  cElementFactories.push_back(new TrampolineFactory(TRAMPOLINE, this, &cSpindizzyTextureSet));
  cElementFactories.push_back(new SwitchFactory(SWITCH_CIRCLE_BOTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_CIRCLE_BOTH));
  cElementFactories.push_back(new SwitchFactory(SWITCH_CIRCLE_LEFT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_CIRCLE_LEFT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_CIRCLE_RIGHT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_CIRCLE_RIGHT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_CIRCLE_NONE, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_CIRCLE_NONE));
  cElementFactories.push_back(new SwitchFactory(SWITCH_SQUARE_BOTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_SQUARE_BOTH));
  cElementFactories.push_back(new SwitchFactory(SWITCH_SQUARE_LEFT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_SQUARE_LEFT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_SQUARE_RIGHT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_SQUARE_RIGHT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_SQUARE_NONE, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_SQUARE_NONE));
  cElementFactories.push_back(new SwitchFactory(SWITCH_DIAMOND_BOTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_DIAMOND_BOTH));
  cElementFactories.push_back(new SwitchFactory(SWITCH_DIAMOND_LEFT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_DIAMOND_LEFT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_DIAMOND_RIGHT, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_DIAMOND_RIGHT));
  cElementFactories.push_back(new SwitchFactory(SWITCH_DIAMOND_NONE, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_DIAMOND_NONE));
  cElementFactories.push_back(new SpindizzyWaterFactory(&cSpindizzyTextureSet, this));

  assignDummyPlugin(&cDummyTextureSet, "SpindizzyTextureSet");
  cSpindizzyTextureSet = cDummyTextureSet;
  cSpindizzyTextureSetController = NULL;
  assignDummyPlugin(&cSurfaceProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");

  addBlockState(SWITCH_CIRCLE_BOTH);
  addBlockState(SWITCH_CIRCLE_LEFT);
  addBlockState(SWITCH_CIRCLE_RIGHT);
  addBlockState(SWITCH_CIRCLE_NONE);
  addBlockState(SWITCH_SQUARE_BOTH);
  addBlockState(SWITCH_SQUARE_LEFT);
  addBlockState(SWITCH_SQUARE_RIGHT);
  addBlockState(SWITCH_SQUARE_NONE);
  addBlockState(SWITCH_DIAMOND_BOTH);
  addBlockState(SWITCH_DIAMOND_LEFT);
  addBlockState(SWITCH_DIAMOND_RIGHT);
}

void SpindizzyBlockSet::addBlockState(const std::string& name) {
  bool* mState = new bool(false);
  cBlockStates.push_back(new ConditionElement(name, mState));
  IUserCommand* mStateOnCommand = new BlockStateCommand(name, mState, true);
  IUserCommand* mStateOffCommand = new BlockStateCommand(name, mState, false);
  cSpindizzyBlockCommands.push_back(mStateOnCommand);
  cSpindizzyBlockCommands.push_back(mStateOffCommand);
}

std::vector<PlugSocket*> SpindizzyBlockSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  if (cSpindizzyTextureSetController == NULL) {
    if (cSpindizzyTextureSet == cDummyTextureSet) {
      mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger"));
    }
    mSockets.push_back(new PlugSocket("SpindizzyTextureSet"));
  } else {
    mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger"));
  }
  mSockets.push_back(new PlugSocket("SurfaceProcessor"));
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  return mSockets;
}

void SpindizzyBlockSet::setSpindizzyTextureSet(ISpindizzyTextureSet* textureSet) {
  cSpindizzyTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

void SpindizzyBlockSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else if (socket->getType() == "SpindizzyTextureSet") {
    if (assignPlugin(implementation, &cSpindizzyTextureSet, *socket)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<ISpindizzyBlockFactory*>(cElementFactories[i])->signalAllElementsDirty();
      }
    }
  } else if (socket->getType() == "SpindizzyTextureSetChanger") {
    ISpindizzyTextureSetChanger* mPreviousController = cSpindizzyTextureSetController;
    if (assignPlugin(implementation, &cSpindizzyTextureSetController, *socket, false)) {
      if (mPreviousController != NULL) {
        cSpindizzyTextureSetController->setControlObject(NULL);
      }
      if (cSpindizzyTextureSetController != NULL) {
        cSpindizzyTextureSetController->setControlObject(this);
      }
    }
  } else if (socket->getType() == "SurfaceProcessor") {
    ISurfaceProcessor* mPreviousSurfaceProcessor = cSurfaceProcessor;
    if (assignPlugin(implementation, &cSurfaceProcessor, *socket)) {
      mPreviousSurfaceProcessor->reinitialise();
    }
  } else {
    std::cout << "WARNING!  I don't know what to do with the implementation I was given!" << std::endl;
  }
}

IPlugin* SpindizzyBlockSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "CollidableSurfaceRegistry")  {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "SpindizzyTextureSet")        {return cSpindizzyTextureSet;}
  if (socket->getType() == "SurfaceProcessor")           {return cSurfaceProcessor;}
  if (socket->getType() == "SpindizzyTextureSetChanger") {return cSpindizzyTextureSetController;}
  // TODO: Throw wobbly!
  return NULL;
}

std::vector<IElementFactory*> SpindizzyBlockSet::getElementFactories() {
  return cElementFactories;
}

void SpindizzyBlockSet::setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*, ICommandRegistry* commandRegistry) {
  cCommandRegistry = commandRegistry;
  for (unsigned int i = 0; i < cSpindizzyBlockCommands.size(); i++) {
    cCommandRegistry->registerCommand(cSpindizzyBlockCommands[i]);
  }
}

void SpindizzyBlockSet::destroy(IElement* element) {
  delete element;
}

void SpindizzyBlockSet::save(DOMNodeWriter* node) {
  // Nothing to do
}

void SpindizzyBlockSet::registerSurfaceProvider(ISurfaceProvider* provider) {
  cSurfaceProcessor->registerSurfaceProvider(provider);
}

void SpindizzyBlockSet::unregisterSurfaceProvider(ISurfaceProvider* provider) {
  cSurfaceProcessor->unregisterSurfaceProvider(provider);
}

void SpindizzyBlockSet::setDirty() {
  cSurfaceProcessor->setDirty();
}

std::vector<ITileSurfaceTemplate*> SpindizzyBlockSet::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection facing) {
  return cSurfaceProcessor->getTileSurfaces(provider, facing);
}

std::vector<IWallSurface*> SpindizzyBlockSet::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing) {
  return cSurfaceProcessor->getWallSurfaces(provider, facing);
}

void SpindizzyBlockSet::registerRollableSurface(IRollableSurface* rollableSurface) {
  cCollidableSurfaceRegistry->registerRollableSurface(rollableSurface);
}

void SpindizzyBlockSet::registerWallSurface(ICollidableWallSurface* wallSurface) {
  cCollidableSurfaceRegistry->registerWallSurface(wallSurface);
}

std::vector<ConditionElement*> SpindizzyBlockSet::getConditionElements() {
  return cBlockStates;
}

void SpindizzyBlockSet::notifyZoneAction(Zone* zone) {
  cSurfaceProcessor->notifyZoneAction(zone);
}

SpindizzyBlockSet::BlockFactory::BlockFactory(std::string name, ISpindizzyBlockSet* elementSet, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType tileSurfaceTexture) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  cTileSurfaceTexture = tileSurfaceTexture;
}

AbstractSpindizzyBlock* SpindizzyBlockSet::BlockFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyBlock(this, startLocation, endLocation, textureSet, cTileSurfaceTexture, blockProperties, addition);
}

SpindizzyBlockSet::IceFactory::IceFactory(std::string name, ISpindizzyBlockSet* elementSet, ISpindizzyTextureSet** textureSet) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  // Nothing to do.
}

AbstractSpindizzyBlock* SpindizzyBlockSet::IceFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyIceBlock(this, startLocation, endLocation, textureSet, blockProperties, addition);
}

SpindizzyBlockSet::SwitchFactory::SwitchFactory(std::string name, ISpindizzyBlockSet* elementSet, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType tileSurfaceTexture) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  cTileSurfaceTexture = tileSurfaceTexture;
}

AbstractSpindizzyBlock* SpindizzyBlockSet::SwitchFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzySwitchBlock(this, startLocation, endLocation, textureSet, cTileSurfaceTexture, blockProperties, addition);
}

SpindizzyBlockSet::TrampolineFactory::TrampolineFactory(std::string name, ISpindizzyBlockSet* elementSet, ISpindizzyTextureSet** textureSet) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  // Nothing to do.
}

AbstractSpindizzyBlock* SpindizzyBlockSet::TrampolineFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyTrampolineBlock(this, startLocation, endLocation, textureSet, blockProperties, addition);
}

SpindizzyBlockSet::~SpindizzyBlockSet() {
  cSurfaceProcessor->reinitialise();
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyBlockSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}

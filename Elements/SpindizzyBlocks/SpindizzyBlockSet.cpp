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
const std::string SpindizzyBlockSet::SWITCH_CICRLE_BOTH = "SwitchCircleBoth";
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

SpindizzyBlockSet::SpindizzyBlockSet() {
  cElementFactories.push_back(new BlockFactory(PLAIN, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::PLAIN));
  cElementFactories.push_back(new BlockFactory(ARROW_NORTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_NORTH));
  cElementFactories.push_back(new BlockFactory(ARROW_EAST, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_EAST));
  cElementFactories.push_back(new BlockFactory(ARROW_SOUTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_SOUTH));
  cElementFactories.push_back(new BlockFactory(ARROW_WEST, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::ARROW_WEST));
  cElementFactories.push_back(new IceFactory(ICE, this, &cSpindizzyTextureSet));
  cElementFactories.push_back(new TrampolineFactory(TRAMPOLINE, this, &cSpindizzyTextureSet));
  cElementFactories.push_back(new SwitchFactory(SWITCH_CICRLE_BOTH, this, &cSpindizzyTextureSet, ISpindizzyTextureSet::SWITCH_CIRCLE_BOTH));
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
  std::string mDummyName("SpindizzyTextureSet");
  cDummyTextureSet = dynamic_cast<ISpindizzyTextureSet*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cDummyTextureSet == NULL) {
    std::cout << "Warning: dynamic_cast failed for dummy spindizzy texture set!" << std::endl;
    // TODO: Throw wobbly
  }
  cSpindizzyTextureSet = cDummyTextureSet;
  cSpindizzyTextureSetController = NULL;
  mDummyName = "SurfaceProcessor";
  cSurfaceProcessor = dynamic_cast<ISurfaceProcessor*>(PluginRegistry::getDummyPlugin(mDummyName));
  if (cSurfaceProcessor == NULL) {
    std::cout << "Warning: dynamic_cast failed for dummy surface processor!" << std::endl;
    // TODO: Throw wobbly
  }
}

std::vector<PlugSocket*> SpindizzyBlockSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  if (cSpindizzyTextureSetController == NULL) {
    if (cSpindizzyTextureSet == cDummyTextureSet) {
      mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger", "")); // TODO: Default the second parameter to empty string!
    }
    mSockets.push_back(new PlugSocket("SpindizzyTextureSet", "")); // TODO: Default the second parameter to empty string!
  } else {
    mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger", "")); // TODO: Default the second parameter to empty string!
  }
  mSockets.push_back(new PlugSocket("SurfaceProcessor", "")); // TODO: Default the second parameter to empty string!
  return mSockets;
}

void SpindizzyBlockSet::setSpindizzyTextureSet(ISpindizzyTextureSet* textureSet) {
  if (textureSet != NULL) {
    std::cout << "Texture set is not NULL!" << std::endl;
  }
  cSpindizzyTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

void SpindizzyBlockSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "SpindizzyTextureSet") {
    if (implementation == cSpindizzyTextureSet) {
      return;
    }
    if (implementation == NULL) {
      cSpindizzyTextureSet = cDummyTextureSet;
    } else {
      ISpindizzyTextureSet* mSpindizzyTextureSet = dynamic_cast<ISpindizzyTextureSet*>(implementation);
      if (mSpindizzyTextureSet == NULL) {
        std::cout << "Warning: dynamic_cast failed for spindizzy texture set!" << std::endl;
      } else {
        cSpindizzyTextureSet = mSpindizzyTextureSet;
      }
    }
    for (unsigned int i = 0; i < cElementFactories.size(); i++) {
      static_cast<ISpindizzyBlockFactory*>(cElementFactories[i])->signalAllElementsDirty();
    }
  } else if (socket->getType() == "SurfaceProcessor") {
    if (implementation == cSurfaceProcessor) {
      return;
    }
    if (implementation == NULL) {
      std::string mDummyName("SurfaceProcessor");
      cSurfaceProcessor->reinitialise();
      cSurfaceProcessor = dynamic_cast<ISurfaceProcessor*>(PluginRegistry::getDummyPlugin(mDummyName));
    } else {
      ISurfaceProcessor* mSurfaceProcessor = dynamic_cast<ISurfaceProcessor*>(implementation);
      if (mSurfaceProcessor == NULL) {
        std::cout << "Warning: dynamic_cast failed for surface processor!" << std::endl;
        return;
      } else {
        // Surface processor might still be in use by other element sets
        cSurfaceProcessor->reinitialise();
        cSurfaceProcessor = mSurfaceProcessor;
      }
    }
  } else if (socket->getType() == "SpindizzyTextureSetChanger") {
    if (implementation == NULL) {
      if (cSpindizzyTextureSetController != NULL) {
        cSpindizzyTextureSetController->setControlObject(NULL);
      }
      cSpindizzyTextureSetController = NULL;
      cSpindizzyTextureSet = cDummyTextureSet;
    } else {
      ISpindizzyTextureSetChanger* mSpindizzyTextureSetController = dynamic_cast<ISpindizzyTextureSetChanger*>(implementation);
      if (mSpindizzyTextureSetController == NULL) {
        std::cout << "Warning: dynamic_cast failed for spindizzy texture set!" << std::endl;
      } else {
        if (cSpindizzyTextureSetController != NULL) {
          cSpindizzyTextureSetController->setControlObject(NULL);
        }
        cSpindizzyTextureSetController = mSpindizzyTextureSetController;
        cSpindizzyTextureSet = cDummyTextureSet;
        cSpindizzyTextureSetController->setControlObject(this);
      }
    }
  } else {
    std::cout << "WARNING!  I don't know what to do with the implementation I was given!" << std::endl;
  }
}

IPlugin* SpindizzyBlockSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "SpindizzyTextureSet") {
    if (!PluginRegistry::isDummyPlugin(cSpindizzyTextureSet)) {
      return (IPlugin*) cSpindizzyTextureSet;
    }
  } else if (socket->getType() == "SurfaceProcessor") {
    if (!PluginRegistry::isDummyPlugin(cSurfaceProcessor)) {
      return (IPlugin*) cSurfaceProcessor;
    }
  } else if (socket->getType() == "SpindizzyTextureSetChanger") {
    return cSpindizzyTextureSetController;
  }
  // TODO: Throw wobbly!
  return NULL;
}

std::string SpindizzyBlockSet::getName() {
  return "Spindizzy Blocks";
}

std::vector<IElementFactory*> SpindizzyBlockSet::getElementFactories() {
  return cElementFactories;
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

std::vector<ITileSurface*> SpindizzyBlockSet::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection facing) {
  return cSurfaceProcessor->getTileSurfaces(provider, facing);
}

std::vector<IWallSurface*> SpindizzyBlockSet::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing) {
  return cSurfaceProcessor->getWallSurfaces(provider, facing);
}

void SpindizzyBlockSet::notifyZoneAction(Zone* zone) {
  cSurfaceProcessor->notifyZoneAction(zone);
}

SpindizzyBlockSet::BlockFactory::BlockFactory(std::string name, IElementSet* elementSet, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType tileSurfaceTexture) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  cTileSurfaceTexture = tileSurfaceTexture;
}

AbstractSpindizzyBlock* SpindizzyBlockSet::BlockFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyBlock(this, startLocation, endLocation, textureSet, cTileSurfaceTexture, blockProperties, addition);
}

SpindizzyBlockSet::IceFactory::IceFactory(std::string name, IElementSet* elementSet, ISpindizzyTextureSet** textureSet) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  // Nothing to do.
}

AbstractSpindizzyBlock* SpindizzyBlockSet::IceFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzyIceBlock(this, startLocation, endLocation, textureSet, blockProperties, addition);
}

SpindizzyBlockSet::SwitchFactory::SwitchFactory(std::string name, IElementSet* elementSet, ISpindizzyTextureSet** textureSet, ISpindizzyTextureSet::TextureType tileSurfaceTexture) : SpindizzyBlockFactory(name, textureSet, elementSet) {
  cTileSurfaceTexture = tileSurfaceTexture;
}

AbstractSpindizzyBlock* SpindizzyBlockSet::SwitchFactory::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, ISpindizzyTextureSet** textureSet, SpindizzyBlockProperties* blockProperties, bool addition) {
  return new SpindizzySwitchBlock(this, startLocation, endLocation, textureSet, cTileSurfaceTexture, blockProperties, addition);
}

SpindizzyBlockSet::TrampolineFactory::TrampolineFactory(std::string name, IElementSet* elementSet, ISpindizzyTextureSet** textureSet) : SpindizzyBlockFactory(name, textureSet, elementSet) {
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

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

SpindizzyBlockSet::SpindizzyBlockSet() {
  cElementFactories.push_back(new SpindizzyWaterFactory(&cSpindizzyTextureSet, this));
  assignDummyPlugin(&cDummyTextureSet, "SpindizzyTextureSet");
  cSpindizzyTextureSet = cDummyTextureSet;
  cSpindizzyTextureSetController = NULL;
  assignDummyPlugin(&cCamera, "Camera");
  assignDummyPlugin(&cHUD, "HUD");
  assignDummyPlugin(&cSurfaceProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cHUDClue = new HUDClue(cCamera);
}

void SpindizzyBlockSet::addBlockState(const std::string& name, ISimpleModel* model) {
  bool* mState = new bool(false);
  cBlockStates.push_back(new ConditionElement(name, mState));
  cBlockStateClueModels.push_back(model);
  IUserCommand* mStateOnCommand = new BlockStateCommand(name, mState, true);
  IUserCommand* mStateOffCommand = new BlockStateCommand(name, mState, false);
  cSpindizzyBlockCommands.push_back(mStateOnCommand);
  cSpindizzyBlockCommands.push_back(mStateOffCommand);
  cCommandRegistry->registerCommand(mStateOnCommand);
  cCommandRegistry->registerCommand(mStateOffCommand);
}

std::vector<PlugSocket*> SpindizzyBlockSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  for (unsigned int i = 0; i <= cClueModelFactories.size(); i++) {
    std::ostringstream mSocketID;
    mSocketID << i;
    mSockets.push_back(new PlugSocket("3DModel", mSocketID.str()));
  }
  if (cSpindizzyTextureSetController == NULL) {
    if (cSpindizzyTextureSet == cDummyTextureSet) {
      mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger"));
    }
    mSockets.push_back(new PlugSocket("SpindizzyTextureSet"));
  } else {
    mSockets.push_back(new PlugSocket("SpindizzyTextureSetChanger"));
  }
  mSockets.push_back(new PlugSocket("Camera"));
  mSockets.push_back(new PlugSocket("HUD"));
  mSockets.push_back(new PlugSocket("SurfaceProcessor"));
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  mSockets.push_back(new PlugSocket("ZoneContext"));
  return mSockets;
}

void SpindizzyBlockSet::setSpindizzyTextureSet(ISpindizzyTextureSet* textureSet) {
  cSpindizzyTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

void SpindizzyBlockSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    ISimpleModelFactory* mNewModelFactory;
    if (implementation != NULL) {
      assignPlugin(implementation, &mNewModelFactory, *socket);
      ISimpleModel* mNewModel = mNewModelFactory->createModel(&cClueModelLocation);
      if (mIndex == cClueModelFactories.size()) {
        cClueModelFactories.push_back(mNewModelFactory);
        cClueModels.push_back(mNewModel);
      } else {
        cClueModelFactories[mIndex]->destroyModel(cClueModels[mIndex]);
        cClueModelFactories[mIndex] = mNewModelFactory;
        cClueModels[mIndex] = mNewModel;
      }
    } else { // TODO: Range check?
      cClueModelFactories[mIndex]->destroyModel(cClueModels[mIndex]);
      cClueModelFactories.erase(cClueModelFactories.begin() + mIndex);
      cClueModels.erase(cClueModels.begin() + mIndex);
    }
  } else if (socket->getType() == "Camera") {
    assignPlugin(implementation, &cCamera, *socket);
    cHUDClue->setCamera(cCamera);
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else if (socket->getType() == "SpindizzyTextureSet") {
    if (assignPlugin(implementation, &cSpindizzyTextureSet, *socket)) {
      for (unsigned int i = 0; i < cElementFactories.size(); i++) {
        static_cast<ISpindizzyBlockFactory*>(cElementFactories[i])->signalAllElementsDirty();
      }
    }
  } else if (socket->getType() == "HUD") {
    IHUD* mPreviousHUD = cHUD;
    if (assignPlugin(implementation, &cHUD, *socket)) {
      mPreviousHUD->unregisterHUDComponentFactory(this);
      cHUD->registerHUDComponentFactory(this);
    }
  } else if (socket->getType() == "SpindizzyTextureSetChanger") {
    ISpindizzyTextureSetChanger* mPreviousController = cSpindizzyTextureSetController;
    if (assignPlugin(implementation, &cSpindizzyTextureSetController, *socket, false)) {
      if (mPreviousController != NULL) {
        cSpindizzyTextureSetController->removeControlObject(this);
      }
      if (cSpindizzyTextureSetController != NULL) {
        cSpindizzyTextureSetController->addControlObject(this);
      }
    }
  } else if (socket->getType() == "SurfaceProcessor") {
    ISurfaceProcessor* mPreviousSurfaceProcessor = cSurfaceProcessor;
    if (assignPlugin(implementation, &cSurfaceProcessor, *socket)) {
      mPreviousSurfaceProcessor->reinitialise();
    }
  } else if (socket->getType() == "ZoneContext") {
    IZoneContext* mPreviousZoneContext = cZoneContext;
    if (assignPlugin(implementation, &cZoneContext, *socket)) {
      mPreviousZoneContext->removeZoneContextListener(this);
      cZoneContext->addZoneContextListener(this);
    }
  } else {
    std::cout << "WARNING!  I don't know what to do with the implementation I was given!" << std::endl;
  }
}

IPlugin* SpindizzyBlockSet::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    if (mIndex < cClueModelFactories.size()) {
      return cClueModelFactories[mIndex]; 
    }
  }
  if (socket->getType() == "Camera")                     {return cCamera;}
  if (socket->getType() == "CollidableSurfaceRegistry")  {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "HUD")                        {return cHUD;}
  if (socket->getType() == "SpindizzyTextureSet")        {return cSpindizzyTextureSet;}
  if (socket->getType() == "SurfaceProcessor")           {return cSurfaceProcessor;}
  if (socket->getType() == "SpindizzyTextureSetChanger") {return cSpindizzyTextureSetController;}
  if (socket->getType() == "ZoneContext")                {return cZoneContext;}
  // TODO: Throw wobbly!
  return NULL;
}

std::vector<IElementFactory*> SpindizzyBlockSet::getElementFactories() {
  return cElementFactories;
}

SpindizzyBlockHandler* SpindizzyBlockSet::createHandler(IElementContainer* elementContainer) {
  SpindizzyBlockHandler* mHandler = new SpindizzyBlockHandler();
  cElementHandlers[elementContainer] = mHandler;
  return mHandler;
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
  // TODO: Save states and block type configurations
}

ISpindizzyBlockFactory* SpindizzyBlockSet::getFactory(const std::string& name) {
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    if (cElementFactories[i]->getName() == name) {
      // TODO: Should not use static cast!
      return static_cast<ISpindizzyBlockFactory*>(cElementFactories[i]);
    }
  }
  std::cout << "WARNING: Requested block type doesn't exist!" << std::endl;
  return NULL;
}

void SpindizzyBlockSet::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "State") {
      std::string mStateName = mNode->getStringValue();
      int mClue = mNode->getIntegerAttribute("hudModel");
      ISimpleModel* mClueModel = cClueModels[mClue];
      addBlockState(mStateName, mClueModel);
    } else if (mValueAsString == "BlockType") {
      std::string mBlockTypeName = mNode->getAttribute("name");
      // TODO: Pass the textures into the factory
      ISpindizzyBlockFactory* mFactory = new SpindizzyBlockFactory(mBlockTypeName, &cSpindizzyTextureSet, this, mNode, cCommandRegistry);
      cElementFactories.push_back(mFactory);
    }
  }
}

std::string SpindizzyBlockSet::getHUDComponentFactoryName() {
  return "SpindizzyBlocks";
}

IHUDGameComponent* SpindizzyBlockSet::getHUDComponent(const std::string& component) {
  return component == "Clue" ? cHUDClue : NULL;
}
  
void SpindizzyBlockSet::zoneContextChanged(IZone* zone) {
  SpindizzyBlockHandler* mElementHandler = cElementHandlers[dynamic_cast<IElementContainer*>(zone)];
  if (mElementHandler != NULL) {
    std::set<bool*> mInputs = mElementHandler->getInputs();
    for (std::set<bool*>::iterator i = mInputs.begin(); i != mInputs.end(); i++) {
      if (!(**i)) {
        for (unsigned int j = 0; j < cBlockStates.size(); j++) {
          if (cBlockStates[j]->getInputAddress() == (*i)) {
            ISimpleModel* mClueModel = cBlockStateClueModels[j];
            cHUDClue->setModel(mClueModel);
            return;
          }
        }
      }
    }
  }
  cHUDClue->setModel(NULL);
/*  unsigned int mClue = rand() % (cBlockStateClueModels.size() * 2);
  ISimpleModel* mClueModel = mClue >= cBlockStateClueModels.size() ? NULL : cBlockStateClueModels[mClue];
  cHUDClue->setModel(mClueModel);*/
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

std::vector<IWallSurfaceTemplate*> SpindizzyBlockSet::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing) {
  return cSurfaceProcessor->getWallSurfaces(provider, facing);
}

void SpindizzyBlockSet::registerRollableSurface(IRollableSurface* rollableSurface) {
  cCollidableSurfaceRegistry->registerRollableSurface(rollableSurface, false);
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

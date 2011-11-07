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
  cElementFactories.push_back(new SpindizzyWaterFactory(&cTextureSet, this));
  assignDummyPlugin(&cDummyTextureSet, "TextureSet");
  cTextureSet = cDummyTextureSet;
  cTextureSetController = NULL;
  assignDummyPlugin(&cCamera, "Camera");
  assignDummyPlugin(&cHUD, "HUD");
  assignDummyPlugin(&cPhysicalProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cVisualProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cHUDClue = new HUDClue(cCamera);
  cEditing = false;
}

void SpindizzyBlockSet::addBlockState(const std::string& name, ISimpleModel* model) {
  bool* mState = new bool(false);
  cBlockStates.push_back(new ConditionElement(name, mState));
  cBlockStateClueModels.push_back(model);
  IUserCommand* mStateOnCommand = new BlockStateCommand(this, name, mState, true);
  IUserCommand* mStateOffCommand = new BlockStateCommand(this, name, mState, false);
  cSpindizzyBlockCommands.push_back(mStateOnCommand);
  cSpindizzyBlockCommands.push_back(mStateOffCommand);
  cCommandRegistry->registerCommand(mStateOnCommand);
  cCommandRegistry->registerCommand(mStateOffCommand);
}

std::vector<PlugSocket*> SpindizzyBlockSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  for (unsigned int i = 0; i <= cHUDClueData.size(); i++) {
    std::ostringstream mSocketID;
    mSocketID << i;
    mSockets.push_back(new PlugSocket("3DModel", mSocketID.str()));
  }
  if (cTextureSetController == NULL) {
    if (cTextureSet == cDummyTextureSet) {
      mSockets.push_back(new PlugSocket("TextureSetChanger"));
    }
    mSockets.push_back(new PlugSocket("TextureSet"));
  } else {
    mSockets.push_back(new PlugSocket("TextureSetChanger"));
  }
  mSockets.push_back(new PlugSocket("Camera"));
  mSockets.push_back(new PlugSocket("HUD"));
  mSockets.push_back(new PlugSocket("SurfaceProcessor", "Visual"));
  mSockets.push_back(new PlugSocket("SurfaceProcessor", "Physical"));
  
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  mSockets.push_back(new PlugSocket("ZoneContext"));
  return mSockets;
}

void SpindizzyBlockSet::setTextureSet(ITextureSet* textureSet) {
  cTextureSet = textureSet != NULL ? textureSet : cDummyTextureSet;
}

//      ISimpleModel* mNewModel = ->createModel(&cClueModelLocation);
void SpindizzyBlockSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "3DModel") {
    std::string mSocketID = socket->getID();
    std::stringstream mInputString(mSocketID);
    unsigned int mIndex;
    mInputString >> mIndex;
    ISimpleModelFactory* mNewModelFactory = NULL;
    if (implementation != NULL) {
      assignPlugin(implementation, &mNewModelFactory, *socket);
      HUDClueData* mNewHUDClueData = new HUDClueData(mNewModelFactory);
      if (mIndex == cHUDClueData.size()) {
        cHUDClueData.push_back(mNewHUDClueData);
      } else {
        // TODO: Is this correct?   delete cHUDClueData[mIndex];
        cHUDClueData[mIndex] = mNewHUDClueData;
      }
    } else { // TODO: Range check?
      // TODO: Is this correct?   delete cHUDClueData[mIndex];
      cHUDClueData.erase(cHUDClueData.begin() + mIndex);
    }
  } else if (socket->getType() == "Camera") {
    assignPlugin(implementation, &cCamera, *socket);
    cHUDClue->setCamera(cCamera);
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else if (socket->getType() == "TextureSet") {
    if (assignPlugin(implementation, &cTextureSet, *socket)) {
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
  } else if (socket->getType() == "TextureSetChanger") {
    ITextureSetChanger* mPreviousController = cTextureSetController;
    if (assignPlugin(implementation, &cTextureSetController, *socket, false)) {
      if (mPreviousController != NULL) {
        cTextureSetController->removeControlObject(this);
      }
      if (cTextureSetController != NULL) {
        cTextureSetController->addControlObject(this);
      }
    }
  } else if (socket->getType() == "SurfaceProcessor") {
    if (socket->getID() == "Visual") {
      ISurfaceProcessor* mPreviousSurfaceProcessor = cVisualProcessor;
      if (assignPlugin(implementation, &cVisualProcessor, *socket)) {
        mPreviousSurfaceProcessor->reinitialise();
      }
    } else {
      ISurfaceProcessor* mPreviousSurfaceProcessor = cPhysicalProcessor;
      if (assignPlugin(implementation, &cPhysicalProcessor, *socket)) {
        if (!cEditing) {
          mPreviousSurfaceProcessor->reinitialise();
        }
      }
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
    if (mIndex < cHUDClueData.size()) {
      return cHUDClueData[mIndex]->getFactory(); 
    }
  }
  if (socket->getType() == "Camera")                    {return cCamera;}
  if (socket->getType() == "CollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "HUD")                       {return cHUD;}
  if (socket->getType() == "SurfaceProcessor")          {return socket->getID() == "Visual" ? cVisualProcessor : cPhysicalProcessor;}
  if (socket->getType() == "TextureSet")                {return cTextureSet;}
  if (socket->getType() == "TextureSetChanger")         {return cTextureSetController;}
  if (socket->getType() == "ZoneContext")               {return cZoneContext;}
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

void SpindizzyBlockSet::setRuntimeContext(IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cCommandRegistry = runtimeContext->getCommandRegistry();
  for (unsigned int i = 0; i < cSpindizzyBlockCommands.size(); i++) {
    cCommandRegistry->registerCommand(cSpindizzyBlockCommands[i]);
  }
}

void SpindizzyBlockSet::destroy(IElement* element) {
  delete element;
}

void SpindizzyBlockSet::setEditingContext(IEditingContext* editingContext) {
  std::vector<std::string> mPath;
  mPath.push_back("Configure Spindizzy Blocks...");
  mPath.push_back("Elements");
  cComponentContainer = editingContext->getComponentContainer();
  OpenDialogCommand* mConfigureCommand = new OpenDialogCommand(cComponentContainer, this);
  editingContext->registerCommand(new DefaultCommandInfo(mPath, mConfigureCommand));
}

void SpindizzyBlockSet::initElementsComplete() {
  cVisualProcessor->initElementsComplete();
  if (!cEditing) {
    cPhysicalProcessor->initElementsComplete();
  }
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
      ISimpleModel* mClueModel = cHUDClueData[mClue]->initClueData(mNode, mStateName);
      addBlockState(mStateName, mClueModel);
    } else if (mValueAsString == "BlockType") {
      std::string mBlockTypeName = mNode->getAttribute("name");
      // TODO: Pass the textures into the factory
      ISpindizzyBlockFactory* mFactory = new SpindizzyBlockFactory(mBlockTypeName, &cTextureSet, this, mNode, cCommandRegistry);
      cElementFactories.push_back(mFactory);
    }
  }
}

void SpindizzyBlockSet::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cHUDClueData.size(); i++) {
    DOMNodeWriter* mStateNode = node->addBranch("State");
    cHUDClueData[i]->save(mStateNode);
    mStateNode->addAttribute("hudModel", i);
  }
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    static_cast<ISpindizzyBlockFactory*>(cElementFactories[i])->save(node);
  }
}

IHUDGameComponent* SpindizzyBlockSet::getHUDComponent(const std::string& component) {
  return component == "Clue" ? cHUDClue : NULL;
}
  
void SpindizzyBlockSet::zoneContextChanged(IZone* zone) {
  updateClue();
}

void SpindizzyBlockSet::registerSurfaceProvider(ISurfaceProvider* provider) {
  cVisualProcessor->registerSurfaceProvider(provider);
  if (!cEditing) {
    cPhysicalProcessor->registerSurfaceProvider(provider);
  }
}

void SpindizzyBlockSet::unregisterSurfaceProvider(ISurfaceProvider* provider) {
  cVisualProcessor->unregisterSurfaceProvider(provider);
  if (!cEditing) {
    cPhysicalProcessor->unregisterSurfaceProvider(provider);
  }
}

void SpindizzyBlockSet::setDirty() {
  cVisualProcessor->setDirty();
  if (!cEditing) { // TODO: Is this necessary?  Does this function even get called during runtime?
    cPhysicalProcessor->setDirty();
  }
}

std::vector<ITileSurfaceTemplate*> SpindizzyBlockSet::getTileSurfaces(ISurfaceProvider* provider, ITileSurface::FaceDirection facing, bool visual) {
  return (visual ? cVisualProcessor : cPhysicalProcessor)->getTileSurfaces(provider, facing);
}

std::vector<IWallSurfaceTemplate*> SpindizzyBlockSet::getWallSurfaces(ISurfaceProvider* provider, IWallSurface::FaceDirection facing, bool visual) {
  return (visual ? cVisualProcessor : cPhysicalProcessor)->getWallSurfaces(provider, facing);
}

void SpindizzyBlockSet::destroyWallTemplate(IWallSurfaceTemplate* wallTemplate, bool visual) {
  (visual ? cVisualProcessor : cPhysicalProcessor)->destroyWallTemplate(wallTemplate);
}

void SpindizzyBlockSet::destroyTileTemplate(ITileSurfaceTemplate* tileTemplate, bool visual) {
  (visual ? cVisualProcessor : cPhysicalProcessor)->destroyTileTemplate(tileTemplate);
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

void SpindizzyBlockSet::updateClue() {
  SpindizzyBlockHandler* mElementHandler = cElementHandlers[dynamic_cast<IElementContainer*>(cZoneContext->getZoneContext())];
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
}

bool SpindizzyBlockSet::isEditing() {
  return cEditing;
}

SpindizzyBlockSet::~SpindizzyBlockSet() {
  cVisualProcessor->reinitialise();
  if (!cEditing) {
    cPhysicalProcessor->reinitialise();
  }
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

SpindizzyBlockSet::HUDClueData::HUDClueData(ISimpleModelFactory* factory) {
  cFactory = factory;
  cModel = cFactory->createModel(&cLocation);
}
    
ISimpleModel* SpindizzyBlockSet::HUDClueData::initClueData(DOMNodeWrapper* node, const std::string& name) {
  cName = name;
  cLocation.x = node->getFloatAttribute("x");
  cLocation.y = node->getFloatAttribute("y");
  cLocation.z = node->getFloatAttribute("z");
  return cModel;
}
    
ISimpleModelFactory* SpindizzyBlockSet::HUDClueData::getFactory() {
  return cFactory;
}

void SpindizzyBlockSet::HUDClueData::save(DOMNodeWriter* node) {
  if (cLocation.x != 0.0f) {
    node->addAttribute("x", cLocation.x);
  }
  if (cLocation.y != 0.0f) {
    node->addAttribute("y", cLocation.y);
  }
  if (cLocation.z != 0.0f) {
    node->addAttribute("z", cLocation.z);
  }
  node->addText(cName);
}

IHUDComponent* SpindizzyBlockSet::createComponent() {
  return new SpindizzyBlockConfigurationDialog(cComponentContainer);
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new SpindizzyBlockSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}

/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either VERSION 3 of the License, or
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

SpindizzyBlockSet::SpindizzyBlockSet(IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  assignDummyPlugin(&cCamera, "Camera");
  assignDummyPlugin(&cHUD, "HUD");
  assignDummyPlugin(&cPhysicalProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cVisualProcessor, "SurfaceProcessor");
  assignDummyPlugin(&cCollidableSurfaceRegistry, "CollidableSurfaceRegistry");
  assignDummyPlugin(&cZoneContext, "ZoneContext");
  cHUDClue = new HUDClue(cCamera);
}

std::vector<PlugSocket*> SpindizzyBlockSet::getPlugSockets() {
  std::vector<PlugSocket*> mSockets;
  mSockets.push_back(new PlugSocket("Camera"));
  mSockets.push_back(new PlugSocket("HUD"));
  mSockets.push_back(new PlugSocket("SurfaceProcessor", "Visual"));
  mSockets.push_back(new PlugSocket("SurfaceProcessor", "Physical"));
  
  mSockets.push_back(new PlugSocket("CollidableSurfaceRegistry"));
  mSockets.push_back(new PlugSocket("ZoneContext"));
  return mSockets;
}

//      I3DModel* mNewModel = ->createModel(&cClueModelLocation);
void SpindizzyBlockSet::setPlugin(PlugSocket* socket, IPlugin* implementation) {
  if (socket->getType() == "Camera") {
    assignPlugin(implementation, &cCamera, *socket);
    cHUDClue->setCamera(cCamera);
  } else if (socket->getType() == "CollidableSurfaceRegistry") {
    assignPlugin(implementation, &cCollidableSurfaceRegistry, *socket);
  } else if (socket->getType() == "HUD") {
    IHUD* mPreviousHUD = cHUD;
    if (assignPlugin(implementation, &cHUD, *socket)) {
      mPreviousHUD->unregisterHUDComponentFactory(this);
      cHUD->registerHUDComponentFactory(this);
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
        if (!cRuntimeContext->isEditing()) {
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
  if (socket->getType() == "Camera")                    {return cCamera;}
  if (socket->getType() == "CollidableSurfaceRegistry") {return cCollidableSurfaceRegistry;}
  if (socket->getType() == "HUD")                       {return cHUD;}
  if (socket->getType() == "SurfaceProcessor")          {return socket->getID() == "Visual" ? cVisualProcessor : cPhysicalProcessor;}
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
  if (!cRuntimeContext->isEditing()) {
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
      cBlockStateData.push_back(new BlockStateData(mNode, cRuntimeContext, this));
    } else if (mValueAsString == "BlockType") {
      std::string mBlockTypeName = mNode->getAttribute("name");
      // TODO: Pass the textures into the factory
      ISpindizzyBlockFactory* mFactory = new SpindizzyBlockFactory(mBlockTypeName, this, mNode, cRuntimeContext);
      cElementFactories.push_back(mFactory);
    } else if (mValueAsString == "WaterType") {
      ITexture* mWaterTexture = cRuntimeContext->getTexture(mNode);
      ISpindizzyBlockFactory* mFactory = new SpindizzyWaterFactory(this, mWaterTexture);
      cElementFactories.push_back(mFactory);
    }
  }
}

void SpindizzyBlockSet::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cBlockStateData.size(); i++) {
    cBlockStateData[i]->save(node);
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
  if (!cRuntimeContext->isEditing()) {
    cPhysicalProcessor->registerSurfaceProvider(provider);
  }
}

void SpindizzyBlockSet::unregisterSurfaceProvider(ISurfaceProvider* provider) {
  cVisualProcessor->unregisterSurfaceProvider(provider);
  if (!cRuntimeContext->isEditing()) {
    cPhysicalProcessor->unregisterSurfaceProvider(provider);
  }
}

void SpindizzyBlockSet::setDirty() {
  cVisualProcessor->setDirty();
  if (!cRuntimeContext->isEditing()) { // TODO: Is this necessary?  Does this function even get called during runtime?
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

std::vector<ConditionElement*> SpindizzyBlockSet::getConditionElements() {
  std::vector<ConditionElement*> mConditionElements;
  for (unsigned int i = 0; i < cBlockStateData.size(); i++) {
    mConditionElements.push_back(cBlockStateData[i]->getConditionElement());
  }
  return mConditionElements;
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

void SpindizzyBlockSet::updateClue() {
  SpindizzyBlockHandler* mElementHandler = cElementHandlers[dynamic_cast<IElementContainer*>(cZoneContext->getZoneContext())];
  if (mElementHandler != NULL) {
    std::set<bool*> mInputs = mElementHandler->getInputs();
    for (std::set<bool*>::iterator i = mInputs.begin(); i != mInputs.end(); i++) {
      if (!(**i)) {
        for (unsigned int j = 0; j < cBlockStateData.size(); j++) {
          if (cBlockStateData[j]->getInputAddress() == (*i)) {
            I3DModel* mClueModel = cBlockStateData[j]->getModel();
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
  return cRuntimeContext->isEditing();
}

SpindizzyBlockSet::~SpindizzyBlockSet() {
  cVisualProcessor->reinitialise();
  if (!cRuntimeContext->isEditing()) {
    cPhysicalProcessor->reinitialise();
  }
  for (unsigned int i = 0; i < cElementFactories.size(); i++) {
    delete cElementFactories[i];
  }
}

SpindizzyBlockSet::BlockStateData::BlockStateData(DOMNodeWrapper* node, IRuntimeContext* runtimeContext, SpindizzyBlockSet* parent) {
  cName = node->getStringValue();
  cClueModelLocation.x = node->getFloatAttribute("x");
  cClueModelLocation.y = node->getFloatAttribute("y");
  cClueModelLocation.z = node->getFloatAttribute("z");
  cClueModel = runtimeContext->getModel(node, &cClueModelLocation);
  bool* mState = new bool(false);
  cState = new ConditionElement(cName, mState);
// TODO  runtimeContext->add(cState);
  runtimeContext->add(new BlockStateCommand(parent, mState, true), "Set Block Flag " + cName);
  runtimeContext->add(new BlockStateCommand(parent, mState, false), "Unset Block Flag " + cName);
}

bool* SpindizzyBlockSet::BlockStateData::getInputAddress() {
  return cState->getInputAddress();
}

I3DModel* SpindizzyBlockSet::BlockStateData::getModel() {
  return cClueModel;
}

ConditionElement* SpindizzyBlockSet::BlockStateData::getConditionElement() {
  return cState;
}

void SpindizzyBlockSet::BlockStateData::save(DOMNodeWriter* node) {
  DOMNodeWriter* mStateNode = node->addBranch("State");
// TODO: Implement this
//  cClueModel->save(mNode)
  if (cClueModelLocation.x != 0.0f) {
    mStateNode->addAttribute("x", cClueModelLocation.x);
  }
  if (cClueModelLocation.y != 0.0f) {
    mStateNode->addAttribute("y", cClueModelLocation.y);
  }
  if (cClueModelLocation.z != 0.0f) {
    mStateNode->addAttribute("z", cClueModelLocation.z);
  }
  mStateNode->addText(cName);
}

IHUDComponent* SpindizzyBlockSet::createComponent() {
  return new SpindizzyBlockConfigurationDialog(cComponentContainer);
}

extern "C" IElementSet* create(IRuntimeContext* runtimeContext) {
  return new SpindizzyBlockSet(runtimeContext);
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}

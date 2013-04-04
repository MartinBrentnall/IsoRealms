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

SpindizzyBlockHandler* SpindizzyBlockSet::createHandler() {
   SpindizzyBlockHandler* mHandler = new SpindizzyBlockHandler();
//   cElementHandlers[elementContainer] = mHandler;
   return mHandler;
}

void SpindizzyBlockSet::initElementsComplete() {
  cVisualProcessor->initElementsComplete();
  if (!cEditing) {
    cPhysicalProcessor->initElementsComplete();
  }
}

void SpindizzyBlockSet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cEditing = runtimeContext->isEditing();
  cHUDClue = new HUDClue(cCamera);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "BlockType") {
      std::string mBlockTypeName = mNode->getAttribute("name");
      SpindizzyBlockType* mType = new SpindizzyBlockType(this);
      cElementTypes.push_back(mType);
      runtimeContext->add(mType, mBlockTypeName, mNode);
      runtimeContext->addListener(mType);
    } else if (mValueAsString == "WaterType") {
      std::string mWaterTypeName = mNode->getAttribute("name");
      SpindizzyWaterType* mType = new SpindizzyWaterType(this);
      cWaterTypes.push_back(mType);
      runtimeContext->add(mType, mWaterTypeName, mNode); // TODO: Name shouldn't be hard coded
      runtimeContext->addListener(mType);
    } else if (mValueAsString == "State") {
      SpindizzyBlockState* mBlockState = new SpindizzyBlockState();
      std::string mName = mNode->getAttribute("name");
      cBlockStateData.push_back(mBlockState);
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<SpindizzyBlockState>(mBlockState);
      runtimeContext->add(mBlockState, mNode);
      runtimeContext->registerArgument("BlockState", mName, mArgumentSource);
    } else if (mValueAsString == "Configuration") {
      runtimeContext->add(this, mNode);
    }
  }
}

void SpindizzyBlockSet::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  cEditingContext = editingContext;
}

void SpindizzyBlockSet::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cVisualProcessor = resources->getSurfaceProcessor(node->getAttribute("visualProcessor"));
  cPhysicalProcessor = resources->getSurfaceProcessor(node->getAttribute("physicalProcessor"));
  cCollidableSurfaceRegistry = resources->getSurfaceRegistry(node->getAttribute("surfaceRegistry"));
}

void SpindizzyBlockSet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  DOMNodeWriter* mConfigurationNode = node->addBranch("Configuration");
  mConfigurationNode->addAttribute("visualProcessor", resourceLocator->getPath(cVisualProcessor));
  mConfigurationNode->addAttribute("physicalProcessor", resourceLocator->getPath(cPhysicalProcessor));
  mConfigurationNode->addAttribute("surfaceRegistry", resourceLocator->getPath(cCollidableSurfaceRegistry));
  for (unsigned int i = 0; i < cBlockStateData.size(); i++) {
    DOMNodeWriter* mStateNode = node->addBranch("State");
    cBlockStateData[i]->save(mStateNode, resourceLocator);
  }
  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("BlockType");
    std::string mResourceName = resourceLocator->getPath(cElementTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cElementTypes[i]->save(mCameraNode, resourceLocator);
  }
  for (unsigned int i = 0; i < cWaterTypes.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("WaterType");
    std::string mResourceName = resourceLocator->getPath(cWaterTypes[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cWaterTypes[i]->save(mCameraNode, resourceLocator);
  }
}

IHUDGameComponent* SpindizzyBlockSet::getHUDComponent() {
  return cHUDClue;
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

// TODO: call this function when the zone changes!
void SpindizzyBlockSet::updateClue() {
//   SpindizzyBlockHandler* mElementHandler = cElementHandlers[dynamic_cast<IElementContainer*>(cZoneContext->getZoneContext())];
//   if (mElementHandler != NULL) {
//     std::set<bool*> mInputs = mElementHandler->getInputs();
//     for (std::set<bool*>::iterator i = mInputs.begin(); i != mInputs.end(); i++) {
//       if (!(**i)) {
//         for (unsigned int j = 0; j < cBlockStateData.size(); j++) {
//           if (cBlockStateData[j]->getInputAddress() == (*i)) {
//             I3DModel* mClueModel = cBlockStateData[j]->getModel();
//             cHUDClue->setModel(mClueModel);
//             return;
//           }
//         }
//       }
//     }
//   }
//   cHUDClue->setModel(NULL);
}

bool SpindizzyBlockSet::isEditing() {
  return cEditing;
}

void SpindizzyBlockSet::staticChanged() {
  cEditingContext->staticChanged();
}

SpindizzyBlockSet::~SpindizzyBlockSet() {
  cVisualProcessor->reinitialise();
  if (!cEditing) {
    cPhysicalProcessor->reinitialise();
  }
/*TODO  for (unsigned int i = 0; i < cElementTypes.size(); i++) {
    delete cElementTypes[i];
  }*/
}

extern "C" IPlugin* create() {
  return new SpindizzyBlockSet();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}

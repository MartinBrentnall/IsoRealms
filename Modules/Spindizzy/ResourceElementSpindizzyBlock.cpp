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
#include "ResourceElementSpindizzyBlock.h"

ResourceElementSpindizzyBlock::ResourceElementSpindizzyBlock(ISpindizzyBlockSet* elementSet, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
  cModuleInterface = elementSet;
  cConfigurationComponent = nullptr;
  cBlockProperties = new SpindizzyBlockProperties();
  cSampleBlock = nullptr;
  cStartLocation = nullptr;
}

void ResourceElementSpindizzyBlock::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resourceAccessor) {
  cBlockTypeProperties.configure(node, resourceAccessor);
}

Vertex* ResourceElementSpindizzyBlock::editorCursorStopped(Vertex* location) {
  Vertex* mGridLocation = new Vertex();
  mGridLocation->x = std::round(location->x);
  mGridLocation->y = std::round(location->y);
  mGridLocation->z = std::round(location->z * 2.0) * 0.5;
  return mGridLocation;
}

IElement* ResourceElementSpindizzyBlock::getElement() {
  return nullptr;
}

void ResourceElementSpindizzyBlock::loadElement(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* container, IResourceAccessor* resources) {
  BlockLocation mStartLocation;
  BlockLocation mEndLocation;
  // TODO: Should throw something if these are not specified!
  cBlockProperties->reset();
  mStartLocation.setRelative(node, *zoneLocation);
  mEndLocation.setRelative(node, mStartLocation, "width", "length", "height");
  std::vector<ConditionElement*> mElements = cModuleInterface->getConditionElements();
  cBlockProperties->setup(node, mElements);
  bool mAddition = mStartLocation.z <= mEndLocation.z;
  (mAddition ? mStartLocation.z : mEndLocation.z)++;
  mEndLocation.x--;
  mEndLocation.y--;
  bool mIndependent = node->getBooleanAttribute("independent");
  ElementHandlerSpindizzyBlock* mHandler = cModuleInterface->getElementHandlerSpindizzyBlock(container);  
  ElementSpindizzyBlock* mLoadedBlock = createBlock(&mStartLocation, &mEndLocation, cBlockProperties, mAddition, mHandler);
  cContent.push_back(mLoadedBlock);
  if (mIndependent) {
    mLoadedBlock->createSampleSurfaces();
  } else {
    cModuleInterface->registerSurfaceProvider(mLoadedBlock, false);
    cModuleInterface->setDirty();
  }
  mHandler->addElement(mLoadedBlock);
}

// void ResourceElementSpindizzyBlock::unregisterSurfaces(ISurfaceProcessor* surfaceProcessor) {
//   for (unsigned int i = 0; i < cContent.size(); i++) {
//     surfaceProcessor->unregisterSurfaceProvider(cContent[i]);
//   }
// }

ISpindizzyBlockSet* ResourceElementSpindizzyBlock::getSpindizzyBlockInterface() {
  return cModuleInterface;
}

BlockTypeProperties* ResourceElementSpindizzyBlock::getBlockTypeProperties() {
  return &cBlockTypeProperties;
}

void ResourceElementSpindizzyBlock::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  cBlockTypeProperties.save(node, resourceLocator);
}

void ResourceElementSpindizzyBlock::setEditingContext(BlockLocation* editingLocation, IComponentContainer* componentContainer) {
  cComponentContainer = componentContainer;
}

unsigned int ResourceElementSpindizzyBlock::getXCorner(float angle) {
  return 0;
}

bool ResourceElementSpindizzyBlock::keyDown(SDLKey& key, ILayerEditingContext* editingContext) {
  switch (key) {
    case SDLK_KP_DIVIDE:   cBlockProperties->raiseCorner(0, 1);     return true;
    case SDLK_KP8:         cBlockProperties->lowerCorner(0, 1);     return true;
    case SDLK_KP9:         cBlockProperties->raiseCorner(1, 1);     return true;
    case SDLK_KP6:         cBlockProperties->lowerCorner(1, 1);     return true;
    case SDLK_KP5:         cBlockProperties->raiseCorner(1, 0);     return true;
    case SDLK_KP2:         cBlockProperties->lowerCorner(1, 0);     return true;
    case SDLK_KP7:         cBlockProperties->raiseCorner(0, 0);     return true;
    case SDLK_KP4:         cBlockProperties->lowerCorner(0, 0);     return true;
    case SDLK_KP_MULTIPLY: cBlockProperties->toggleSplit();         return true;
    case SDLK_KP_PERIOD:   cBlockProperties->toggleSteppedBottom(); return true;
    case SDLK_SPACE: {
      IElementContainer* mContainer = editingContext->getElementContainer();
      ElementHandlerSpindizzyBlock* mHandler = cModuleInterface->getElementHandlerSpindizzyBlock(mContainer);
      if (cStartLocation == nullptr) {
        editingContext->setCursorRestriction(mContainer);
        Vertex* mLocation = editingContext->getLocation();
        BlockLocation mStartLocation(mLocation->x, mLocation->y, mLocation->z * 2.0);
        if (cEditingBlock != nullptr) {
          delete cEditingBlock;
        }
        cEditingBlock = createBlock(&mStartLocation, &mStartLocation, cBlockProperties, true, mHandler);
        cStartLocation = new Vertex(*mLocation);
      } else {
        editingContext->removeCursorRestriction(mContainer);
        mHandler->addElement(cEditingBlock);
        cContent.push_back(cEditingBlock);
        cModuleInterface->registerSurfaceProvider(cEditingBlock, true);
        cStartLocation = nullptr;
        BlockLocation mIdentityBlockLocation(0, 0, 0);
        cEditingBlock = createBlock(&mIdentityBlockLocation, &mIdentityBlockLocation, cBlockProperties, true, nullptr);
        cEditingBlock->createSampleSurfaces();
        editingContext->staticChanged();
      }
    }
//     case SDLK_DELETE: {
//       if (cStartBlockLocation == nullptr) {
//         cStartBlockLocation = new BlockLocation(*cEditingLocation);
//       } else {
//         ElementSpindizzyBlock* mNewBlock = createBlock(cStartBlockLocation, cEditingLocation, cBlockProperties, false, nullptr);
//         cContent.push_back(mNewBlock);
// //        addElement(mNewBlock);
//         delete cStartBlockLocation;
//         cStartBlockLocation = nullptr;
//       }
//     }

    default: {
      return false;
    }
  }
  return false;
}

void ResourceElementSpindizzyBlock::configureElement() {
  if (cConfigurationComponent == nullptr) {
    cConfigurationComponent = new SpindizzyBlockConfigurationComponent(cComponentContainer, this, cBlockProperties, this);
    cComponentContainer->addComponent(cConfigurationComponent);
  }
}

bool ResourceElementSpindizzyBlock::inputEdit(SDL_Event& event, ILayerEditingContext* editingContext) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      if (keyDown(event.key.keysym.sym, editingContext)) {
        delete cEditingBlock;
        cEditingBlock = nullptr;
        return true;
      }
    }
  }
  return false;
}

void ResourceElementSpindizzyBlock::renderEditingPreview(Vertex& location) {
  glEnable(GL_BLEND);
  if (cEditingBlock == nullptr) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cEditingBlock = createBlock(&mIdentityBlockLocation, &mIdentityBlockLocation, cBlockProperties, true, nullptr);
    cEditingBlock->createSampleSurfaces();
    cStartLocation = nullptr;
  }
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  if (cStartLocation != nullptr) {
    cEditingBlock->renderPreview(*cStartLocation, location);
  } else {
    glTranslatef(location.x, location.y, location.z);
    cEditingBlock->renderRuntime();
  }
  glDisable(GL_BLEND);
}

void ResourceElementSpindizzyBlock::renderIcon() {
  glTranslatef(0.0f, 0.3f, 0.0f);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  // TODO: Scale the icon
  glScalef(1.3f, 1.3f, 1.3f);
  glColor3f(1.0f, 1.0f, 1.0f);
  if (cSampleBlock == nullptr) {
    BlockLocation mIdentityBlockLocation(0, 0, 0);
    cSampleBlock = createBlock(&mIdentityBlockLocation, &mIdentityBlockLocation, cBlockProperties, true, nullptr);
    cSampleBlock->createSampleSurfaces();
  }
  cSampleBlock->renderRuntime();
}

void ResourceElementSpindizzyBlock::updateIcon(unsigned int milliseconds) {
  // Nothing to do.
}

void ResourceElementSpindizzyBlock::componentClosed(IHUDComponent* component) {
  if (component == cConfigurationComponent) {
    cConfigurationComponent = nullptr;
  }
}

void ResourceElementSpindizzyBlock::destroy(IElement* element) {
  delete element;
}

ElementSpindizzyBlock* ResourceElementSpindizzyBlock::createBlock(BlockLocation* startLocation, BlockLocation* endLocation, SpindizzyBlockProperties* blockProperties, bool addition, ElementHandlerSpindizzyBlock* handler) {
  return new ElementSpindizzyBlock(this, startLocation, endLocation, blockProperties, addition, handler);
}

void ResourceElementSpindizzyBlock::resourcePendingDestruction(ITexture* destroyee, ITexture* replacement) {
  if (cBlockTypeProperties.resourcePendingDestruction(destroyee, replacement)) {
    cModuleInterface->staticChanged();
  }
}

void ResourceElementSpindizzyBlock::resourceChanged(ITexture* texture) {
  if (cBlockTypeProperties.contains(texture)) {
    cModuleInterface->staticChanged();
  }
}

ResourceElementSpindizzyBlock::~ResourceElementSpindizzyBlock() {
  delete cSampleBlock;
  delete cBlockProperties;
  if (cConfigurationComponent != nullptr) {
    cComponentContainer->removeComponent(cConfigurationComponent);
    delete cConfigurationComponent;
  }
  for (unsigned int i = 0; i < cContent.size(); i++) {
    IElementContainer* mContainer = cContent[i]->getElementContainer();
    ElementHandlerSpindizzyBlock* mHandler = cModuleInterface->getElementHandlerSpindizzyBlock(mContainer);
    mHandler->removeElement(cContent[i]);
    if (mHandler->isEmpty()) {
      mContainer->removeElement(mHandler);
      cModuleInterface->removeElementHandlerSpindizzyBlock(mContainer);
    }
    cModuleInterface->unregisterSurfaceProvider(cContent[i]);
    delete cContent[i];
  }
}


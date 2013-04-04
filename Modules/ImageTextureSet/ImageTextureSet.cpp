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
#include "ImageTextureSet.h"

ImageTextureSet::ImageTextureSet() {
  // TODO: Should only init once!
  int mFlags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  int mInit = IMG_Init(mFlags);
  if ((mInit & mFlags) != mFlags) {
    std::cout << "Failed to initialise SDL_Image" << std::endl;
    // TODO: Throw here
  }
}

void ImageTextureSet::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Texture") {
      std::string mID = mNode->getAttribute("id");
      ImageTexture* mTexture = new ImageTexture();
      cTextures.push_back(mTexture);
      runtimeContext->add(mTexture, mID, mNode);
    } else {
      // TODO: Throw here
    }
  }
}

void ImageTextureSet::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cTextures.size(); i++) {
    DOMNodeWriter* mResourceNode = node->addBranch("Texture");
    std::string mResourceName = resourceLocator->getPath(cTextures[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mResourceNode->addAttribute("id", mResourceName);
    cTextures[i]->save(mResourceNode);
  }
}

void ImageTextureSet::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "From File...");
  cComponentContainer = editingContext->getComponentContainer();
}

void ImageTextureSet::createResource(IResourceAccessor* resources) {
  DialogImageTextureSelector* mDialog = new DialogImageTextureSelector(cComponentContainer, resources, this);
  cCreateDialogs.push_back(mDialog);
  cComponentContainer->addComponent(mDialog);
  // TODO: Implement this
}

void ImageTextureSet::editResource(ITexture* texture, IResourceAccessor* resources) {
  // TODO: Implement this
}

void ImageTextureSet::removeResource(ITexture* texture, IResourceAccessor* resources, IResourceInstanceListener<ITexture>* listener) {
  for (unsigned int i = 0; i < cTextures.size(); i++) {
    if (cTextures[i] == texture) {
      listener->resourcePendingDestruction(texture);
      delete cTextures[i];
      cTextures.erase(cTextures.begin() + i);
      return;
    }
  }
}

void ImageTextureSet::dialogConfirmed(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      std::string mFileName = cCreateDialogs[i]->getFileName();
      ImageTexture* mNewTexture = new ImageTexture(mFileName);
      cRuntimeContext->add(mNewTexture, "MyFixedColour");
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      cTextures.push_back(mNewTexture);
      return;
    }
  }
}

void ImageTextureSet::dialogCancelled(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
//       Colour* mNewColour = cCreateDialogs[i]->getColour();
//       delete mNewColour;
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      return;
    }
  }
}

ImageTextureSet::~ImageTextureSet() {
  for (unsigned int i = 0; i < cTextures.size(); i++) {
    delete cTextures[i];
  }
  IMG_Quit();
}

extern "C" IPlugin* create() {
  return new ImageTextureSet();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}
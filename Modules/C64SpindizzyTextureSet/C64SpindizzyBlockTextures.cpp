#include "C64SpindizzyBlockTextures.h"

void C64SpindizzyBlockTextures::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TextureSet") {
      std::string mName = mNode->getAttribute("name");
      C64SpindizzyTextureSet* mTextureSet = new C64SpindizzyTextureSet(runtimeContext, mName);
      cTextureSets.push_back(mTextureSet);
      runtimeContext->add(mTextureSet, mNode);
      runtimeContext->addListener(mTextureSet);
    } else {
      // TODO: Throw here
    }
  }
}

void C64SpindizzyBlockTextures::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cTextureSets.size(); i++) {
    DOMNodeWriter* mSpriteSetNode = node->addBranch("TextureSet");
    cTextureSets[i]->save(mSpriteSetNode, resourceLocator);
  }
}

void C64SpindizzyBlockTextures::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "C64-style Spindizzy Textures");
  cComponentContainer = editingContext->getComponentContainer();
  cResourceSelector = editingContext->getResourceSelector();
}

void C64SpindizzyBlockTextures::createResource(IResourceAccessor* resources) {
  C64SpindizzyTextureSet* mTextureSet = new C64SpindizzyTextureSet();
  DialogSpindizzyTextureSet* mDialog = new DialogSpindizzyTextureSet(cComponentContainer, mTextureSet, resources, cResourceSelector, this);
  cCreateDialogs.push_back(mDialog);
  cComponentContainer->addComponent(mDialog);
}

void C64SpindizzyBlockTextures::editResource(ITexture* texture, IResourceAccessor* resources) {
  for (unsigned int i = 0; i < cTextureSets.size(); i++) {
    if (cTextureSets[i]->contains(texture)) {
      DialogSpindizzyTextureSet* mDialog = new DialogSpindizzyTextureSet(cComponentContainer, cTextureSets[i], resources, cResourceSelector);
      cComponentContainer->addComponent(mDialog);
      return;
    }
  }
}

void C64SpindizzyBlockTextures::removeResource(ITexture* texture, IResourceAccessor* resources, IResourceInstanceListener<ITexture>* listener) {
  for (unsigned int i = 0; i < cTextureSets.size(); i++) {
    if (cTextureSets[i]->contains(texture)) {
      cTextureSets[i]->deleteResources(listener);
      delete cTextureSets[i];
      cTextureSets.erase(cTextureSets.begin() + i);
      return;
    }
  }
}

void C64SpindizzyBlockTextures::dialogConfirmed(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      C64SpindizzyTextureSet* mNewTextures = cCreateDialogs[i]->getTextureSet();
      mNewTextures->addTextures(cRuntimeContext, "MySpindizzyTextureSet"); // TODO: This should be made sure not to clash
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      cTextureSets.push_back(mNewTextures);
      return;
    }
  }
}

void C64SpindizzyBlockTextures::dialogCancelled(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      C64SpindizzyTextureSet* mNewTextures = cCreateDialogs[i]->getTextureSet();
      delete mNewTextures;
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      return;
    }
  }
}

extern "C" IPlugin* create() {
  return new C64SpindizzyBlockTextures();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

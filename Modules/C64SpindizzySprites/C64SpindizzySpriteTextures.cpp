#include "C64SpindizzySpriteTextures.h"

void C64SpindizzySpriteTextures::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "TextureSet") {
      std::string mName = mNode->getAttribute("name");
      C64SpindizzySpriteSet* mTextureSet = new C64SpindizzySpriteSet(runtimeContext, mName);
      cSpriteSets.push_back(mTextureSet);
      runtimeContext->add(mTextureSet, mNode);
      runtimeContext->addListener(mTextureSet);
    } else {
      // TODO: Throw here
    }
  }
}

void C64SpindizzySpriteTextures::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cSpriteSets.size(); i++) {
    DOMNodeWriter* mSpriteSetNode = node->addBranch("TextureSet");
    cSpriteSets[i]->save(mSpriteSetNode, resourceLocator);
  }
}

void C64SpindizzySpriteTextures::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "C64-style Spindizzy Sprites");
  cComponentContainer = editingContext->getComponentContainer();
  cResourceSelector = editingContext->getResourceSelector();
}

void C64SpindizzySpriteTextures::createResource(IResourceAccessor* resources) {
  C64SpindizzySpriteSet* mTextureSet = new C64SpindizzySpriteSet();
  DialogSpindizzySprites* mDialog = new DialogSpindizzySprites(cComponentContainer, mTextureSet, resources, cResourceSelector, this);
  cCreateDialogs.push_back(mDialog);
  cComponentContainer->addComponent(mDialog);
}

void C64SpindizzySpriteTextures::editResource(ITexture* texture, IResourceAccessor* resources) {
  for (unsigned int i = 0; i < cSpriteSets.size(); i++) {
    if (cSpriteSets[i]->contains(texture)) {
      DialogSpindizzySprites* mDialog = new DialogSpindizzySprites(cComponentContainer, cSpriteSets[i], resources, cResourceSelector);
      cComponentContainer->addComponent(mDialog);
      return;
    }
  }
}

void C64SpindizzySpriteTextures::removeResource(ITexture* texture, IResourceAccessor* resources, IResourceInstanceListener<ITexture>* listener) {
  for (unsigned int i = 0; i < cSpriteSets.size(); i++) {
    if (cSpriteSets[i]->contains(texture)) {
      cSpriteSets[i]->deleteResources(listener);
      delete cSpriteSets[i];
      cSpriteSets.erase(cSpriteSets.begin() + i);
      return;
    }
  }
}

void C64SpindizzySpriteTextures::dialogConfirmed(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      C64SpindizzySpriteSet* mNewTextures = cCreateDialogs[i]->getSprites();
      mNewTextures->addTextures(cRuntimeContext, "MySpindizzySpriteSet"); // TODO: This should be made sure not to clash
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      cSpriteSets.push_back(mNewTextures);
      return;
    }
  }
}

void C64SpindizzySpriteTextures::dialogCancelled(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      C64SpindizzySpriteSet* mNewTextures = cCreateDialogs[i]->getSprites();
      delete mNewTextures;
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      return;
    }
  }
}

extern "C" IPlugin* create() {
  return new C64SpindizzySpriteTextures();
}

extern "C" void destroy(IPlugin* textureSet) {
  delete textureSet;
}

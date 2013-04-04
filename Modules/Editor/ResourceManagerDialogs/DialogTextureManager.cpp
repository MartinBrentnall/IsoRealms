#include "DialogTextureManager.h"

DialogTextureManager::DialogTextureManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogTextureManager", resources) {
  projectManager->addProjectListener(this);
  cAbsoluteColour = new Colour();
  cColourSelector = new ComponentColourSelector(cAbsoluteColour, resources);
  cColourPalette = new ComponentResourceBrowser<IColour, ColourIcon>(resources, resourceManager, projectManager, resourceLocator, 0.02f);
  cTextureBrowser = new ComponentResourceBrowser<ITexture, TextureIcon>(resources, resourceManager, projectManager, resourceLocator, 0.02f);
  addComponent("colourTab/colourSelector", cColourSelector);
  addComponent("paletteTab/colourPalette", cColourPalette);
  addComponent("textureTab/textureBrowser", cTextureBrowser);
  // TODO: Init resources
}

void DialogTextureManager::addColourSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cColourSelector->addColourSelectionListener(listener);
  cColourPalette->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  if (mSelectedTab == "textureTab") {
    setSelectedTab("tabbedContainer", "paletteTab");
  }
}

void DialogTextureManager::addTextureSelectionListener(IResourceSelectionListener<ITexture>* listener) {
  cTextureBrowser->addResourceSelectionListener(listener);
  std::string mSelectedTab = getSelectedTab("tabbedContainer");
  if (mSelectedTab != "textureTab") {
    setSelectedTab("tabbedContainer", "textureTab");
  }
}

void DialogTextureManager::removeColourSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cColourSelector->removeColourSelectionListener(listener);
  cColourPalette->removeResourceSelectionListener(listener);
}

void DialogTextureManager::removeTextureSelectionListener(IResourceSelectionListener<ITexture>* listener) {
  cTextureBrowser->removeResourceSelectionListener(listener);
}

void DialogTextureManager::notifyResourceReleased(IColour* colour) {
  for (unsigned int i = 0; i < cOwnedAbsoluteColours.size(); i++) {
    if (cOwnedAbsoluteColours[i] == colour) {
      delete cOwnedAbsoluteColours[i];
    }
  }
}

void DialogTextureManager::notifyResourceOwned(IColour* colour) {
  if (colour == cAbsoluteColour) {
    cOwnedAbsoluteColours.push_back(cAbsoluteColour);
    cAbsoluteColour = new Colour(cAbsoluteColour->getRed(), cAbsoluteColour->getGreen(), cAbsoluteColour->getBlue(), cAbsoluteColour->getAlpha());
    cColourSelector->setColour(cAbsoluteColour);
  }
}

void DialogTextureManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IColour*> mColours = mResourceManager->getAllColours();
  std::vector<ITexture*> mTextures = mResourceManager->getAllTextures();
  cColourPalette->initialise(mColours);
  cTextureBrowser->initialise(mTextures);
}

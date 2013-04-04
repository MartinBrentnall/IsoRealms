#include "DialogFontManager.h"

DialogFontManager::DialogFontManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogFontManager", resources) {
  projectManager->addProjectListener(this);
  cFontBrowser = new ComponentResourceBrowser<IFont, FontIcon>(resources, resourceManager, projectManager, resourceLocator, .02f);
  addComponent("fontBrowser", cFontBrowser);
  // TODO: Init resources
}

void DialogFontManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IFont*> mFonts = mResourceManager->getAllFonts();
  cFontBrowser->initialise(mFonts);
}

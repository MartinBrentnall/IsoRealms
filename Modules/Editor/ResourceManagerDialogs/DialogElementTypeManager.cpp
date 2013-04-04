#include "DialogElementTypeManager.h"

DialogElementTypeManager::DialogElementTypeManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogElementTypeManager", resources) {
  projectManager->addProjectListener(this);
  cElementTypeBrowser = new ComponentResourceBrowser<IElementType, ElementTypeIcon>(resources, resourceManager, projectManager, resourceLocator, 0.02f);
  addComponent("elementTypeBrowser", cElementTypeBrowser);
  // TODO: Init resources
}

void DialogElementTypeManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<IElementType*> mElementTypes = mResourceManager->getAllElementTypes();
  cElementTypeBrowser->initialise(mElementTypes);
}

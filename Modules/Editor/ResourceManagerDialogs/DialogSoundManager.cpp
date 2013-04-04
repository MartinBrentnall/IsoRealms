#include "DialogSoundManager.h"

DialogSoundManager::DialogSoundManager(IComponentContainer* container, IResourceAccessor* resources, IResourceManager* resourceManager, IProjectManager* projectManager, IResourceLocator* resourceLocator) : Dialog(container, "Modules/Editor/ResourceManagerDialogs/DialogSoundManager", resources) {
  projectManager->addProjectListener(this);
  cSoundBrowser = new ComponentResourceBrowser<ISound, SoundIcon>(resources, resourceManager, projectManager, resourceLocator, 0.0f);
  addComponent("soundBrowser", cSoundBrowser);
  // TODO: Init resources
}

void DialogSoundManager::projectOpened(IProject* project) {
  IResourceManager* mResourceManager = project->getResourceManager();
  std::vector<ISound*> mSounds = mResourceManager->getAllSounds();
  cSoundBrowser->initialise(mSounds);
}

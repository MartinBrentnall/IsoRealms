#ifndef DIALOG_SOUND_MANAGER_H
#define DIALOG_SOUND_MANAGER_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceType.h>

#include "ComponentResourceBrowser.h"
#include "SoundIcon.h"

class DialogSoundManager:public Dialog,
                         public IProjectManagerListener {
  private:
  Colour* cAbsoluteColour;
  std::vector<Colour*> cOwnedAbsoluteColours;
  ComponentColourSelector* cColourSelector;
  ComponentResourceBrowser<ISound, SoundIcon>* cSoundBrowser;
    
  public:
  DialogSoundManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*);
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void projectOpened(IProject* project);
};

#endif

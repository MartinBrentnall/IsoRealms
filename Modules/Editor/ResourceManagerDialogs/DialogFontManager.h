#ifndef DIALOG_FONT_MANAGER_H
#define DIALOG_FONT_MANAGER_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceType.h>

#include "ComponentResourceBrowser.h"
#include "FontIcon.h"

class DialogFontManager:public Dialog,
                        public IProjectManagerListener {
  private:
  ComponentResourceBrowser<IFont, FontIcon>* cFontBrowser;
    
  public:
  DialogFontManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*);
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void projectOpened(IProject* project);
};

#endif

#ifndef DIALOG_ELEMENT_TYPE_MANAGER_H
#define DIALOG_ELEMENT_TYPE_MANAGER_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceType.h>

#include "ComponentResourceBrowser.h"
#include "ElementTypeIcon.h"

class DialogElementTypeManager:public Dialog,
                               public IProjectManagerListener {
  private:
  Colour* cAbsoluteColour;
  std::vector<Colour*> cOwnedAbsoluteColours;
  ComponentColourSelector* cColourSelector;
  ComponentResourceBrowser<IElementType, ElementTypeIcon>* cElementTypeBrowser;
    
  public:
  DialogElementTypeManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*);
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void projectOpened(IProject* project);
};

#endif

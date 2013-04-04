#ifndef DIALOG_TEXTURE_MANAGER_H
#define DIALOG_TEXTURE_MANAGER_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>

#include "ComponentResourceBrowser.h"
#include "ColourIcon.h"
#include "TextureIcon.h"

class DialogTextureManager:public Dialog,
                           public IProjectManagerListener {
  private:
  Colour* cAbsoluteColour;
  std::vector<Colour*> cOwnedAbsoluteColours;
  ComponentColourSelector* cColourSelector;
  ComponentResourceBrowser<IColour,  ColourIcon>*  cColourPalette;
  ComponentResourceBrowser<ITexture, TextureIcon>* cTextureBrowser;
    
  public:
  DialogTextureManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*);
  
  void addColourSelectionListener(IResourceSelectionListener<IColour>* listener);
  void addTextureSelectionListener(IResourceSelectionListener<ITexture>* listener);
  void removeColourSelectionListener(IResourceSelectionListener<IColour>* listener);
  void removeTextureSelectionListener(IResourceSelectionListener<ITexture>* listener);
  void notifyResourceReleased(IColour*);
  void notifyResourceOwned(IColour*);
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void projectOpened(IProject* project);
};

#endif

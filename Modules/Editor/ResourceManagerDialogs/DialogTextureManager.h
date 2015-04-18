/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
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
  DialogTextureManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*, IEditingContext*);
  
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

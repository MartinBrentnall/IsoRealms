/*
 * Copyright 2016 Martin Brentnall
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
#ifndef DIALOG_SPINDIZZY_ZONE_THEME_SELECTOR_H
#define DIALOG_SPINDIZZY_ZONE_THEME_SELECTOR_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentCustomResourceBrowser.h>
#include <IsoRealms/GUI/Dialogs/ICustomResourceManager.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceType.h>

#include "DialogSpindizzyZoneTheme.h"
#include "ISpindizzyZoneThemeAccessor.h"
#include "SpindizzyZoneTheme.h"
#include "SpindizzyZoneThemeIcon.h"

class DialogSpindizzyZoneThemeSelector:public Dialog,
                                       public ICustomResourceManager<ISpindizzyZoneTheme>,
                                       public IResourceSelectionListener<ISpindizzyZoneTheme> {
  private:
  ISpindizzyZoneThemeAccessor* cSpindizzyZoneThemeAccessor;
  ComponentCustomResourceBrowser<ISpindizzyZoneTheme, SpindizzyZoneThemeIcon>* cSpindizzyZoneThemeBrowser;
  IComponentContainer* cWindowWorkspace;
    
  public:
  DialogSpindizzyZoneThemeSelector(IComponentContainer*, IResourceAccessor*, ISpindizzyZoneThemeAccessor*);

  /*********************************************************\
   * Implements ICustomResourceManager<SpindizzyZoneTheme> *
  \*********************************************************/
  std::string getPath(IResource*);
  void editResource(ISpindizzyZoneTheme*, IResourceAccessor*, IEditingContext*);
  
  /*************************************************************\
   * Implements IResourceSelectionListener<SpindizzyZoneTheme> *
  \*************************************************************/
  void resourceSelected(ISpindizzyZoneTheme*);
};

#endif

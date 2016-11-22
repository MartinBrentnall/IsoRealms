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
#ifndef DIALOG_SPINDIZZY_ZONE_THEME_H
#define DIALOG_SPINDIZZY_ZONE_THEME_H

#include <IsoRealms/GUI/ComponentResourceColourSelector.h>
#include <IsoRealms/GUI/ComponentResourceTextureSelector.h>
#include <IsoRealms/GUI/ComponentTable.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

#include "ISpindizzyZoneTheme.h"

class DialogSpindizzyZoneTheme:public DialogOKCancelUndo,
                               public IComponentSelectorListener<IColour>,
                               public IComponentSelectorListener<ITexture> {
  private:
  ComponentTable* cTable;
  
  public:
  DialogSpindizzyZoneTheme(IEditingContext*, IResourceAccessor*, ISpindizzyZoneTheme*);
  
  /*********************************\
   * Implements DialogOKCancelUndo *
  \*********************************/
  void undo();
  
  /**********************8***************************\
   * Implements IComponentSelectorListener<IColour> *
  \**************************************************/
  void selected(ISelector*, IColour*);

  /**********************8****************************\
   * Implements IComponentSelectorListener<ITexture> *
  \***************************************************/
  void selected(ISelector*, ITexture*);
};

#endif


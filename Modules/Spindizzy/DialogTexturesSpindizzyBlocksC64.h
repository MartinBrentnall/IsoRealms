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
#ifndef DIALOG_TEXTURES_SPINDIZZY_BLOCKS_C64_H
#define DIALOG_TEXTURES_SPINDIZZY_BLOCKS_C64_H

#include <IsoRealms/GUI/ComponentResourceColourSelector.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/GUI/IComponentSelectorListener.h>
#include <IsoRealms/GUI/Icons/ComponentIconTexture.h>
#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceTexturesSpindizzyBlocksC64.h"

class DialogTexturesSpindizzyBlocksC64:public IComponentSelectorListener<IColour>,
                                       public DialogOKCancelUndo {
  private:
  DialogOKCancelUndo* cDialog;
  
  ResourceTexturesSpindizzyBlocksC64* cTextureSet;
  IResourceSelector* cResourceSelector;
  IColour* cOriginalFloorColour;
  IColour* cOriginalWallColour;
  IColour* cOriginalGridColour;
  IColour* cOriginalDetailColour;
  ComponentResourceColourSelector* cColourSelectorFloor;
  ComponentResourceColourSelector* cColourSelectorWall;
  ComponentResourceColourSelector* cColourSelectorGrid;
  ComponentResourceColourSelector* cColourSelectorDetail;
  
  void undo();
  
  public:
  DialogTexturesSpindizzyBlocksC64(IEditingContext*, ResourceTexturesSpindizzyBlocksC64*, IResourceAccessor*, const std::string&);
  
  ResourceTexturesSpindizzyBlocksC64* getResource();
  
  /**************************************************\
   * Implements IComponentSelectorListener<IColour> *
  \**************************************************/
  void selected(ISelector*, IColour*);
};

#endif

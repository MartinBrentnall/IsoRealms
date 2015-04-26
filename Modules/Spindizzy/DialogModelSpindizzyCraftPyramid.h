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
#ifndef DIALOG_MODEL_SPINDIZZY_CRAFT_PYRAMID_H
#define DIALOG_MODEL_SPINDIZZY_CRAFT_PYRAMID_H

#include <IsoRealms/GUI/ComponentResourceColourSelector.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/GUI/Icons/ComponentIconModel.h>
#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceModelSpindizzyCraftPyramid.h"

class DialogModelSpindizzyCraftPyramid : public IComponentSelectorListener<IColour>,
                                         public DialogOKCancelUndo {
  private:
  ResourceModelSpindizzyCraftPyramid* cModelType;
  IColour* cOriginalTopColour;
  IColour* cOriginalSideColour;
  IColour* cOriginalOutlineColour;
  ComponentResourceColourSelector* cColourSelectorTop;
  ComponentResourceColourSelector* cColourSelectorSide;
  ComponentResourceColourSelector* cColourSelectorOutline;
  
  void undo();
  
  public:
  DialogModelSpindizzyCraftPyramid(IEditingContext*, ResourceModelSpindizzyCraftPyramid*, IResourceAccessor*, const std::string&);
  
  ResourceModelSpindizzyCraftPyramid* getResource();
  
  /**************************************************\
   * Implements IComponentSelectorListener<IColour> *
  \**************************************************/
  void selected(ISelector*, IColour*);
};

#endif

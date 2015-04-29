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
#ifndef DIALOG_MODEL_SPINDIZZY_JEWEL_H
#define DIALOG_MODEL_SPINDIZZY_JEWEL_H

#include <IsoRealms/GUI/ComponentResourceColourSelector.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/GUI/Icons/ComponentIconModel.h>
#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceModelSpindizzyJewel.h"

class DialogModelSpindizzyJewel : public IComponentSelectorListener<IColour>,
                                  public DialogOKCancelUndo {
  ResourceModelSpindizzyJewel* cModelType;
  IResourceSelector* cResourceSelector;
  IColour* cOriginalFrameColour;
  std::vector<IColour*> cOriginalCycleColours;
  ComponentResourceColourSelector* cColourSelectorFrame;
  std::vector<ComponentResourceColourSelector*> cColourSelectorsCycle;
  
  class StringListener:public IValueListener<std::string> {
    private:
    DialogModelSpindizzyJewel* cParent;
    
    public:
    StringListener(DialogModelSpindizzyJewel*);
    
    /******************************\
     * Implements IStringListener *
    \******************************/
    void valueChanged(std::string);
  };
    
  void undo();
  void setCycleColours(unsigned int);  
  
  public:
  DialogModelSpindizzyJewel(IEditingContext*, ResourceModelSpindizzyJewel*, IResourceAccessor*, const std::string&);
  
  ResourceModelSpindizzyJewel* getResource();
  
  /**************************************************\
   * Implements IComponentSelectorListener<IColour> *
  \**************************************************/
  void selected(ISelector*, IColour*);  
};

#endif

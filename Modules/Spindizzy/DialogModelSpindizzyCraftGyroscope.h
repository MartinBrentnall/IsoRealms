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
#ifndef DIALOG_MODEL_SPINDIZZY_CRAFT_GYROSCOPE_H
#define DIALOG_MODEL_SPINDIZZY_CRAFT_GYROSCOPE_H

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceModelSpindizzyCraftGyroscope.h"

class DialogModelSpindizzyCraftGyroscope : public DialogOKCancelUndo {
  private:
  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogModelSpindizzyCraftGyroscope* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogModelSpindizzyCraftGyroscope*, IColour*, unsigned int);
    
    void render(SelectableComponent*);
    void selected();
    void deselected();
    
    void resourceSelected(IColour*);
  };
    
  class ModelIcon:public ISizedComponent {
    private:
    I3DModelType* cModelType;
    
    public:
    ModelIcon(ResourceModelSpindizzyCraftGyroscope*);
        
    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(unsigned int);
    bool input(SDL_Event&);
  };
  
  ResourceModelSpindizzyCraftGyroscope* cModelType;
  IResourceSelector* cResourceSelector;
  IColour* cOriginalColour1;
  IColour* cOriginalColour2;
  IColour* cOriginalColour3;
  IColour* cOriginalColour4;
  IColour* cOriginalOutlineColour;
  ColourSelector* cColourSelector1;
  ColourSelector* cColourSelector2;
  ColourSelector* cColourSelector3;
  ColourSelector* cColourSelector4;
  ColourSelector* cColourSelectorOutline;
  
  
  public:
  DialogModelSpindizzyCraftGyroscope(IEditingContext*, ResourceModelSpindizzyCraftGyroscope*, IResourceAccessor*, const std::string&);
  
  ResourceModelSpindizzyCraftGyroscope* getResource();
  void undo();
};

#endif

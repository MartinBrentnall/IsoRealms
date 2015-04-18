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

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceModelSpindizzyCraftPyramid.h"

class DialogModelSpindizzyCraftPyramid:public ResizableDialog {
  private:
  class OKCommand:public ICommand {
    private:
    DialogModelSpindizzyCraftPyramid* cParent;
    
    public:
    OKCommand(DialogModelSpindizzyCraftPyramid*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogModelSpindizzyCraftPyramid* cParent;
    
    public:
    CancelCommand(DialogModelSpindizzyCraftPyramid*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogModelSpindizzyCraftPyramid* cParent;
    
    public:
    UndoCommand(DialogModelSpindizzyCraftPyramid*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogModelSpindizzyCraftPyramid* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogModelSpindizzyCraftPyramid*, IColour*, unsigned int);
    
    void render(SelectableComponent*);
    void selected();
    void deselected();
    
    void resourceSelected(IColour*);
  };
    
  class ModelIcon:public ISizedComponent {
    private:
    I3DModelType* cModelType;
    
    public:
    ModelIcon(ResourceModelSpindizzyCraftPyramid*);
        
    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(unsigned int);
    bool input(SDL_Event&);
  };
  
  ResourceModelSpindizzyCraftPyramid* cModelType;
  IResourceSelector* cResourceSelector;
  IConfirmationListener* cConfirmationListener;
  IColour* cOriginalTopColour;
  IColour* cOriginalSideColour;
  IColour* cOriginalOutlineColour;
  ColourSelector* cColourSelectorTop;
  ColourSelector* cColourSelectorSide;
  ColourSelector* cColourSelectorOutline;
  
  void undo();
  
  public:
  DialogModelSpindizzyCraftPyramid(IEditingContext*, ResourceModelSpindizzyCraftPyramid*, IResourceAccessor*);

  void addConfirmationListener(IConfirmationListener*);
  
  ResourceModelSpindizzyCraftPyramid* getResource();
  std::string getResourceName();
};

#endif

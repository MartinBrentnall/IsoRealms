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

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceModelSpindizzyJewel.h"

class DialogModelSpindizzyJewel:public ResizableDialog {
  private:
  class OKCommand:public ICommand {
    private:
    DialogModelSpindizzyJewel* cParent;
    
    public:
    OKCommand(DialogModelSpindizzyJewel*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogModelSpindizzyJewel* cParent;
    
    public:
    CancelCommand(DialogModelSpindizzyJewel*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogModelSpindizzyJewel* cParent;
    
    public:
    UndoCommand(DialogModelSpindizzyJewel*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogModelSpindizzyJewel* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogModelSpindizzyJewel*, IColour*, unsigned int);
    
    void render(SelectableComponent*);
    void selected();
    void deselected();
    
    void resourceSelected(IColour*);
  };
    
  class ModelIcon:public ISizedComponent {
    private:
    ResourceModelSpindizzyJewel* cModelType;
    
    public:
    ModelIcon(ResourceModelSpindizzyJewel*);
        
    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(unsigned int);
    bool input(SDL_Event&);
  };
  
  ResourceModelSpindizzyJewel* cModelType;
  IResourceSelector* cResourceSelector;
  IConfirmationListener* cConfirmationListener;
  IColour* cOriginalFrameColour;
  std::vector<IColour*> cOriginalCycleColours;
  ColourSelector* cColourSelectorFrame;
  std::vector<ColourSelector*> cColourSelectorsCycle;
  
  void undo();
  
  public:
  DialogModelSpindizzyJewel(IEditingContext*, ResourceModelSpindizzyJewel*, IResourceAccessor*, const std::string&);
  
  void addConfirmationListener(IConfirmationListener*);
  
  ResourceModelSpindizzyJewel* getResource();
  std::string getResourceName();
};

#endif

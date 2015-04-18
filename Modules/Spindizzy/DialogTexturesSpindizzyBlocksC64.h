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

#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>

#include "ResourceTexturesSpindizzyBlocksC64.h"

class DialogTexturesSpindizzyBlocksC64:public ResizableDialog {
  private:
  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogTexturesSpindizzyBlocksC64* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogTexturesSpindizzyBlocksC64*, IColour*, unsigned int);
    
    void render(SelectableComponent*);
    void selected();
    void deselected();
    
    void resourceSelected(IColour*);
  };
    
  class TextureIcon:public ISizedComponent {
    private:
    ITexture* cTexture;
    
    public:
    TextureIcon(ITexture*);
        
    /******************************\
     * Implements ISizedComponent *
    \******************************/
    float getWidth();
    float getHeight();
    void render();
    void update(unsigned int);
    bool input(SDL_Event&);
  };
  
  ResourceTexturesSpindizzyBlocksC64* cTextureSet;
  IResourceSelector* cResourceSelector;
  IConfirmationListener* cConfirmationListener;
  IColour* cOriginalFloorColour;
  IColour* cOriginalWallColour;
  IColour* cOriginalGridColour;
  IColour* cOriginalDetailColour;
  ColourSelector* cColourSelectorFloor;
  ColourSelector* cColourSelectorWall;
  ColourSelector* cColourSelectorGrid;
  ColourSelector* cColourSelectorDetail;
  
  void undo();
  
  public:
  DialogTexturesSpindizzyBlocksC64(IEditingContext*, ResourceTexturesSpindizzyBlocksC64*, IResourceAccessor*);
  
  void addConfirmationListener(IConfirmationListener*);
  
  ResourceTexturesSpindizzyBlocksC64* getResource();
  std::string getResourceName();
};

#endif

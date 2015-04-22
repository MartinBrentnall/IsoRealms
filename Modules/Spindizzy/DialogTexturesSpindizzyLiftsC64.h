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
#ifndef DIALOG_TEXTURE_SPINDIZZY_LIFTS_C64_H
#define DIALOG_TEXTURE_SPINDIZZY_LIFTS_C64_H

#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/GUI/ISelector.h>
#include <IsoRealms/GUI/SelectableComponent.h>
#include <IsoRealms/IConfirmationListener.h>

#include "ResourceTexturesSpindizzyLiftsC64.h"

class DialogTexturesSpindizzyLiftsC64:public DialogOKCancelUndo {
  private:
  class ColourSelector:public ISelector,
                       public IResourceSelectionListener<IColour> {
    private:
    unsigned int cWhich;
    DialogTexturesSpindizzyLiftsC64* cParent;
    IColour* cBorrowedColour;
    IColour* cColour;
    
    public:
    ColourSelector(DialogTexturesSpindizzyLiftsC64*, IColour*, unsigned int);
    
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
  
  ResourceTexturesSpindizzyLiftsC64* cSprites;
  IResourceSelector* cResourceSelector;
  IColour* cOriginalColour1;
  IColour* cOriginalColour2;
  IColour* cOriginalColour3;
  IColour* cOriginalColourOutline;
  ColourSelector* cColourSelector1;
  ColourSelector* cColourSelector2;
  ColourSelector* cColourSelector3;
  ColourSelector* cColourSelectorOutline;
  
  void undo();
  
  public:
  DialogTexturesSpindizzyLiftsC64(IEditingContext*, ResourceTexturesSpindizzyLiftsC64*, IResourceAccessor*, const std::string&);
  
  ResourceTexturesSpindizzyLiftsC64* getResource();
  std::string getResourceName();
};

#endif

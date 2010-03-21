/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef SOUND_CONFIGURATION_DIALOG_H
#define SOUND_CONFIGURATION_DIALOG_H

#include <string>

#include "../../../Global/GUI/Button.h"
#include "../../../Global/GUI/ComponentEdgeLayout.h"
#include "../../../Global/GUI/EdgeRelation.h"
#include "../../../Global/GUI/GridLayoutComponent.h"
#include "../../../Global/GUI/ListBox.h"
#include "../../../Global/GUI/ResizableDialog.h"
#include "../../../Global/GUI/ScrollableContainer.h"
#include "../../../Global/GUI/TextLabelComponent.h"
#include "../../../Global/System.h"

#include "ISoundRegistry.h"

class SoundConfigurationDialog:public ResizableDialog {
  private:
  ISoundRegistry* cSoundRegistry;
  ICommand* cUseSoundCommand;
  ICommand* cRemoveSoundCommand;
  ListBox cAvailableList;
  ListBox cUsingList;

  class UseSoundCommand:public ICommand {
    private:
    SoundConfigurationDialog* cParent;

    public:
    UseSoundCommand(SoundConfigurationDialog*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class RemoveSoundCommand:public ICommand {
    private:
    SoundConfigurationDialog* cParent;

    public:
    RemoveSoundCommand(SoundConfigurationDialog*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:
  SoundConfigurationDialog(IComponentContainer*, ISoundRegistry*);
};

#endif

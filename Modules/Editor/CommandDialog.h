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
#ifndef COMMAND_DIALOG_H
#define COMMAND_DIALOG_H

#include <IsoRealms/ICommand.h>

#include "DialogModules.h"
#include "IDialogParent.h"
#include "IEditor.h"

template <class DIALOG> class CommandDialog:public ICommand,
                                            public IDialogParent {
  private:
  IEditor* cEditor;
  IComponentContainer* cComponentContainer;
  DIALOG* cInstance;
  
  public:
  CommandDialog(IEditor* editor, IComponentContainer* componentContainer) {
    cEditor = editor;
    cComponentContainer = componentContainer;
    cInstance = nullptr;
  }
    
  void execute() {
    if (cInstance == nullptr) {
      cInstance = new DIALOG(cComponentContainer, nullptr, cEditor, this);
      cComponentContainer->addComponent(cInstance);
    } else {
      // TODO: Focus the existing dialog.
    }
  }
  
  void dialogClosed(Dialog* dialog) {
    if (dialog == cInstance) {
      cInstance = nullptr;
    }
  }
};

#endif

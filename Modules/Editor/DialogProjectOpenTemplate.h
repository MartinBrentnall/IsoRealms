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
#ifndef DIALOG_PROJECT_OPEN_TEMPLATE_H
#define DIALOG_PROJECT_OPEN_TEMPLATE_H

#include <IsoRealms/GUI/Dialogs/ComponentFileSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IConfirmationListener.h>

#include "IEditor.h"

class DialogProjectOpenTemplate:public ResizableDialog,
                                public IFileSelectionListener {
  private:
  ComponentFileSelector* cFileSelector;
  std::string cFile;
  IEditor* cEditor;
  
  class OKCommand:public ICommand {
    private:
    DialogProjectOpenTemplate* cParent;
    
    public:
    OKCommand(DialogProjectOpenTemplate*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogProjectOpenTemplate* cParent;
    
    public:
    CancelCommand(DialogProjectOpenTemplate*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  void confirmSelection();

  public:
  DialogProjectOpenTemplate(IComponentContainer*, IResourceAccessor*, IEditor*);
  
  /*************************************\
   * Implements IFileSelectionListener *
  \*************************************/
  void fileSelectionAsserted(const std::string&);
};

#endif

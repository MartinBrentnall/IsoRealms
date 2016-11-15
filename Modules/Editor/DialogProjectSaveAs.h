/*
 * Copyright 2016 Martin Brentnall
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
#ifndef DIALOG_PROJECT_SAVE_AS_H
#define DIALOG_PROJECT_SAVE_AS_H

#include <IsoRealms/GUI/Dialogs/ComponentFileSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IConfirmationListener.h>

#include "IMapManager.h"

class DialogProjectSaveAs:public ResizableDialog,
                          public IFileSelectionListener {
  private:
  ComponentFileSelector* cFileSelector;
  std::string cFile;
  IMapManager* cMapManager;
  
  class OKCommand:public ICommand {
    private:
    DialogProjectSaveAs* cParent;
    
    public:
    OKCommand(DialogProjectSaveAs*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogProjectSaveAs* cParent;
    
    public:
    CancelCommand(DialogProjectSaveAs*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  void confirmSelection();

  public:
  DialogProjectSaveAs(IComponentContainer*, IResourceAccessor*, IMapManager*);
  
  /*************************************\
   * Implements IFileSelectionListener *
  \*************************************/
  void fileSelectionAsserted(const std::string&);
};

#endif

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
#ifndef DIALOG_SOUND_FILE_H
#define DIALOG_SOUND_FILE_H

#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/GUI/Dialogs/ComponentFileSelector.h>
#include <IsoRealms/GUI/ResizableDialog.h>

#include "ResourceSoundFile.h"

class DialogSoundFile:public ResizableDialog,
                      public IFileSelectionListener {
  private:
  ComponentFileSelector* cFileSelector;
  IConfirmationListener* cConfirmationListener;
  std::string cFile;
  
  class OKCommand:public ICommand {
    private:
    DialogSoundFile* cParent;
    
    public:
    OKCommand(DialogSoundFile*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogSoundFile* cParent;
    
    public:
    CancelCommand(DialogSoundFile*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  void confirmSelection();
  
  public:
  DialogSoundFile(IEditingContext*, ResourceSoundFile*, IResourceAccessor*, const std::string&);
  
  void addConfirmationListener(IConfirmationListener*);
  
  ResourceSoundFile* getResource();
  std::string getResourceName();

  std::string getFileName();
  
  /*************************************\
   * Implements IFileSelectionListener *
  \*************************************/
  void fileSelectionAsserted(const std::string&);
};

#endif

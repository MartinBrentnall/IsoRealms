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
#ifndef DIALOG_OK_CANCEL_UNDO_H
#define DIALOG_OK_CANCEL_UNDO_H

#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/Resources/IResourceAccessor.h>

class DialogOKCancelUndo:public ResizableDialog {
  private:
  class OKCommand:public ICommand {
    private:
    DialogOKCancelUndo* cParent;
    
    public:
    OKCommand(DialogOKCancelUndo*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class CancelCommand:public ICommand {
    private:
    DialogOKCancelUndo* cParent;
    
    public:
    CancelCommand(DialogOKCancelUndo*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class UndoCommand:public ICommand {
    private:
    DialogOKCancelUndo* cParent;
    
    public:
    UndoCommand(DialogOKCancelUndo*);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  std::vector<IConfirmationListener*> cConfirmationListeners;
  
  public:
  DialogOKCancelUndo(IEditingContext*, IResourceAccessor*, const std::string&, const std::string&);
    
  void addConfirmationListener(IConfirmationListener*);
  void removeConfirmationListener(IConfirmationListener*);
  
  virtual void undo() = 0;
};

#endif

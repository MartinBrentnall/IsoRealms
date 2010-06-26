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
#ifndef CONFIRMATION_BOX_H
#define CONFIRMATION_BOX_H

#include <string>

#include <IsoRealms/GUI/Button.h>
#include <IsoRealms/GUI/ComponentEdgeLayout.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/TextLabelComponent.h>
#include <IsoRealms/ICommand.h>

class ConfirmationBox:public ResizableDialog {
  private:
  std::string cMessage;

  class OKCommand:public ICommand {
    private:
    ConfirmationBox* cParent;
    ICommand* cCommand;

    public:
    OKCommand(ConfirmationBox*, ICommand*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class CancelCommand:public ICommand {
    private:
    ConfirmationBox* cParent;

    public:
    CancelCommand(ConfirmationBox*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  public:

  /**
   * 
   */
  ConfirmationBox(IComponentContainer*, ICommand*, std::string&, std::string&);

  /******************************\
   * Implements ResizableDialog *
  \******************************/
  void updateResizableDialogContent(int);
  void renderResizableDialogContent();
  bool inputResizableDialogContent(SDL_Event&);
};

#endif

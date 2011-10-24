/*
 * Copyright 2009 Martin Brentnall
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
#ifndef ENTER_MENU_COMMAND
#define ENTER_MENU_COMMAND

#include <IsoRealms/ICommand.h>

#include "FrontEndMenu.h"
#include "IMenuStack.h"

/**
 * This command performs the following actions:
 * 
 * It updates the menu being displayed.
 * It updates the location within the menu hierarchy.
 * It resets the selected menu item to zero.
 */
class EnterMenuCommand:public ICommand {
  private:
  IMenuStack* cMenuStack;
  FrontEndMenu* cMenuToEnter;

  public:

  /**
   * 
   * 
   * @param
   */
  EnterMenuCommand(IMenuStack*, FrontEndMenu*);
    
  /***********************\
   * Implements ICommand *
  \***********************/
  void execute();  
};

#endif


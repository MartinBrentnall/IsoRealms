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
#ifndef POP_CONTROL_LOOP_COMMAND_H
#define POP_CONTROL_LOOP_COMMAND_H

#include <iostream>
#include <stack>

#include <IsoRealms/ICommand.h>

#include "IControlLoop.h"

class PopControlLoopCommand:public ICommand {
  private:
  std::stack<IControlLoop*>* cControlLoopStackPointer;
  
  public:
  PopControlLoopCommand(std::stack<IControlLoop*>*);

  /*************************************************************************\
   * Implemented methods of ICommand.h                                     *
  \*************************************************************************/
  void execute();  
};

#endif

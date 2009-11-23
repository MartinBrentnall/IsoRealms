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
#ifndef GENERATE_ENGINE_COMMAND_H
#define GENERATE_ENGINE_COMMAND_H

#include <queue>

#include "../../Global/ICommand.h"

/**
 * When executed, this command will register a command to be executed on the
 * next cycle of engine.  This avoids the possibility of changing a pointer
 * whilst it is in use.
 */
class GenerateEngineCommand:public ICommand {
  private:
  ICommand* cEngineCommand;
  std::queue<ICommand*>* cEngineCommandQueue;

  public:
  GenerateEngineCommand(std::queue<ICommand*>*, ICommand*);
    
  /*************************************************************************\
   * Implemented methods of ICommand.h                                     *
  \*************************************************************************/
  void execute();  
};

#endif

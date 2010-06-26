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
#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>
#include <vector>

#include <IsoRealms/CommandManager.h>
#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/Map.h>

#include "../../IControlLoop.h"

class Runtime:public IControlLoop {
  private:
  bool cRunExitCommands;
  std::vector<ICommand*> cExitCommands;
  Map* cMap;

  std::vector<ICommand*> parseCommands(DOMNodeWrapper* node);
  void keyDown(SDLKey&);

  public:
  Runtime(DOMNodeWrapper*);

  /***************************\
   * Implements IControlLoop *
  \***************************/
  void input(SDL_Event&);
  void execute(int);
};

#endif

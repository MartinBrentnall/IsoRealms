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
#ifndef INPUT_COMMANDS_H
#define INPUT_COMMANDS_H

#include <vector>

#include <IsoRealms/IInteractiveElement.h>

#include "../../CommandRegistry/ICommandRegistry.h"

#include "../IUtilities.h"

class InputCommands:public IUtilities,
                    public IInteractiveElement {
  private:
  std::vector<PlugSocket*> cSockets;
  ICommandRegistry* cCommandRegistry;
  std::map<SDLKey, std::vector<IUserCommand*> > cKeyCommands;

  bool keyDown(SDLKey&);
  SDLKey getSDLKey(const std::string&);
  
  std::vector<IUserCommand*> getCommands(DOMNodeWrapper*);

  public:
  InputCommands();

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
  std::vector<IInteractiveElement*> getInteractiveElements();

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************************\
   * Implements IInteractiveElement *
  \**********************************/
  bool input(SDL_Event&);
};

#endif

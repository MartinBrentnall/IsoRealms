/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef FRONT_END_MENU_ITEM_H
#define FRONT_END_MENU_ITEM_H

#include <GL/glew.h>
#include <string>

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/Resources/Font/IFont.h>

#include "FocusAction.h"
#include "IFrontEndCommands.h"
#include "IFrontEndMenuItem.h"

class FrontEndMenuItem:public IFrontEndMenuItem {
  private:
  std::string cMenuText;
  ICommand* cCommand;

  public:
  FrontEndMenuItem(IFrontEndCommands*, DOMNodeWrapper*);
  FrontEndMenuItem(const std::string&, ICommand*);

  /********************************\
   * Implements IFrontEndMenuItem *
  \********************************/
  void render(int, float, IFont*, bool);
  FocusAction keyDown(SDLKey&);
};

#endif

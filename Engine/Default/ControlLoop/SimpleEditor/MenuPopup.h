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
#ifndef MENU_POPUP_H
#define MENU_POPUP_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <map>
#include <vector>

#include "../../../../Global/Configuration.h"
#include "../../../../Global/DOMNodeWrapper.h"
#include "../../../../Global/ICommand.h"
#include "../../../../Global/ICommandInfo.h"
#include "../../../../Global/ScreenConfiguration.h"

class PopupMenuCommand;

#include "EditorCommandManager.h"
#include "IMenuContainer.h"
#include "IMenu.h"
#include "MenuItem.h"
#include "PopupMenuCommand.h"

class MenuPopup:public IMenu {
  private:
  std::map<std::string, MenuPopup*> cMenuPopups;
  std::vector<MenuItem*> cMenuItems;
  float cX;
  float cY;
  IMenuContainer* cParent;
  unsigned int cSelectedItem;

  ICommand* parseCommand(DOMNodeWrapper*);

  bool mouseButtonDown(SDL_Event&);
  bool mouseMotion(SDL_Event&);
  bool keyDown(SDLKey&);

  void addMenuItem(std::string, ICommand*);

  public:
  MenuPopup(DOMNodeWrapper*, IMenuContainer*, float, float);
  MenuPopup(IMenuContainer*, float, float);

  void addCommand(std::vector<std::string>, ICommandInfo*);

  float getX();
  float getY();
  float getWidth();
  float getHeight();

  void render();
  bool input(SDL_Event&);    
};

#endif
